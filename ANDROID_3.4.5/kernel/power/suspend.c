/*
 * kernel/power/suspend.c - Suspend to RAM and standby functionality.
 *
 * Copyright (c) 2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
 * Copyright (c) 2009 Rafael J. Wysocki <rjw@sisk.pl>, Novell Inc.
 *
 * This file is released under the GPLv2.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/cpu.h>
#include <linux/syscalls.h>
#include <linux/gfp.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/rtc.h>
#include <trace/events/power.h>
#include <linux/cpufreq.h>

#include <mach/hardware.h>
#include <mach/wmt_secure.h>

#include "power.h"

unsigned int pm_flags;
EXPORT_SYMBOL(pm_flags);

#ifdef CONFIG_KEYBOARD_WMT
extern void wmt_kpad_int_ctrl(int state);
#endif

const char *const pm_states[PM_SUSPEND_MAX] = {
	[PM_SUSPEND_STANDBY]	= "standby",
	[PM_SUSPEND_MEM]	= "mem",
};

extern unsigned int WMT_WAKE_UP_EVENT;

static const struct platform_suspend_ops *suspend_ops;

/**
 * suspend_set_ops - Set the global suspend method table.
 * @ops: Suspend operations to use.
 */
void suspend_set_ops(const struct platform_suspend_ops *ops)
{
	lock_system_sleep();
	suspend_ops = ops;
	unlock_system_sleep();
}
EXPORT_SYMBOL_GPL(suspend_set_ops);

bool valid_state(suspend_state_t state)
{
	/*
	 * All states need lowlevel support and need to be valid to the lowlevel
	 * implementation, no valid callback implies that none are valid.
	 */
	return suspend_ops && suspend_ops->valid && suspend_ops->valid(state);
}

/**
 * suspend_valid_only_mem - Generic memory-only valid callback.
 *
 * Platform drivers that implement mem suspend only and only need to check for
 * that in their .valid() callback can use this instead of rolling their own
 * .valid() callback.
 */
int suspend_valid_only_mem(suspend_state_t state)
{
	return state == PM_SUSPEND_MEM;
}
EXPORT_SYMBOL_GPL(suspend_valid_only_mem);

static int suspend_test(int level)
{
#ifdef CONFIG_PM_DEBUG
	if (pm_test_level == level) {
		printk(KERN_INFO "suspend debug: Waiting for 5 seconds.\n");
		mdelay(5000);
		return 1;
	}
#endif /* !CONFIG_PM_DEBUG */
	return 0;
}

/**
 * suspend_prepare - Prepare for entering system sleep state.
 *
 * Common code run for every system sleep state that can be entered (except for
 * hibernation).  Run suspend notifiers, allocate the "suspend" console and
 * freeze processes.
 */
static int suspend_prepare(void)
{
	int error;

	if (!suspend_ops || !suspend_ops->enter)
		return -EPERM;

	pm_prepare_console();

	error = pm_notifier_call_chain(PM_SUSPEND_PREPARE);
	if (error)
		goto Finish;

	error = suspend_freeze_processes();
	if (!error)
		return 0;

	suspend_stats.failed_freeze++;
	dpm_save_failed_step(SUSPEND_FREEZE);
 Finish:
	pm_notifier_call_chain(PM_POST_SUSPEND);
	pm_restore_console();
	return error;
}

/* default implementation */
void __attribute__ ((weak)) arch_suspend_disable_irqs(void)
{
	local_irq_disable();
}

/* default implementation */
void __attribute__ ((weak)) arch_suspend_enable_irqs(void)
{
	local_irq_enable();
}

extern unsigned int cpu_trustzone_enabled;

/**
 * suspend_enter - Make the system enter the given sleep state.
 * @state: System sleep state to enter.
 * @wakeup: Returns information that the sleep state should not be re-entered.
 *
 * This function should be called after devices have been suspended.
 */
static int suspend_enter(suspend_state_t state, bool *wakeup)
{
	int error;

	if (suspend_ops->prepare) {
		error = suspend_ops->prepare();
		if (error)
			goto Platform_finish;
	}

	error = dpm_suspend_end(PMSG_SUSPEND);
	if (error) {
		printk(KERN_ERR "PM: Some devices failed to power down\n");
		goto Platform_finish;
	}


	if (suspend_ops->prepare_late) {
		error = suspend_ops->prepare_late();
		if (error)
			goto Platform_wake;
	}

	if (suspend_test(TEST_PLATFORM))
		goto Platform_wake;

	error = disable_nonboot_cpus();
	if (error || suspend_test(TEST_CPUS))
		goto Enable_cpus;

	arch_suspend_disable_irqs();
	BUG_ON(!irqs_disabled());

	/*Secure OS FIQ disable*/
	if (cpu_trustzone_enabled == 1) {
			wmt_smc(WMT_SMC_CMD_SECURE_GIC_CTL, GIC_DISABLE);
	}

	error = syscore_suspend();

	/*Secure OS GIC suspend*/
	if (cpu_trustzone_enabled == 1) {
			wmt_smc(WMT_SMC_CMD_SECURE_GIC_CTL, GIC_SUSPEND);
	}
	
	WMT_WAKE_UP_EVENT = 0;
	if (!error) {

		unsigned int pmc_temp;		
		pmc_temp = PMWS_VAL;
		pmc_temp &= (WK_TRG_EN_VAL | 0x4000);
		if (!pmc_temp) {			
			*wakeup = pm_wakeup_pending();
			if (!(suspend_test(TEST_CORE) || *wakeup)) {
				error = suspend_ops->enter(state);
				events_check_enabled = false;
			}
		} else {
			WMT_WAKE_UP_EVENT = (PMWS_VAL & (WK_TRG_EN_VAL | 0x4000));//wmt_pm_enter
		}
		
		/*Secure OS GIC resume*/
		if (cpu_trustzone_enabled == 1) {
			wmt_smc(WMT_SMC_CMD_SECURE_GIC_CTL, GIC_RESUME);
		}

	
		syscore_resume();
	}
	/*Disable Secure OS FIQ*/
	if (cpu_trustzone_enabled == 1) {
			wmt_smc(WMT_SMC_CMD_SECURE_GIC_CTL, GIC_ENABLE);

	}

	arch_suspend_enable_irqs();
	BUG_ON(irqs_disabled());

 Enable_cpus:
	enable_nonboot_cpus();

 Platform_wake:
	if (suspend_ops->wake)
		suspend_ops->wake();

	dpm_resume_start(PMSG_RESUME);

 Platform_finish:
	if (suspend_ops->finish)
		suspend_ops->finish();

	return error;
}

/**
 * suspend_devices_and_enter - Suspend devices and enter system sleep state.
 * @state: System sleep state to enter.
 */

extern int wmt_trigger_resume_kpad;
extern int wmt_trigger_resume_notify;
extern void wmt_resume_kpad(void);
extern void wmt_resume_notify(void);

int suspend_devices_and_enter(suspend_state_t state)
{
	int error;
	bool wakeup = false;

	if (!suspend_ops)
		return -ENOSYS;

	trace_machine_suspend(state);

	if (suspend_ops->begin) {
		error = suspend_ops->begin(state);
		if (error)
			goto Close;
	}
	/*
	suspend_console();
	*/
	suspend_test_start();
	error = dpm_suspend_start(PMSG_SUSPEND);
	if (error) {
		printk(KERN_ERR "PM: Some devices failed to suspend\n");
		goto Recover_platform;
	}
	suspend_test_finish("suspend devices");
	if (suspend_test(TEST_DEVICES))
		goto Recover_platform;

	do {
		error = suspend_enter(state, &wakeup);
	} while (!error && !wakeup
		&& suspend_ops->suspend_again && suspend_ops->suspend_again());

 Resume_devices:
 
	if (wmt_trigger_resume_kpad){
	  wmt_trigger_resume_kpad=0;
	  wmt_resume_kpad();
	}
	if (wmt_trigger_resume_notify){
	  wmt_trigger_resume_notify=0;
	  wmt_resume_notify();
	}

 
	suspend_test_start();
	dpm_resume_end(PMSG_RESUME);
	suspend_test_finish("resume devices");
	resume_console();
	printk("WMT wake up event %x\n",WMT_WAKE_UP_EVENT);
	WMT_WAKE_UP_EVENT = 0;
 Close:
	if (suspend_ops->end)
		suspend_ops->end();

	trace_machine_suspend(PWR_EVENT_EXIT);
	return error;

 Recover_platform:
	if (suspend_ops->recover)
		suspend_ops->recover();
	goto Resume_devices;
}

/**
 * suspend_finish - Clean up before finishing the suspend sequence.
 *
 * Call platform code to clean up, restart processes, and free the console that
 * we've allocated. This routine is not called for hibernation.
 */
static void suspend_finish(void)
{
	suspend_thaw_processes();
	pm_notifier_call_chain(PM_POST_SUSPEND);
	pm_restore_console();
}

static int run_suspend(void)
{
	int ret;
	char *argv[] = { "/system/etc/wmt/pm.sh", "", NULL };
	char *envp[] =
		{ "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", "ACTION=suspend", NULL };

	ret = call_usermodehelper(argv[0], argv, envp, 1);
	return ret;
}

static int run_resume(void)
{
	int ret;
	char *argv[] = { "/system/etc/wmt/pm.sh", "", NULL };
	char *envp[] =
		{ "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", "ACTION=resume", NULL };

	ret = call_usermodehelper(argv[0], argv, envp, 1);
	return ret;
}

/**
 * enter_state - Do common work needed to enter system sleep state.
 * @state: System sleep state to enter.
 *
 * Make sure that no one else is trying to put the system into a sleep state.
 * Fail if that's not the case.  Otherwise, prepare for system suspend, make the
 * system enter the given sleep state and clean up after wakeup.
 */
static int enter_state(suspend_state_t state)
{
	int error;

	if (!valid_state(state))
		return -ENODEV;

	if (!mutex_trylock(&pm_mutex))
		return -EBUSY;

	run_suspend();
	printk(KERN_INFO "PM: Syncing filesystems ... ");
	sys_sync();
	printk("done.\n");

	pr_debug("PM: Preparing system for %s sleep\n", pm_states[state]);
	error = suspend_prepare();
	if (error)
		goto Unlock;

	if (suspend_test(TEST_FREEZER))
		goto Finish;

	pr_debug("PM: Entering %s sleep\n", pm_states[state]);
	pm_restrict_gfp_mask();
	error = suspend_devices_and_enter(state);
	pm_restore_gfp_mask();

 Finish:
	pr_debug("PM: Finishing wakeup.\n");
	suspend_finish();
	run_resume();
 Unlock:
	mutex_unlock(&pm_mutex);
	return error;
}

static void pm_suspend_marker(char *annotation)
{
	struct timespec ts;
	struct rtc_time tm;

	getnstimeofday(&ts);
	rtc_time_to_tm(ts.tv_sec, &tm);
	printk(KERN_ERR"PM: suspend %s %d-%02d-%02d %02d:%02d:%02d.%09lu UTC\n",
		annotation, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec);
}

/**
 * pm_suspend - Externally visible function for suspending the system.
 * @state: System sleep state to enter.
 *
 * Check if the value of @state represents one of the supported states,
 * execute enter_state() and update system suspend statistics.
 */
int pm_suspend(suspend_state_t state)
{
	int error;

	if (state <= PM_SUSPEND_ON || state >= PM_SUSPEND_MAX)
		return -EINVAL;

	pm_suspend_marker("entry");
	error = enter_state(state);
	if (error) {
		suspend_stats.fail++;
		dpm_save_failed_errno(error);
	} else {
		suspend_stats.success++;
	}
	pm_suspend_marker("exit");
	return error;
}
EXPORT_SYMBOL(pm_suspend);
