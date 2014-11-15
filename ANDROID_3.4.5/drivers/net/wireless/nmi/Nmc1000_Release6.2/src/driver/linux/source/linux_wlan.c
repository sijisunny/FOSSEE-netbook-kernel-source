#ifndef SIMULATION
#include "NMI_WFI_CfgOperations.h"
#include "linux_wlan_common.h"
#include "nmi_wlan_if.h"
#include "nmi_wlan.h"
#ifdef USE_WIRELESS
#include "NMI_WFI_CfgOperations.h"
#endif

#include "linux_wlan_common.h"

#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#ifndef PLAT_ALLWINNER_A10	// tony
#include <asm/gpio.h>
#endif
#include <linux/kthread.h>
#include <linux/firmware.h>
#include <linux/delay.h>

#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>

#include <linux/version.h>
#include <linux/semaphore.h>

#include "svnrevision.h"

#ifdef NMI_SDIO
#include "linux_wlan_sdio.h"
#else
#include "linux_wlan_spi.h"
#endif


#if defined(PLAT_WMS8304)
#include "nmi_custom_gpio.c"
#elif defined(PLAT_WM8880)
#include "nmi_wm8880_gpio.c"

#include <linux/wireless.h> 	// simon, rachel
#include <mach/irqs.h>
#include <linux/mmc/sdio.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

int wmt_nmc1000_intr_num=15; //gpio 15
#endif

#define nmi_wlan_deinit(nic) 	if(&nic->oup != NULL)	\
								if(nic->oup.wlan_cleanup != NULL) \
								        	nic->oup.wlan_cleanup()

//[[ johnny : enable to selecte bin file on Makefile
#ifndef STA_FIRMWARE
#define STA_FIRMWARE	"wifi_firmware.bin"
#endif

#ifndef AP_FIRMWARE
#define AP_FIRMWARE		"wifi_firmware_ap.bin"
#endif
//]]

// [[ added by tony
typedef struct android_wifi_priv_cmd {
	char *buf;
	int used_len;
	int total_len;
} android_wifi_priv_cmd;
// ]] for wpa supplicant iw

#if defined(PLAT_AML8726_M3) //[[ johnny : because of dummy irq
static int _available_irq_ready = 0;
#endif

#define IRQ_WAIT	1
#define IRQ_NO_WAIT	0
	/*
		to sync between mac_close and module exit.
		don't initialize or de-initialize from init/deinitlocks
		to be initialized from module nmc_netdev_init and 
		deinitialized from mdoule_exit
	*/
static struct semaphore close_exit_sync;
unsigned int int_rcvdU;
unsigned int int_rcvdB;
unsigned int int_clrd;

static int wlan_deinit_locks(linux_wlan_t* nic);
static void wlan_deinitialize_threads(linux_wlan_t* nic);
static void linux_wlan_lock(void* vp);
static void linux_wlan_unlock(void* vp);


static void* internal_alloc(uint32_t size, uint32_t flag);
static void linux_wlan_tx_complete(void* priv, int status);
void frmw_to_linux(uint8_t *buff, uint32_t size);
static int  mac_init_fn(struct net_device *ndev);
int  mac_xmit(struct sk_buff *skb, struct net_device *dev);
static int  mac_open(struct net_device *ndev);
static int  mac_close(struct net_device *ndev);
static struct net_device_stats *mac_stats(struct net_device *dev);
static int  mac_ioctl(struct net_device *ndev, struct ifreq *req, int cmd);


struct tx_complete_data{
	int size;
	void* buff;
	struct sk_buff *skb;
};


	/*
	for now - in frmw_to_linux there should be private data to be passed to it 
	and this data should be pointer to net device
	*/
linux_wlan_t* g_linux_wlan = NULL;
nmi_wlan_oup_t* gpstrWlanOps;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)

static const struct net_device_ops nmc_netdev_ops = {
	.ndo_init = mac_init_fn,
	.ndo_open = mac_open,
	.ndo_stop = mac_close,
	.ndo_start_xmit = mac_xmit,
	.ndo_do_ioctl = mac_ioctl,
	.ndo_get_stats = mac_stats,
	
};

#define nmc_set_netdev_ops(ndev) do { (ndev)->netdev_ops = &nmc_netdev_ops; } while(0)

#else

static void nmc_set_netdev_ops(struct net_device *ndev)
{

	ndev->init = mac_init_fn;
	ndev->open = mac_open;
	ndev->stop = mac_close;
	ndev->hard_start_xmit = mac_xmit;
	ndev->do_ioctl = mac_ioctl;
	ndev->get_stats = mac_stats;
}

#endif
#ifdef DEBUG_MODE

extern volatile int timeNo;

#define DEGUG_BUFFER_LENGTH 1000
volatile int WatchDogdebuggerCounter=0;
char DebugBuffer[DEGUG_BUFFER_LENGTH+20]={0};
static char * ps8current=DebugBuffer;

 

void printk_later(const char * format, ...)
{
	va_list args;
	va_start (args, format);
       ps8current += vsprintf (ps8current,format, args);
	va_end (args);
       if((ps8current-DebugBuffer)>DEGUG_BUFFER_LENGTH)
       {
       	ps8current=DebugBuffer;
       }
		
}

 
void dump_logs()
{
		if(DebugBuffer[0])
			{
				DebugBuffer[DEGUG_BUFFER_LENGTH]=0;
				printk("early printed\n");	
				printk(ps8current+1);
				ps8current[1]=0;
				printk("latest printed\n");
				printk(DebugBuffer);
				DebugBuffer[0]=0;
				ps8current=DebugBuffer;
			}
}

void Reset_WatchDogdebugger()
{
	WatchDogdebuggerCounter=0;
}

static int DebuggingThreadTask(void* vp)
{
	while(1){
			while(!WatchDogdebuggerCounter)
			{
				printk("Debug Thread Running %d\n",timeNo);
				WatchDogdebuggerCounter=1;
				msleep(10000);
			}
			dump_logs();
			WatchDogdebuggerCounter=0;
		}
}


#endif
/*
 *	Interrupt initialization and handling functions
 */

void linux_wlan_enable_irq(void){

#if (RX_BH_TYPE != RX_BH_THREADED_IRQ)
#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
	PRINT_D(INT_DBG,"Enabling IRQ ...\n");

#if defined(PLAT_WMS8304)		// added by rachel
	//enable_irq(nic->dev_irq_num);
	/*clean corresponding int status bit, or it will generate int continuously for ever*/	
	gpio_clean_irq_status_any(&gpio_irq_ctrl);

	enable_irq(g_linux_wlan->dev_irq_num);
	
	/*we should reopen this gpio pin int in order to capture the future/comming interruption*/	
	enable_gpio_int_any(&gpio_irq_ctrl, INT_EN);
 #elif defined(PLAT_WM8880)

	wmt_gpio_ack_irq(wmt_nmc1000_intr_num);
	wmt_gpio_unmask_irq(wmt_nmc1000_intr_num);

 #else
	enable_irq(g_linux_wlan->dev_irq_num);
 #endif
	
#endif
#endif
}

void linux_wlan_disable_irq(int wait){
#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
	if(wait){
		PRINT_D(INT_DBG,"Disabling IRQ ...\n");
#if defined(PLAT_WM8880)
		wmt_gpio_mask_irq(wmt_nmc1000_intr_num);
#else
		disable_irq(g_linux_wlan->dev_irq_num);
#endif
	} else {
		PRINT_D(INT_DBG,"Disabling IRQ ...\n");
#if defined(PLAT_WM8880)
		wmt_gpio_mask_irq(wmt_nmc1000_intr_num);
#else
		disable_irq_nosync(g_linux_wlan->dev_irq_num);
#endif
	}
#endif
}
#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
static irqreturn_t isr_uh_routine(int irq, void* user_data){
	linux_wlan_t *nic = (linux_wlan_t*)user_data;

#if defined(PLAT_WMS8304)
	int pin_state;
	//printk("isr_uh_routine\n");
	/*if this gpio pin int is not enable or int status bit is not toggled, it is not our interruption*/
	if(!gpio_irq_isEnable_any(&gpio_irq_ctrl) || !gpio_irq_state_any(&gpio_irq_ctrl))
		return IRQ_NONE;

	/*be here, be sure that this int is myself, but we should disable int before handler this interruption*/	
	enable_gpio_int_any(&gpio_irq_ctrl, INT_DIS);

	/*get value on gpin int pin*/	
	pin_state = gpio_get_value_any(&gpio_irq_ctrl);

	if(pin_state == 0) {		
 		//printk("wlan int\n");
		//dont_deinit_irq = 0;
	}
	else 
	{
		//printk("this is fake interruption!!\n");
		enable_gpio_int_any(&gpio_irq_ctrl, INT_EN);
		return IRQ_HANDLED;
	}
#elif defined(PLAT_WM8880)

	if(!is_gpio_irqenable(wmt_nmc1000_intr_num) || !gpio_irqstatus(wmt_nmc1000_intr_num))
		return IRQ_NONE;	

	wmt_gpio_mask_irq(wmt_nmc1000_intr_num);
	
#endif

	int_rcvdU++;
	PRINT_D(INT_DBG,"Interrupt received UH\n");
#if (RX_BH_TYPE != RX_BH_THREADED_IRQ)
	linux_wlan_disable_irq(IRQ_NO_WAIT);
#endif

#if defined(PLAT_AML8726_M3)//[[ johnny : because of dummy irq
  if ( _available_irq_ready == 0 )
  {
    linux_wlan_enable_irq();
    PRINT_D(GENERIC_DBG,"johnny test : recevied dummy irq\n");
    return IRQ_HANDLED;
  }
#endif//]]

    /*While mac is closing cacncel the handling of any interrupts received*/
	if(nic->close)
	{
		PRINT_ER("Driver is CLOSING: Can't handle UH interrupt\n");
	#if (RX_BH_TYPE == RX_BH_THREADED_IRQ)
		return IRQ_HANDLED;
	#else
		return IRQ_NONE;
	#endif

	}
#if (RX_BH_TYPE == RX_BH_WORK_QUEUE)
	schedule_work(&nic->rx_work_queue);
	return IRQ_HANDLED;
#elif (RX_BH_TYPE == RX_BH_KTHREAD)
	linux_wlan_unlock(&nic->rx_sem);
	return IRQ_HANDLED;
#elif (RX_BH_TYPE == RX_BH_THREADED_IRQ)
	return IRQ_WAKE_THREAD;
#endif

}
#endif

#if (RX_BH_TYPE == RX_BH_WORK_QUEUE || RX_BH_TYPE == RX_BH_THREADED_IRQ)

#if (RX_BH_TYPE == RX_BH_THREADED_IRQ)
irqreturn_t isr_bh_routine(int irq, void *userdata){	
	linux_wlan_t* nic;
	nic = (linux_wlan_t*)userdata;
#else
static void isr_bh_routine(struct work_struct *work){
	linux_wlan_t* nic;
	nic = container_of(work,linux_wlan_t,rx_work_queue);
#endif	

	/*While mac is closing cacncel the handling of any interrupts received*/
	if(nic->close)
	{
		PRINT_ER("Driver is CLOSING: Can't handle BH interrupt\n");
	#if (RX_BH_TYPE == RX_BH_THREADED_IRQ)
		return IRQ_HANDLED;
	#else
		return;
	#endif



	}

	int_rcvdB++;
	PRINT_D(INT_DBG,"Interrupt received BH\n");
	if(nic->oup.wlan_handle_rx_isr != 0){
		nic->oup.wlan_handle_rx_isr();
	}else{
			PRINT_ER("wlan_handle_rx_isr() hasn't been initialized\n");
		}


#if (RX_BH_TYPE == RX_BH_THREADED_IRQ)
	return IRQ_HANDLED;
#endif
}
#elif (RX_BH_TYPE == RX_BH_KTHREAD)
static int isr_bh_routine(void *vp)
{
	linux_wlan_t* nic;
	
	nic = (linux_wlan_t*)vp;

	while(1) {
		linux_wlan_lock(&nic->rx_sem);		
		if (nic->close){

			while(!kthread_should_stop())
				schedule();

			break;
		}
		int_rcvdB++;
		PRINT_D(INT_DBG,"Interrupt received BH\n");
		if(nic->oup.wlan_handle_rx_isr != 0){
			nic->oup.wlan_handle_rx_isr();
		} else{
				PRINT_ER("wlan_handle_rx_isr() hasn't been initialized\n");
		}
	}

	return 0;
}
#endif


#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
static int init_irq(linux_wlan_t* p_nic){
	int ret = 0;
	linux_wlan_t *nic = p_nic;
	
#if defined(PLAT_AML8726_M3) //johnny add
	nic->dev_irq_num = INT_GPIO_4; // <== skyworth //'100' value for onda vi30 platform

	ret = request_irq(nic->dev_irq_num, isr_uh_routine, IORESOURCE_IRQ | IORESOURCE_IRQ_SHAREABLE,
		"NMC_IRQ", nic);

	if (ret < 0) {
		PRINT_ER("Failed to request IRQ for GPIO: %d\n",nic->dev_irq_num);
	}

#elif defined (PLAT_WMS8304) //simon
	//system("rmmod /system/modules/3.0.8-default/kpad.ko");
	irq_gpio_init();
	nic->dev_irq_num = IRQ_GPIO; //(5) gpio_irq_ctrl;
	printk("%s\n",__FUNCTION__);
	ret = request_irq(nic->dev_irq_num, isr_uh_routine, IRQF_SHARED,
		"NMC_IRQ", nic);

	if (ret < 0) {
		PRINT_ER("Failed to request IRQ for GPIO: %d\n",nic->dev_irq_num);
	}

	enable_gpio_int_any(&gpio_irq_ctrl,INT_EN);

#elif defined(PLAT_WM8880)

	printk("%s\n",__FUNCTION__);
	wmt_gpio_setpull(wmt_nmc1000_intr_num,WMT_GPIO_PULL_UP);
	wmt_gpio_mask_irq(wmt_nmc1000_intr_num);
	wmt_gpio_ack_irq(wmt_nmc1000_intr_num);


	nic->dev_irq_num = IRQ_GPIO;
	ret = request_irq(nic->dev_irq_num, isr_uh_routine, IRQF_SHARED,
		"NMC_IRQ", nic);

	if (ret < 0) {
		PRINT_ER("Failed to request IRQ for GPIO: %d\n",nic->dev_irq_num);
	}

	/*clear int status register before enable this int pin*/
	wmt_gpio_ack_irq(wmt_nmc1000_intr_num);
	/*enable this int pin*/
	wmt_gpio_unmask_irq(wmt_nmc1000_intr_num);

#else
		/*initialize GPIO and register IRQ num*/						
		/*GPIO request*/
		if ((gpio_request(GPIO_NUM, "NMC_INTR") == 0) &&
		    (gpio_direction_input(GPIO_NUM) == 0)) {
#if defined (NM73131_0_BOARD)
			nic->dev_irq_num = IRQ_NMC1000;
#else
			gpio_export(GPIO_NUM, 1);
			nic->dev_irq_num = OMAP_GPIO_IRQ(GPIO_NUM);
			irq_set_irq_type(nic->dev_irq_num, IRQ_TYPE_LEVEL_LOW);
#endif
		} else {
			ret = -1;
			PRINT_ER("could not obtain gpio for NMC_INTR\n");
		}


#if (RX_BH_TYPE == RX_BH_THREADED_IRQ)
		if( (ret != -1) && ( request_threaded_irq(nic->dev_irq_num, isr_uh_routine, isr_bh_routine,
				  				IRQF_TRIGGER_LOW | IRQF_ONESHOT, /*Without IRQF_ONESHOT the uh will remain kicked in and dont gave a chance to bh*/
				   				"NMC_IRQ", nic))<0){

#else
		/*Request IRQ*/
		if( (ret != -1) && (request_irq(nic->dev_irq_num, isr_uh_routine,
				IRQF_TRIGGER_LOW, "NMC_IRQ", nic) < 0)){

#endif
				PRINT_ER("Failed to request IRQ for GPIO: %d\n",GPIO_NUM);
				ret = -1;
		}else{			

				PRINT_D(INIT_DBG,"IRQ request succeeded IRQ-NUM= %d on GPIO: %d\n",
				nic->dev_irq_num,GPIO_NUM);			
		}
#endif
	
	return ret;
}
#endif

static void deinit_irq(linux_wlan_t* nic){
	#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
		/* Deintialize IRQ */
		if(&nic->dev_irq_num != 0){
		  free_irq(nic->dev_irq_num, nic);
#if defined(PLAT_AML8726_M3) //johnny

#elif defined (PLAT_WMS8304)
		  irq_gpio_deinit();

#elif defined(PLAT_WM8880)

#else
		  gpio_free(GPIO_NUM);
#endif
		}
	#endif
}


/*
	OS functions
*/
static void linux_wlan_msleep(uint32_t msc){
	if(msc <= 4000000)
	{
		NMI_Uint32 u32Temp = msc * 1000;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
		usleep_range(u32Temp, u32Temp);
#else
	/* This is delay not sleep !!!, has to be changed*/
		msleep(msc);
#endif
	}
	else
	{
    		msleep(msc);
	}		
}

static void linux_wlan_atomic_msleep(uint32_t msc){
	mdelay(msc);	
}
static void linux_wlan_dbg(uint8_t *buff){
	printk(buff);
#if defined (NMC_DEBUGFS)
	kmsgdump_write(buff);
#endif
}

static void* linux_wlan_malloc_atomic(uint32_t sz){
	char* pntr = NULL;
	pntr = (char*)kmalloc(sz,GFP_ATOMIC );	
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",sz,pntr);
 	return ((void*)pntr);

}
static void* linux_wlan_malloc(uint32_t sz){
	char* pntr = NULL;
	pntr = (char*)kmalloc(sz,GFP_KERNEL );
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",sz,pntr);
 	return ((void*)pntr);
}

void linux_wlan_free(void* vp){
	if(vp != NULL){
			PRINT_D(MEM_DBG,"Freeing %p\n",vp);
			kfree(vp);
		}
}


static void* internal_alloc(uint32_t size, uint32_t flag){
	char* pntr = NULL;
	pntr = (char*)kmalloc(size,flag);
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",size,pntr);
 	return ((void*)pntr);
}


static void linux_wlan_init_lock(char* lockName, void* plock,int count)
{
	sema_init((struct semaphore*)plock,count);
	PRINT_D(LOCK_DBG,"Initializing [%s][%p]\n",lockName,plock);

}

static void linux_wlan_deinit_lock(void* plock)
{
	//mutex_destroy((struct mutex*)plock);
}

static void linux_wlan_lock(void* vp)
{
	PRINT_D(LOCK_DBG,"Locking %p\n",vp);
	if(vp != NULL)
	{		
		down((struct semaphore*)vp);
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}

static int linux_wlan_lock_timeout(void* vp,NMI_Uint32 timeout)
{
	int error = -1;
	PRINT_D(LOCK_DBG,"Locking %p\n",vp);
	if(vp != NULL)
	{		
		error = down_timeout((struct semaphore*)vp, msecs_to_jiffies(timeout));
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
	return error;
}

static void linux_wlan_unlock(void* vp)
{
	PRINT_D(LOCK_DBG,"Unlocking %p\n",vp);
	if(vp != NULL)
	{
		up((struct semaphore*)vp);			
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}


static void linux_wlan_init_mutex(char* lockName, void* plock,int count)
{
	mutex_init((struct mutex*)plock);
	PRINT_D(LOCK_DBG,"Initializing mutex [%s][%p]\n",lockName,plock);

}

static void linux_wlan_deinit_mutex(void* plock)
{
	mutex_destroy((struct mutex*)plock);
}

static void linux_wlan_lock_mutex(void* vp)
{
	PRINT_D(LOCK_DBG,"Locking mutex %p\n",vp);
	if(vp != NULL)
	{
	/*
		if(mutex_is_locked((struct mutex*)vp))
		{
			//PRINT_ER("Mutex already locked - %p \n",vp);
		}
*/
		mutex_lock((struct mutex*)vp);		

	}else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}

static void linux_wlan_unlock_mutex(void* vp){
	PRINT_D(LOCK_DBG,"Unlocking mutex %p\n",vp);
	if(vp != NULL){

		if(mutex_is_locked((struct mutex*)vp)){
			mutex_unlock((struct mutex*)vp);			
			}else{
					//PRINT_ER("Mutex already unlocked  - %p\n",vp);
			}
			
		}else{
		PRINT_ER("Failed, mutex is NULL\n");
		}
}



static void linux_wlan_mac_indicate(int flag){
	/*I have to do it that way becuase there is no mean to encapsulate device pointer 
	  as a parameter
	*/
	linux_wlan_t *pd = g_linux_wlan;
	int status;
	
	if (flag == NMI_MAC_INDICATE_STATUS) {		
		pd->oup.wlan_cfg_get_value(WID_STATUS, (unsigned char*)&status, 4);
		if (pd->mac_status == NMI_MAC_STATUS_INIT) {
			pd->mac_status = status;
			linux_wlan_unlock(&pd->sync_event);
		} else {
			pd->mac_status = status;
		}

		if (pd->mac_status == NMI_MAC_STATUS_CONNECT) {	/* Connect */
#if 0
			/**
				get the mac and bssid address
			**/
			PRINT_D(RX_DBG,"Calling cfg_get to get MAC_ADDR\n");
			pd->oup.wlan_cfg_get(1, WID_MAC_ADDR, 0);
			PRINT_D(RX_DBG,"Calling cfg_get to get BSSID\n");			
			pd->oup.wlan_cfg_get(0, WID_BSSID, 1);

			/**
				get the value
			**/
			pd->oup.wlan_cfg_get_value(WID_MAC_ADDR, pd->eth_src_address, 6);
			pd->oup.wlan_cfg_get_value(WID_BSSID, pd->eth_dst_address, 6);

			PRINT_D(GENERIC_DBG,"Source Address = %s",pd->eth_src_address);
			PRINT_D(GENERIC_DBG,"Destiation Address = %s",pd->eth_dst_address);			

			/**
				launch ndis
			**/
#endif
		}

	} else if (flag == NMI_MAC_INDICATE_SCAN) {
		PRINT_D(GENERIC_DBG,"Scanning ...\n");

	}

}


static int linux_wlan_rxq_task(void* vp){

	linux_wlan_t *pd = (linux_wlan_t*)vp;
	/* inform nmc1000_wlan_init that RXQ task is started. */
	linux_wlan_unlock(&pd->rxq_thread_started);
	while(1) {
		linux_wlan_lock(&pd->rxq_event);
		//wait_for_completion(&pd->rxq_event);
		if (pd->close){
			/*Unlock the mutex in the mac_close function to indicate the exiting of the RX thread */
			linux_wlan_unlock(&pd->rxq_thread_started);

			while(!kthread_should_stop())
				schedule();

			PRINT_D(RX_DBG," RX thread stopped\n");
			break;		
		}
		PRINT_D(RX_DBG,"Calling wlan_handle_rx_que()\n");
		pd->oup.wlan_handle_rx_que();		
	}
	return 0;
}

#define USE_TX_BACKOFF_DELAY_IF_NO_BUFFERS

static int linux_wlan_txq_task(void* vp)
{
	int ret,txq_count;
	linux_wlan_t *pd = (linux_wlan_t*)vp;

#if defined USE_TX_BACKOFF_DELAY_IF_NO_BUFFERS
#define TX_BACKOFF_WEIGHT_INCR_STEP (1)
#define TX_BACKOFF_WEIGHT_DECR_STEP (1) 
#define TX_BACKOFF_WEIGHT_MAX (7)
#define TX_BACKOFF_WEIGHT_MIN (0)
#define TX_BACKOFF_WEIGHT_UNIT_MS (10)
	int backoff_weight = TX_BACKOFF_WEIGHT_MIN;
	signed long timeout;
#endif

	/* inform nmc1000_wlan_init that TXQ task is started. */
	linux_wlan_unlock(&pd->txq_thread_started);
	while(1) {
		
		PRINT_D(TX_DBG,"txq_task Taking a nap :)\n");
		linux_wlan_lock(&pd->txq_event);
		//wait_for_completion(&pd->txq_event);
		PRINT_D(TX_DBG,"txq_task Who waked me up :$\n");

		if (pd->close){
		    /*Unlock the mutex in the mac_close function to indicate the exiting of the TX thread */
			linux_wlan_unlock(&pd->txq_thread_started);

			while(!kthread_should_stop())
				schedule();

			PRINT_D(TX_DBG,"TX thread stopped\n");			
			break;		
		}
		PRINT_D(TX_DBG,"txq_task handle the sending packet and let me go to sleep.\n");
#if !defined USE_TX_BACKOFF_DELAY_IF_NO_BUFFERS
		pd->oup.wlan_handle_tx_que();		
#else
		do {
			ret = pd->oup.wlan_handle_tx_que(&txq_count);	
			if(txq_count < FLOW_CONTROL_LOWER_THRESHOLD && netif_queue_stopped(pd->nmc_netdev))
			{
				PRINT_D(TX_DBG,"Waking up queue\n");
				netif_wake_queue(pd->nmc_netdev);
			}

			if(ret == NMI_TX_ERR_NO_BUF) { /* failed to allocate buffers in chip. */
				timeout = msecs_to_jiffies(TX_BACKOFF_WEIGHT_UNIT_MS << backoff_weight);
				do {
					/* Back off from sending packets for some time. */
					/* schedule_timeout will allow RX task to run and free buffers.*/					
					//set_current_state(TASK_UNINTERRUPTIBLE);
					//timeout = schedule_timeout(timeout); 
					msleep(TX_BACKOFF_WEIGHT_UNIT_MS << backoff_weight);
				} while(/*timeout*/0);
				backoff_weight += TX_BACKOFF_WEIGHT_INCR_STEP;
				if(backoff_weight > TX_BACKOFF_WEIGHT_MAX) {
					backoff_weight = TX_BACKOFF_WEIGHT_MAX;
				}
			} else {
				if(backoff_weight > TX_BACKOFF_WEIGHT_MIN) {
					backoff_weight -= TX_BACKOFF_WEIGHT_DECR_STEP;
					if(backoff_weight < TX_BACKOFF_WEIGHT_MIN) {
						backoff_weight = TX_BACKOFF_WEIGHT_MIN;
					}
				}
			}
			/*TODO: drop packets after a certain time/number of retry count. */
		} while(ret == NMI_TX_ERR_NO_BUF&&!pd->close); /* retry sending packets if no more buffers in chip. */
#endif
	}
	return 0;
}

static void linux_wlan_rx_complete(void){
	PRINT_D(RX_DBG,"RX completed\n");
}
int linux_wlan_get_firmware(linux_wlan_t* p_nic){

	linux_wlan_t* nic = p_nic;
	int ret = 0;	
	const struct firmware* nmc_firmware;
	char *firmware;
	

	if(nic->iftype == AP_MODE)
		firmware = AP_FIRMWARE;
	else
		firmware = STA_FIRMWARE;

	
	if(nic == NULL){
		PRINT_ER("NIC is NULL\n");
		goto _fail_;
	}

	if(&nic->nmc_netdev->dev == NULL){ 
		PRINT_ER("&nic->nmc_netdev->dev  is NULL\n");
		goto _fail_;
	}
	
	
	/*	the firmare should be located in /lib/firmware in 
		root file system with the name specified above */

#ifdef NMI_SDIO
	if( request_firmware(&nmc_firmware,firmware, &nic->nmc_sdio_func->dev) != 0){
		PRINT_ER("%s - firmare not available\n",firmware);
		ret = -1;
		goto _fail_;
	}
#else
	if( request_firmware(&nmc_firmware,firmware, &nic->nmc_spidev->dev) != 0){
		PRINT_ER("%s - firmare not available\n",firmware);
		ret = -1;
		goto _fail_;
	}
#endif
	nic->nmc_firmware = nmc_firmware; /* Bug 4703 */

_fail_:
	
	return ret;	

}

static int linux_wlan_start_firmware(linux_wlan_t* nic){

	int ret = 0;
	/* start firmware */
	PRINT_D(INIT_DBG,"Starting Firmware ...\n");
	ret = nic->oup.wlan_start();
	if(ret < 0){
		PRINT_ER("Failed to start Firmware\n");
		goto _fail_;
	}
#if defined(PLAT_AML8726_M3)//[[ johnny : because of dummy irq
	_available_irq_ready = 1;
#endif

	/* wait for mac ready */
	PRINT_D(INIT_DBG,"Waiting for Firmware to get ready ...\n");
	linux_wlan_lock(&nic->sync_event);
	/*
		TODO: Driver shouoldn't wait forever for firmware to get started -
		in case of timeout this should be handled properly
	*/
	PRINT_D(INIT_DBG,"Firmware successfully started\n");

_fail_:
	return ret;
}
static int linux_wlan_firmware_download(linux_wlan_t* p_nic){

	int ret = 0;	
	linux_wlan_t* nic = p_nic;

	if(nic->nmc_firmware == NULL){
		PRINT_ER("Firmware buffer is NULL\n");
		ret = -ENOBUFS;
		goto _FAIL_;
	}
	/**
		do the firmware download
	**/
	PRINT_D(INIT_DBG,"Downloading Firmware ...\n");
	ret = nic->oup.wlan_firmware_download(nic->nmc_firmware->data, nic->nmc_firmware->size);
	if(ret < 0){
		goto _FAIL_;
	}

	/* Freeing FW buffer */
	PRINT_D(INIT_DBG,"Freeing FW buffer ...\n");
	PRINT_D(GENERIC_DBG,"Releasing firmware\n");
	release_firmware(nic->nmc_firmware);
	nic->nmc_firmware = NULL;

	PRINT_D(INIT_DBG,"Download Succeeded \n");	
	
_FAIL_:
	return ret;
}


/* startup configuration - could be changed later using iconfig*/
static int linux_wlan_init_test_config(linux_wlan_t* p_nic){

	linux_wlan_t* nic = p_nic;	
	unsigned char c_val[64];
	unsigned char mac_add[] = {0x00, 0x80, 0xC2, 0x5E, 0xa2, 0xb2};
	unsigned int chipid = 0;
	PRINT_D(TX_DBG,"Start configuring Firmware\n");

	get_random_bytes(&mac_add[5], 1);

	printk("MAC address is : %02x-%02x-%02x-%02x-%02x-%02x\n", mac_add[0],mac_add[1],mac_add[2],mac_add[3],mac_add[4],mac_add[5]);
	chipid = nmi_get_chipid(0);

	

	/*to tell fw that we are going to use PC test - NMI specific*/
	c_val[0] = 0;
	if (!nic->oup.wlan_cfg_set(1, WID_PC_TEST_MODE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = INFRASTRUCTURE;
	if (!nic->oup.wlan_cfg_set(0, WID_BSS_TYPE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = G_MIXED_11B_2_MODE;
	if (!nic->oup.wlan_cfg_set(0, WID_11G_OPERATING_MODE, c_val, 1, 0))
		goto _fail_;
	
	c_val[0] = RATE_AUTO; /* bug 4275: Enable autorate and limit it to 24Mbps */
	if (!nic->oup.wlan_cfg_set(0, WID_CURRENT_TX_RATE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 1;
	if (!nic->oup.wlan_cfg_set(0, WID_CURRENT_CHANNEL, c_val, 1, 0))
		goto _fail_;

	c_val[0] = G_SHORT_PREAMBLE;
	if (!nic->oup.wlan_cfg_set(0, WID_PREAMBLE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = AUTO_PROT;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_PROT_MECH, c_val, 1, 0))
		goto _fail_;

	c_val[0] = ACTIVE_SCAN;
	if (!nic->oup.wlan_cfg_set(0, WID_SCAN_TYPE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = SITE_SURVEY_OFF;
	if (!nic->oup.wlan_cfg_set(0, WID_SITE_SURVEY, c_val, 1, 0))
		goto _fail_;

	*((int *)c_val) = 0xffff; /* Never use RTS-CTS */
	if (!nic->oup.wlan_cfg_set(0, WID_RTS_THRESHOLD, c_val, 2, 0))
		goto _fail_;

	*((int *)c_val) = 2346;
	if (!nic->oup.wlan_cfg_set(0, WID_FRAG_THRESHOLD, c_val, 2, 0))
		goto _fail_;

    /*  SSID                                                                 */
    /*  --------------------------------------------------------------       */
    /*  Configuration :   String with length less than 32 bytes              */
    /*  Values to set :   Any string with length less than 32 bytes          */
    /*                    ( In BSS Station Set SSID to "" (null string)      */
    /*                      to enable Broadcast SSID suppport )              */
    /*  --------------------------------------------------------------       */
#ifndef USE_WIRELESS
	strcpy(c_val, "nwifi");
	if (!nic->oup.wlan_cfg_set(0, WID_SSID, c_val, (strlen(c_val)+1), 0))
		goto _fail_;
#endif

	c_val[0] = 0;
	if (!nic->oup.wlan_cfg_set(0, WID_BCAST_SSID, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 1;
	if (!nic->oup.wlan_cfg_set(0, WID_QOS_ENABLE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = NO_POWERSAVE;
	if (!nic->oup.wlan_cfg_set(0, WID_POWER_MANAGEMENT, c_val, 1, 0))
		goto _fail_;

	c_val[0] = NO_ENCRYPT; //NO_ENCRYPT, 0x79
	if (!nic->oup.wlan_cfg_set(0, WID_11I_MODE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = OPEN_SYSTEM;
	if (!nic->oup.wlan_cfg_set(0, WID_AUTH_TYPE, c_val, 1, 0))
		goto _fail_;

    /*  WEP/802 11I Configuration                                            */
    /*  ------------------------------------------------------------------   */
    /*  Configuration : WEP Key                                              */
    /*  Values (0x)   : 5 byte for WEP40 and 13 bytes for WEP104             */
    /*                  In case more than 5 bytes are passed on for WEP 40   */
    /*                  only first 5 bytes will be used as the key           */
    /*  ------------------------------------------------------------------   */

	strcpy(c_val, "123456790abcdef1234567890");
	if (!nic->oup.wlan_cfg_set(0, WID_WEP_KEY_VALUE, c_val, (strlen(c_val)+1), 0))
		goto _fail_;

    /*  WEP/802 11I Configuration                                            */
    /*  ------------------------------------------------------------------   */
    /*  Configuration : AES/TKIP WPA/RSNA Pre-Shared Key                     */
    /*  Values to set : Any string with length greater than equal to 8 bytes */
    /*                  and less than 64 bytes                               */
    /*  ------------------------------------------------------------------   */
	strcpy(c_val, "12345678");
	if (!nic->oup.wlan_cfg_set(0, WID_11I_PSK, c_val, (strlen(c_val)), 0))
		goto _fail_;

    /*  IEEE802.1X Key Configuration                                         */
    /*  ------------------------------------------------------------------   */
    /*  Configuration : Radius Server Access Secret Key                      */
    /*  Values to set : Any string with length greater than equal to 8 bytes */
    /*                  and less than 65 bytes                               */
    /*  ------------------------------------------------------------------   */
	strcpy(c_val, "password");
	if (!nic->oup.wlan_cfg_set(0, WID_1X_KEY, c_val, (strlen(c_val)+1), 0))
		goto _fail_;

    /*   IEEE802.1X Server Address Configuration                             */
    /*  ------------------------------------------------------------------   */
    /*  Configuration : Radius Server IP Address                             */
    /*  Values to set : Any valid IP Address                                 */
    /*  ------------------------------------------------------------------   */
	c_val[0] = 192;
	c_val[1] = 168;
	c_val[2] = 1;
	c_val[3] = 112;
 	if (!nic->oup.wlan_cfg_set(0, WID_1X_SERV_ADDR, c_val, 4, 0))
		goto _fail_;

	c_val[0] = 3;
	if (!nic->oup.wlan_cfg_set(0, WID_LISTEN_INTERVAL, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 3;
	if (!nic->oup.wlan_cfg_set(0, WID_DTIM_PERIOD, c_val, 1, 0))
		goto _fail_;

	c_val[0] = NORMAL_ACK;
	if (!nic->oup.wlan_cfg_set(0, WID_ACK_POLICY, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 0;
	if (!nic->oup.wlan_cfg_set(0, WID_USER_CONTROL_ON_TX_POWER, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 48;
	if (!nic->oup.wlan_cfg_set(0, WID_TX_POWER_LEVEL_11A, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 28;
	if (!nic->oup.wlan_cfg_set(0, WID_TX_POWER_LEVEL_11B, c_val, 1, 0))
		goto _fail_;

    /*  Beacon Interval                                                      */
    /*  -------------------------------------------------------------------- */
    /*  Configuration : Sets the beacon interval value                       */
    /*  Values to set : Any 16-bit value                                     */
    /*  -------------------------------------------------------------------- */

	*((int *)c_val) = 100;
	if (!nic->oup.wlan_cfg_set(0, WID_BEACON_INTERVAL, c_val, 2, 0))
		goto _fail_;

	c_val[0] = REKEY_DISABLE;
	if (!nic->oup.wlan_cfg_set(0, WID_REKEY_POLICY, c_val, 1, 0))
		goto _fail_;

    /*  Rekey Time (s) (Used only when the Rekey policy is 2 or 4)           */
    /*  -------------------------------------------------------------------- */
    /*  Configuration : Sets the Rekey Time (s)                              */
    /*  Values to set : 32-bit value                                         */
    /*  -------------------------------------------------------------------- */
	*((int *)c_val) = 84600;
	if (!nic->oup.wlan_cfg_set(0, WID_REKEY_PERIOD, c_val, 4, 0))
		goto _fail_;

    /*  Rekey Packet Count (in 1000s; used when Rekey Policy is 3)           */
    /*  -------------------------------------------------------------------- */
    /*  Configuration : Sets Rekey Group Packet count                        */
    /*  Values to set : 32-bit Value                                         */
    /*  -------------------------------------------------------------------- */
	*((int *)c_val) = 500;
	if (!nic->oup.wlan_cfg_set(0, WID_REKEY_PACKET_COUNT, c_val, 4, 0))
		goto _fail_;

	c_val[0] = 1;
	if (!nic->oup.wlan_cfg_set(0, WID_SHORT_SLOT_ALLOWED, c_val, 1, 0))
		goto _fail_;

	c_val[0] = G_SELF_CTS_PROT;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_ERP_PROT_TYPE, c_val, 1, 0))
		goto _fail_;

	if((chipid & 0xfff) > 0xd0) 
		c_val[0] = 1;  /* Enable N */
	else
		c_val[0] = 0;  /* Disable N */
		
	if (!nic->oup.wlan_cfg_set(0, WID_11N_ENABLE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = HT_MIXED_MODE;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_OPERATING_MODE, c_val, 1, 0))
		goto _fail_;	

	c_val[0] = 1; 	/* TXOP Prot disable in N mode: No RTS-CTS on TX A-MPDUs to save air-time. */
	if (!nic->oup.wlan_cfg_set(0, WID_11N_TXOP_PROT_DISABLE, c_val, 1, 0))
		goto _fail_;	

	memcpy(c_val, mac_add, 6);
	
	if (!nic->oup.wlan_cfg_set(0, WID_MAC_ADDR, c_val, 6, 0))
		goto _fail_;
	
	/**
		AP only
	**/
	c_val[0] = DETECT_PROTECT_REPORT;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_OBSS_NONHT_DETECTION, c_val, 1, 0))
		goto _fail_;

	c_val[0] = RTS_CTS_NONHT_PROT;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_HT_PROT_TYPE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 0;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_RIFS_PROT_ENABLE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = MIMO_MODE;
	if (!nic->oup.wlan_cfg_set(0, WID_11N_SMPS_MODE, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 6; /* Limit TX rate below MCS-7 for NMC1000F0. Should be changed in to 7 in NMC1002. */
	if (!nic->oup.wlan_cfg_set(0, WID_11N_CURRENT_TX_MCS, c_val, 1, 0))
		goto _fail_;

	c_val[0] = 1; /* Enable N with immediate block ack. */
	if (!nic->oup.wlan_cfg_set(0, WID_11N_IMMEDIATE_BA_ENABLED, c_val, 1, 0))
		goto _fail_;

	/*Adde by Amr - BugID 4707*/
	/*A mandatory restart mac has to be done initially.*/
	/*So wid reset is sent with startup configrations*/
	c_val[0] = 1;
	if (!nic->oup.wlan_cfg_set(0, WID_RESET, c_val, 1, 1))
		goto _fail_;

	return 0;

_fail_:
	return -1;
}


/**************************/
void nmc1000_wlan_deinit(linux_wlan_t *nic) {
  
	if(nic->nmc1000_initialized)
	{

		PRINT_D(INIT_DBG,"Deinitializing nmc1000  ...\n");
		
		if(nic == NULL){
			PRINT_ER("nic is NULL\n");
			return;
		}

		nmi_bus_set_default_speed();

		PRINT_D(INIT_DBG,"Disabling IRQ\n");
		#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
			linux_wlan_disable_irq(IRQ_WAIT);
		#else
			disable_sdio_interrupt();
		#endif
		

		/* not sure if the following unlocks are needed or not*/
		if(&nic->rxq_event != NULL){
			linux_wlan_unlock(&nic->rxq_event);
			}

		if(&nic->txq_event != NULL){
			linux_wlan_unlock(&nic->txq_event);
			}

		
	#if (RX_BH_TYPE == RX_BH_WORK_QUEUE)
		/*Removing the work struct from the linux kernel workqueue*/
		if(&nic->rx_work_queue != NULL)
			flush_work(&nic->rx_work_queue);
		
	#elif (RX_BH_TYPE == RX_BH_KTHREAD)
		//if(&nic->rx_sem != NULL)
			//linux_wlan_unlock(&nic->rx_sem);
	#endif

	PRINT_D(INIT_DBG,"Deinitializing Threads\n");
	wlan_deinitialize_threads(nic);
	
	PRINT_D(INIT_DBG,"Deinitializing IRQ\n");
	deinit_irq(nic);


	if(&nic->oup != NULL){
		if(nic->oup.wlan_stop != NULL)
			nic->oup.wlan_stop();		
	}

	PRINT_D(INIT_DBG,"Deinitializing NMI Wlan\n");
	nmi_wlan_deinit(nic);			
			
	/*De-Initialize locks*/
	PRINT_D(INIT_DBG,"Deinitializing Locks\n");
	wlan_deinit_locks(nic);

	/* announce that nmc1000 is not initialized */
	nic->nmc1000_initialized = 0;
	
	PRINT_D(INIT_DBG,"nmc1000 deinitialization Done\n");
		
	}else{
		PRINT_D(INIT_DBG,"nmc1000 is not initialized\n");
	}

	return;
}

int wlan_init_locks(linux_wlan_t* p_nic){

	linux_wlan_t* nic = p_nic;

	PRINT_D(INIT_DBG,"Initializing Locks ...\n");
	
	/*initialize mutexes*/
	linux_wlan_init_mutex("hif_lock/hif_cs",&nic->hif_cs,1);
	linux_wlan_init_mutex("rxq_lock/rxq_cs",&nic->rxq_cs,1);
	linux_wlan_init_mutex("txq_lock/txq_cs",&nic->txq_cs,1);
	
	linux_wlan_init_lock("txq_wait/txq_event",&nic->txq_event,0);
	linux_wlan_init_lock("rxq_wait/rxq_event",&nic->rxq_event,0);	

	linux_wlan_init_lock("cfg_wait/cfg_event",&nic->cfg_event,0);
	linux_wlan_init_lock("sync_event",&nic->sync_event,0);

	linux_wlan_init_lock("rxq_lock/rxq_started",&nic->rxq_thread_started,0);
	linux_wlan_init_lock("rxq_lock/txq_started",&nic->txq_thread_started,0);

	#if (RX_BH_TYPE == RX_BH_KTHREAD)
		linux_wlan_init_lock("BH_SEM", &nic->rx_sem, 0);
	#endif

	return 0;
}

static int wlan_deinit_locks(linux_wlan_t* nic){
	PRINT_D(INIT_DBG,"De-Initializing Locks\n");
	
	if(&nic->hif_cs != NULL)
		linux_wlan_deinit_mutex(&nic->hif_cs);

	if(&nic->rxq_cs != NULL)
		linux_wlan_deinit_mutex(&nic->rxq_cs);

	if(&nic->txq_cs != NULL)
		linux_wlan_deinit_mutex(&nic->txq_cs);
	
	if(&nic->rxq_event != NULL)
		linux_wlan_deinit_lock(&nic->rxq_event);	

	if(&nic->txq_event != NULL)
		linux_wlan_deinit_lock(&nic->txq_event);
	
	if(&nic->rxq_thread_started != NULL)
		linux_wlan_deinit_lock(&nic->rxq_thread_started);

	if(&nic->txq_thread_started != NULL)
		linux_wlan_deinit_lock(&nic->txq_thread_started);

	if(&nic->cfg_event != NULL)
		linux_wlan_deinit_lock(&nic->cfg_event);

	if(&nic->sync_event != NULL)
		linux_wlan_deinit_lock(&nic->sync_event);

	return 0;
}
void linux_to_wlan(nmi_wlan_inp_t* nwi,linux_wlan_t* nic){

	PRINT_D(INIT_DBG,"Linux to Wlan services ...\n");
	
	nwi->os_context.hif_critical_section = (void *)&nic->hif_cs;
	nwi->os_context.os_private = (void *)nic;
	nwi->os_context.tx_buffer_size = LINUX_TX_SIZE;
	nwi->os_context.txq_critical_section = (void *)&nic->txq_cs;
	nwi->os_context.txq_wait_event = (void *)&nic->txq_event;
	//nwi.os_context.rx_buffer_size = LINUX_RX_SIZE;
	nwi->os_context.rxq_critical_section = (void *)&nic->rxq_cs;
	nwi->os_context.rxq_wait_event = (void *)&nic->rxq_event;
	nwi->os_context.cfg_wait_event = (void *)&nic->cfg_event;

	nwi->os_func.os_sleep = linux_wlan_msleep;
	nwi->os_func.os_atomic_sleep = linux_wlan_atomic_msleep;
	nwi->os_func.os_debug = linux_wlan_dbg;
	nwi->os_func.os_malloc = linux_wlan_malloc;
	nwi->os_func.os_malloc_atomic = linux_wlan_malloc_atomic;
	nwi->os_func.os_free = linux_wlan_free;
	nwi->os_func.os_lock = linux_wlan_lock;
	nwi->os_func.os_unlock = linux_wlan_unlock;
	nwi->os_func.os_wait = linux_wlan_lock_timeout;
	nwi->os_func.os_signal = linux_wlan_unlock;
	nwi->os_func.os_enter_cs = linux_wlan_lock_mutex;
	nwi->os_func.os_leave_cs = linux_wlan_unlock_mutex;
#ifdef NMI_SDIO
	nwi->io_func.io_type = HIF_SDIO;
	nwi->io_func.io_init = linux_sdio_init;
	nwi->io_func.io_deinit = linux_sdio_deinit;
	nwi->io_func.u.sdio.sdio_cmd52 = linux_sdio_cmd52;
	nwi->io_func.u.sdio.sdio_cmd53 = linux_sdio_cmd53;
	nwi->io_func.u.sdio.sdio_set_max_speed = linux_sdio_set_max_speed;
	nwi->io_func.u.sdio.sdio_set_default_speed = linux_sdio_set_default_speed;
#else
	nwi->io_func.io_type = HIF_SPI;
	nwi->io_func.io_init = linux_spi_init;
	nwi->io_func.io_deinit = linux_spi_deinit;
	nwi->io_func.u.spi.spi_tx = linux_spi_write;
	nwi->io_func.u.spi.spi_rx = linux_spi_read;
	nwi->io_func.u.spi.spi_trx = linux_spi_write_read;
	nwi->io_func.u.spi.spi_max_speed = linux_spi_set_max_speed;
#endif
	
	/*for now - to be revised*/
	nwi->net_func.rx_indicate = frmw_to_linux;
	nwi->net_func.rx_complete = linux_wlan_rx_complete;
	nwi->indicate_func.mac_indicate = linux_wlan_mac_indicate;
}

int wlan_initialize_threads(linux_wlan_t* nic){

	int ret = 0;
	PRINT_D(INIT_DBG,"Initializing Threads ...\n");

#if (RX_BH_TYPE == RX_BH_WORK_QUEUE)
	/*Initialize rx work queue task*/
	INIT_WORK(&nic->rx_work_queue, isr_bh_routine);
#elif (RX_BH_TYPE == RX_BH_KTHREAD)
	PRINT_D(INIT_DBG,"Creating kthread for Rxq BH\n");
	nic->rx_bh_thread = kthread_run(isr_bh_routine,(void*)nic,"K_RXQ_BH");
	if(nic->rx_bh_thread == 0){
		PRINT_ER("couldn't create RX BH thread\n");
		ret = -ENOBUFS;
		goto _fail_;
	}
#endif

	/* create rx task */
	PRINT_D(INIT_DBG,"Creating kthread for reception\n");
	nic->rxq_thread = kthread_run(linux_wlan_rxq_task,(void*)nic,"K_RXQ_TASK");
	if(nic->rxq_thread == 0){
		PRINT_ER("couldn't create RXQ thread\n");
		ret = -ENOBUFS;
		goto _fail_1;
	}
	
	/* wait for RXQ task to start. */
	linux_wlan_lock(&nic->rxq_thread_started);
	
	/* create tx task */
	PRINT_D(INIT_DBG,"Creating kthread for transmission\n");	
	nic->txq_thread = kthread_run(linux_wlan_txq_task,(void*)nic,"K_TXQ_TASK");												
	if(nic->txq_thread == 0){
		PRINT_ER("couldn't create TXQ thread\n");
		ret = -ENOBUFS;
		goto _fail_2;
	}
#ifdef DEBUG_MODE
	PRINT_D(INIT_DBG,"Creating kthread for Debugging\n");	
	nic->txq_thread = kthread_run(DebuggingThreadTask,(void*)nic,"DebugThread");												
	if(nic->txq_thread == 0){
		PRINT_ER("couldn't create TXQ thread\n");
		ret = -ENOBUFS;
		goto _fail_2;
	}
#endif	
	/* wait for TXQ task to start. */
	linux_wlan_lock(&nic->txq_thread_started);
	
	return 0;
	
	_fail_2:
		/*De-Initialize 2nd thread*/
		nic->close = 1;
		linux_wlan_unlock(&nic->rxq_event);
		kthread_stop(nic->rxq_thread);

	_fail_1:
	#if(RX_BH_TYPE == RX_BH_KTHREAD)
		/*De-Initialize 1st thread*/
		nic->close = 1;
		linux_wlan_unlock(&nic->rx_sem);
		kthread_stop(nic->rx_bh_thread);
	_fail_:
	#endif
		nic->close = 0;
		return ret;	
}

static void wlan_deinitialize_threads(linux_wlan_t* nic){
	
	nic->close = 1;
	PRINT_D(INIT_DBG,"Deinitializing Threads\n");
	if(&nic->rxq_event != NULL)
		linux_wlan_unlock(&nic->rxq_event);

	
	if(nic->rxq_thread != NULL){
		kthread_stop(nic->rxq_thread);
		nic->rxq_thread = NULL;
		}

	
	if(&nic->txq_event != NULL)
		linux_wlan_unlock(&nic->txq_event);


	if(nic->txq_thread != NULL){
		kthread_stop(nic->txq_thread);
		nic->txq_thread = NULL;
		}
	
	#if(RX_BH_TYPE == RX_BH_KTHREAD)
		if(&nic->rx_sem != NULL)
			linux_wlan_unlock(&nic->rx_sem);
	
		if(nic->rx_bh_thread != NULL){
			kthread_stop(nic->rx_bh_thread);		
			nic->rx_bh_thread= NULL;
			}
	#endif
}

int nmc1000_wlan_init(linux_wlan_t* p_nic)
{
	nmi_wlan_inp_t nwi;
	nmi_wlan_oup_t nwo;
	linux_wlan_t* nic = p_nic;
	int ret = 0;
	
	if(!nic->nmc1000_initialized){
		nic->mac_status = NMI_MAC_STATUS_INIT;	
		nic->close = 0;
		nic->nmc1000_initialized = 0;
#if defined(PLAT_AML8726_M3)//[[ johnny : because of dummy irq
		_available_irq_ready = 0;
#endif
		wlan_init_locks(nic);
		
		linux_to_wlan(&nwi,nic);

		ret = nmi_wlan_init(&nwi, &nwo);
		if (ret < 0) {
			PRINT_ER("Initializing NMI_Wlan FAILED\n");
			goto _fail_locks_;
		}	
		memcpy(&nic->oup, &nwo, sizeof(nmi_wlan_oup_t));

		/*Save the oup structre into global pointer*/
		gpstrWlanOps = &nic->oup;


		ret = wlan_initialize_threads(nic);
		if (ret < 0) {
			PRINT_ER("Initializing Threads FAILED\n");
			goto _fail_irq_;
		}	

#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
		if(init_irq(nic)){
			PRINT_ER("couldn't initialize IRQ\n");
			ret = -EIO;
			goto _fail_locks_;
		}
#endif

#if (defined NMI_SDIO) && (!defined NMI_SDIO_IRQ_GPIO)
		enable_sdio_interrupt();
#endif

		if(linux_wlan_get_firmware(nic)){
			PRINT_ER("Can't get firmware \n");
			goto _fail_threads_;
		}

		/*Download firmware*/
		ret = linux_wlan_firmware_download(nic);
		if(ret < 0){
			PRINT_ER("Failed to download firmware\n");
			goto _fail_threads_;
		}

		/* Start firmware*/
		ret = linux_wlan_start_firmware(nic);
		if(ret < 0){
			PRINT_ER("Failed to start firmware\n");
			goto _fail_threads_;
		}

		nmi_bus_set_max_speed();

		/* Initialize firmware with default configuration */
		ret = linux_wlan_init_test_config(nic);

		if(ret < 0){
			PRINT_ER("Failed to configure firmware\n");
			goto _fail_threads_;
		}

		nic->nmc1000_initialized = 1;
		return 0; /*success*/

_fail_threads_:
	wlan_deinitialize_threads(nic);
_fail_irq_:
#if (!defined NMI_SDIO) || (defined NMI_SDIO_IRQ_GPIO)
		deinit_irq(nic);

#endif
_fail_locks_:
		wlan_deinit_locks(nic);
		PRINT_ER("WLAN Iinitialization FAILED\n");
	}else{
		PRINT_D(INIT_DBG,"nmc1000 already initialized\n");
	}
	return ret;
}


/*
	- this function will be called automatically by OS when module inserted.
*/

#if !defined (NM73131_0_BOARD)    
int mac_init_fn(struct net_device *ndev){

	/*Why we do this !!!*/    
    netif_start_queue(ndev); 	//ma
    netif_stop_queue(ndev);	//ma
	  
    return 0;
}
#else
int mac_init_fn(struct net_device *ndev){

	unsigned char mac_add[] = {0x00,0x50,0xc2,0x5e,0x10,0x00};
	/* TODO: get MAC address whenever the source is EPROM - hardcoded and copy it to ndev*/
	memcpy(ndev->dev_addr, mac_add, 6);
	
    if(!is_valid_ether_addr(ndev->dev_addr)){
        PRINT_ER("Error: Wrong MAC address\n");
        return -EINVAL;
    }
	
    return 0;
}
#endif

/* This fn is called, when this device is setup using ifconfig */
#if !defined (NM73131_0_BOARD)
int mac_open(struct net_device *ndev){    
	
	linux_wlan_t* nic;
	unsigned char mac_add[] = {0x00, 0x80, 0xC2, 0x5E, 0xa2, 0xb2};
	int status;
	int ret = 0;
	struct NMI_WFI_priv* priv;
	
	nic = netdev_priv(ndev);
	
	linux_wlan_lock(&close_exit_sync);
	
	/*initialize platform*/
	ret = nmc1000_wlan_init(nic);
	if(ret < 0)
	{
		PRINT_ER("Failed to initialize nmc1000\n");
		linux_wlan_unlock(&close_exit_sync);
		return  ret;
	}
	
#ifdef USE_WIRELESS
	ret = NMI_WFI_InitHostInt(ndev);
	if(ret < 0)
	{
		PRINT_ER("Failed to initialize host interface\n");
		nmc1000_wlan_deinit(nic);
		linux_wlan_unlock(&close_exit_sync);
		return  ret;
	}
#endif

	
	get_random_bytes(&mac_add[5], 1);

	
#ifdef USE_WIRELESS
	priv = wiphy_priv(nic->nmc_netdev->ieee80211_ptr->wiphy);
	status = host_int_get_MacAddress(priv->hNMIWFIDrv, mac_add);
#else
	host_int_set_MacAddress(NULL, mac_add);
	status = host_int_get_MacAddress(NULL, mac_add);
#endif
	/* TODO: get MAC address whenever the source is EPROM - hardcoded and copy it to ndev*/
	memcpy(ndev->dev_addr, mac_add, ETH_ALEN);

	if(!is_valid_ether_addr(ndev->dev_addr)){
	     PRINT_ER("Error: Wrong MAC address\n");
		linux_wlan_unlock(&close_exit_sync);
	     return -EINVAL;
	 }
    /* Start the network interface queue for this device */
	PRINT_D(INIT_DBG,"Starting netifQ\n");
    //netif_start_queue(ndev); ma
    //enable interrupts
    //enable_irq(g_linux_wlan->dev_irq_num);

    netif_wake_queue(ndev); 

	//linux_wlan_lock(&close_exit_sync);
	
    return 0;
}
#else
int mac_open(struct net_device *ndev)
{    

	linux_wlan_t* nic;	
	nic = netdev_priv(ndev);

	/*initialize platform*/
	if(nmc1000_wlan_init(nic)){
		PRINT_ER("Failed to initialize platform\n");
		return 1;
		}
    	/* Start the network interface queue for this device */
	PRINT_D(INIT_DBG,"Starting netifQ\n");
    netif_start_queue(ndev);
	linux_wlan_lock(&close_exit_sync);	
    return 0;
}
#endif

struct net_device_stats *mac_stats(struct net_device *dev)
{
         linux_wlan_t* nic= netdev_priv(dev);
	 

	 return &nic->netstats;      
}

static void linux_wlan_tx_complete(void* priv, int status){

	struct tx_complete_data* pv_data = (struct tx_complete_data*)priv;
	if(status == 1){
		PRINT_D(TX_DBG,"Packet sent successfully - Size = %d - Address = %p - SKB = %p\n",pv_data->size,pv_data->buff, pv_data->skb);
	} else {
		PRINT_D(TX_DBG,"Couldn't send packet - Size = %d - Address = %p - SKB = %p\n",pv_data->size,pv_data->buff, pv_data->skb);
	}
    /* Free the SK Buffer, its work is done */
    dev_kfree_skb(pv_data->skb);	
	linux_wlan_free(pv_data);
}

int mac_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	linux_wlan_t* nic;
	struct tx_complete_data* tx_data = NULL;
	int QueueCount;
	//char* saddr,*daddr,*pu8UdpBuffer;
	char *pu8UdpBuffer;
	struct iphdr *ih;
//	struct ethhdr *eth;
//	char* ethsaddr,*ethdaddr;
//	int i;
	//char* TestBuff;	

	PRINT_D(INT_DBG,"\n========\n IntUH: %d - IntBH: %d - IntCld: %d \n========\n",int_rcvdU,int_rcvdB,int_clrd);
	nic = netdev_priv(ndev);

	PRINT_D(TX_DBG,"Sending packet just received from TCP/IP\n");
   
	/* Stop the network interface queue */
  // 	netif_stop_queue(ndev);

    	if(skb->dev != ndev){
        	PRINT_ER("Packet not destined to this device\n");
        	return 0;
    	}

    /*	allocate memory for the frame - there is no need to allocate the fixed packet size 1496.
     *	just allocat memory with the length of the packet received - also don't put any constraint
     *	on the size of the packet to be received because this restriction is already handled inside firmware.
	 */
/*
	if(skb->len > 1496){
		PRINT_ER("Unable to handle this large packet - Size = %d\n",skb->len);
	}

*/
	
	tx_data = (struct tx_complete_data*)internal_alloc(sizeof(struct tx_complete_data),GFP_ATOMIC);
	if(tx_data == NULL){
		PRINT_ER("Failed to allocate memory for tx_data structure\n");
        dev_kfree_skb(skb);
        netif_wake_queue(ndev);
        return 0;		
	}
	
    tx_data->buff = skb->data;
	tx_data->size = skb->len;
	tx_data->skb  = skb;
	/*get source and dest ip addresses*/
	ih = (struct iphdr *)(skb->data+sizeof(struct ethhdr));
	//saddr = &ih->saddr;
	//daddr = &ih->daddr;
	//NMI_PRINTF("Source IP = %d:%d:%d:%d \n",saddr[0],saddr[1],
		//	saddr[2],saddr[3]);
//	NMI_PRINTF("Dest IP = %d:%d:%d:%d \n",daddr[0],daddr[1],
	//			daddr[2],daddr[3]);
	
	pu8UdpBuffer = (char*)ih + sizeof(struct iphdr);
	if(pu8UdpBuffer[1] == 68 && pu8UdpBuffer[3] == 67)
	{
		PRINT_D(TX_DBG,"DHCP Message transmitted\n");
	}
	PRINT_D(TX_DBG,"Sending packet - Size = %d - Address = %p - SKB = %p\n",tx_data->size,tx_data->buff,tx_data->skb);

	/*eth = (struct ethhdr *)(skb->data);

	ethsaddr = &eth->h_source;
	ethdaddr = &eth->h_dest;

	NMI_PRINTF("Source MAC = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n",ethsaddr[0],ethsaddr[1],
			ethsaddr[2],ethsaddr[3],ethsaddr[4],ethsaddr[5]);
	NMI_PRINTF("Dest MAC = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n",ethdaddr[0],ethdaddr[1],
			ethdaddr[2],ethdaddr[3],ethdaddr[4],ethdaddr[5]);*/
     
    //NMI_PRINTF("sending pkt \n");

   // NMI_PRINTF("skb->data[12] = %2x, skb->data[13] = %2x \n");
    /*TestBuff = (char*)(skb->data + sizeof(struct ethhdr));
    
    for(i = 0; i < 30; i++)
    {
      if(i < skb->len)
      {
      	NMI_PRINTF("TestBuff[%d] = %2x \n", i, TestBuff[i]);
      }
    }*/

    /* Send packet to MAC HW - for now the tx_complete function will be just status
     * indicator. still not sure if I need to suspend host transmission till the tx_complete
     * function called or not?
     * allocated buffer will be freed in tx_complete function.
     */
	PRINT_D(TX_DBG,"Adding tx packet to TX Queue\n");
	nic->netstats.tx_packets++;
	nic->netstats.tx_bytes+=tx_data->size;
	QueueCount = nic->oup.wlan_add_to_tx_que((void*)tx_data,
									tx_data->buff,
									tx_data->size,
									linux_wlan_tx_complete);


	if(QueueCount > FLOW_CONTROL_UPPER_THRESHOLD)
	{
	//	printk("Stopping queue\n");
		netif_stop_queue(ndev);

	}
		

	/* Wake up the network interface queue */
    //netif_wake_queue(ndev);
    
	
    return 0;
}


int mac_close(struct net_device *ndev)
{

	linux_wlan_t* nic;

	nic = netdev_priv(ndev);
  
	PRINT_D(GENERIC_DBG,"Mac close\n");
	
	/* Stop the network interface queue */
	netif_stop_queue(ndev);

#ifdef USE_WIRELESS
	NMI_WFI_DeInitHostInt(ndev);
#endif

	PRINT_D(GENERIC_DBG,"Deinitializing nmc1000\n");
	nmc1000_wlan_deinit(nic);

#ifdef USE_WIRELESS
#ifdef NMI_AP_EXTERNAL_MLME
 	NMI_WFI_deinit_mon_interface();
#endif
#endif
	linux_wlan_unlock(&close_exit_sync);	

	return 0;
}


int mac_ioctl(struct net_device *ndev, struct ifreq *req, int cmd){
	
	NMI_Uint8 *buff= NULL;
	NMI_Sint8 rssi;
	NMI_Uint32 size=0,length=0;
	linux_wlan_t* nic;
	struct NMI_WFI_priv* priv;
	NMI_Sint32 s32Error = NMI_SUCCESS;


	
	//struct iwreq *wrq = (struct iwreq *) req;	// tony moved to case SIOCSIWPRIV
	#ifdef USE_WIRELESS
	nic = netdev_priv(ndev);

	if(!nic->nmc1000_initialized)
		return 0;

	#endif

	switch(cmd){
	// [[ added by tony for SIOCDEVPRIVATE
	case SIOCDEVPRIVATE+1:
		{
			android_wifi_priv_cmd priv_cmd;

			PRINT_D(GENERIC_DBG, "in SIOCDEVPRIVATE+1\n");

			if (copy_from_user(&priv_cmd, req->ifr_data, sizeof(android_wifi_priv_cmd))) {
				s32Error = -EFAULT;
				goto done;
			}
			
			buff = kmalloc(priv_cmd.total_len, GFP_KERNEL);
			if (!buff)
			{
				s32Error = -ENOMEM;
				goto done;
			}

			if (copy_from_user(buff, priv_cmd.buf, priv_cmd.total_len)) {
				s32Error = -EFAULT;
				goto done;
			}

			PRINT_D(GENERIC_DBG, "%s: Android private cmd \"%s\" on %s\n", __FUNCTION__, buff, req->ifr_name);

			if (strnicmp(buff, "SCAN-ACTIVE", strlen("SCAN-ACTIVE")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, SCAN-ACTIVE command\n", __FUNCTION__);
			}else if (strnicmp(buff, "SCAN-PASSIVE", strlen("SCAN-PASSIVE")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, SCAN-PASSIVE command\n", __FUNCTION__);
			}else if (strnicmp(buff, "RXFILTER-START", strlen("RXFILTER-START")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, RXFILTER-START command\n", __FUNCTION__);
			}else if (strnicmp(buff, "RXFILTER-STOP", strlen("RXFILTER-STOP")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, RXFILTER-STOP command\n", __FUNCTION__);
			}else if (strnicmp(buff, "RXFILTER-ADD", strlen("RXFILTER-ADD")) == 0) {
				int filter_num = *(buff + strlen("RXFILTER-ADD") + 1) - '0';
				PRINT_D(GENERIC_DBG, "%s, RXFILTER-ADD command, filter_num=%d\n", __FUNCTION__, filter_num);
			}else if (strnicmp(buff, "RXFILTER-REMOVE", strlen("RXFILTER-REMOVE")) == 0) {
				int filter_num = *(buff + strlen("RXFILTER-REMOVE") + 1) - '0';
				PRINT_D(GENERIC_DBG, "%s, RXFILTER-REMOVE command, filter_num=%d\n", __FUNCTION__, filter_num);
			}else if (strnicmp(buff, "BTCOEXSCAN-START", strlen("BTCOEXSCAN-START")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, BTCOEXSCAN-START command\n", __FUNCTION__);
			}else if (strnicmp(buff, "BTCOEXSCAN-STOP", strlen("BTCOEXSCAN-STOP")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, BTCOEXSCAN-STOP command\n", __FUNCTION__);
			}else if (strnicmp(buff, "BTCOEXMODE", strlen("BTCOEXMODE")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, BTCOEXMODE command\n", __FUNCTION__);
			}else if (strnicmp(buff, "SETBAND", strlen("SETBAND")) == 0) {
				uint band = *(buff + strlen("SETBAND") + 1) - '0';
				PRINT_D(GENERIC_DBG, "%s, SETBAND command, band=%d\n", __FUNCTION__, band);
			}else if (strnicmp(buff, "GETBAND", strlen("GETBAND")) == 0) {
				PRINT_D(GENERIC_DBG, "%s, GETBAND command\n", __FUNCTION__);
			}else if (strnicmp(buff, "COUNTRY", strlen("COUNTRY")) == 0) {
				char *country_code = buff + strlen("COUNTRY") + 1;
				PRINT_D(GENERIC_DBG, "%s, COUNTRY command, country_code=%s\n", __FUNCTION__, country_code);
			}else {
				PRINT_D(GENERIC_DBG, "%s, Unknown command\n", __FUNCTION__);
			}
		}break;
	// ]] 2013-06-24
	case SIOCSIWPRIV:
		{
			struct iwreq *wrq = (struct iwreq *) req;	// added by tony

			size = wrq->u.data.length;

			if (size&& wrq->u.data.pointer) 
			{
				buff = kmalloc(size, GFP_KERNEL);
				if (!buff)
				{
					s32Error = -ENOMEM;
					goto done;
				}

				if (copy_from_user
					(buff, wrq->u.data.pointer,
					wrq->u.data.length)) 
					{
						s32Error = -EFAULT;
						goto done;
					}

				if(strnicmp(buff,"RSSI",length) == 0) 
				{

					#ifdef USE_WIRELESS
					priv = wiphy_priv(nic->nmc_netdev->ieee80211_ptr->wiphy);
					s32Error = host_int_get_rssi(priv->hNMIWFIDrv, &(rssi));
					if(s32Error)
						PRINT_ER("Failed to send get rssi param's message queue ");
					#endif
					PRINT_INFO(GENERIC_DBG,"RSSI :%d\n",rssi);

					/*Rounding up the rssi negative value*/		
					rssi+=5;

					snprintf(buff, size, "rssi %d", rssi);
					
					if (copy_to_user(wrq->u.data.pointer, buff, size)) {
						PRINT_ER("%s: failed to copy data to user buffer\n", __FUNCTION__);
						s32Error = -EFAULT;
						goto done;
					}
				}
			}
		}
		break;
	default:
		{
	    		PRINT_INFO(GENERIC_DBG,"Command - %d - has been received\n",cmd);
			s32Error = -EOPNOTSUPP;
			goto done;
		}
	}
	
done:
	
	if(buff != NULL)
	{
		kfree(buff);
	}
	
	return s32Error;
}

void frmw_to_linux(uint8_t *buff, uint32_t size){

    unsigned int frame_len = 0;	
	int stats;
	unsigned char* buff_to_send = NULL;	    
    struct sk_buff *skb;
    //char *saddr,*daddr;
	char*pu8UdpBuffer;
    struct iphdr *ih;
//    static int once = 1;
//    int i;
    linux_wlan_t *nic;

		if(size > 0){

			frame_len = size;
			buff_to_send = buff;
			
			
			/* Need to send the packet up to the host, allocate a skb buffer */
		    	skb = dev_alloc_skb(frame_len);
		    	if(skb == NULL){
		       	 	PRINT_ER("Low memory - packet droped\n");
		        	return;
			    }

			skb_reserve(skb, (unsigned int)skb->data & 0x3);

			if(g_linux_wlan == NULL || g_linux_wlan->nmc_netdev == NULL){
			    PRINT_ER("nmc_netdev in g_linux_wlan is NULL");
			}
			skb->dev = g_linux_wlan->nmc_netdev;
	
			if(skb->dev == NULL){
				PRINT_ER("skb->dev is NULL\n");
			}
			
			/*
			for(i=0;i<40;i++)
			{
				if(i<frame_len)
					NMI_PRINTF("buff_to_send[%d]=%2x\n",i,buff_to_send[i]);

			}*/

			//skb_put(skb, frame_len);
			memcpy(skb_put(skb, frame_len),buff_to_send, frame_len);

			//NMI_PRINTF("After MEM_CPY\n");

			nic = netdev_priv(g_linux_wlan->nmc_netdev);
#ifdef USE_WIRELESS
		/*	if(nic->monitor_flag)
			{
				NMI_WFI_monitor_rx(g_linux_wlan->nmc_netdev,skb);
				return;
			}*/
#endif
		    skb->protocol = eth_type_trans(skb, g_linux_wlan->nmc_netdev);
		    /*get source and dest ip addresses*/
			ih = (struct iphdr *)(skb->data+sizeof(struct ethhdr));
			//saddr = &ih->saddr;
			//daddr = &ih->daddr;
			//NMI_PRINTF("[REC]Source IP = %.2x:%.2x:%.2x:%.2x",saddr[0],saddr[1],
					//saddr[2],saddr[3]);
			//NMI_PRINTF("[REC]Dest IP = %.2x:%.2x:%.2x:%.2x",daddr[0],daddr[1],
				//		daddr[2],daddr[3]);
			//NMI_PRINTF("Protocol = %4x\n",skb->protocol);
			pu8UdpBuffer = (char*)ih + sizeof(struct iphdr);
			//printk("Port No = %d,%d\n",pu8UdpBuffer[1],pu8UdpBuffer[3]);
			if(buff_to_send[35] == 67 && buff_to_send[37] == 68)
			{
				PRINT_D(RX_DBG,"DHCP Message received\n");
			}
			
		    /* Send the packet to the stack by giving it to the bridge */
			nic->netstats.rx_packets++;
			nic->netstats.rx_bytes+=frame_len;
			stats = netif_rx(skb);
			//printk("netif_rx ret value is: %d\n",stats);
		    PRINT_D(RX_DBG,"netif_rx ret value is: %d\n",stats);
		}else{
				PRINT_ER("Discard sending packet with len = %d\n",size);
			}
}

int nmc_netdev_init(void){

	linux_wlan_t* nic;
	struct net_device* ndev;

	linux_wlan_init_lock("close_exit_sync",&close_exit_sync,1);
	
	/*allocate ethernet device with linux_wlan_t as its private data*/
	if(! (ndev = alloc_etherdev(sizeof(linux_wlan_t)))){
			PRINT_ER("Failed to allocate ethernet dev\n");
			return -1;
		}

	nic = netdev_priv(ndev);
	memset(nic,sizeof(linux_wlan_t),0);
	
	nic->nmc_netdev = ndev;


	/*Reset interrupt count debug*/
	int_rcvdU= 0;
	int_rcvdB= 0;
	int_clrd = 0;
	
	g_linux_wlan = nic;
	
	ether_setup(ndev);

#if defined(NM73131)	// tony, 2012-09-20
	strcpy(ndev->name,"nmc_eth%d");
#else //PANDA_BOARD, PLAT_ALLWINNER_A10, PLAT_ALLWINNER_A20, PLAT_AML8726_M3 or PLAT_WMS8304
	strcpy(ndev->name,"wlan%d");	
#endif
	nmc_set_netdev_ops(ndev);	
#ifdef USE_WIRELESS
{
	struct wireless_dev *wdev;
	/*Register WiFi*/
	wdev = NMI_WFI_WiphyRegister(ndev);
	if(wdev == NULL){
		PRINT_ER("Can't register NMI Wiphy\n");
		return -1;
		}
	
	/*Pointing the priv structure the netdev*/

	/*linking the wireless_dev structure with the netdevice*/
	nic->nmc_netdev->ieee80211_ptr = wdev;
	nic->nmc_netdev->ml_priv = nic;
	wdev->netdev = nic->nmc_netdev;
	nic->netstats.rx_packets=0;
	nic->netstats.tx_packets=0;
	nic->netstats.rx_bytes=0;
	nic->netstats.tx_bytes=0;

}
#endif

	
    if(register_netdev(ndev)){
        PRINT_ER("Device couldn't be registered - %s\n", ndev->name);
        return -1; /* ERROR */
    }



#ifndef NMI_SDIO
	if(!linux_spi_init(&nic->nmc_spidev)){
		PRINT_ER("Can't initialize SPI \n");
        return -1; /* ERROR */
	}
	nic->nmc_spidev = nmc_spi_dev;
#else
	nic->nmc_sdio_func= local_sdio_func;
#endif

	nic->iftype = STATION_MODE;


	return 0;
}

#if defined(PLAT_AML8726_M3)
 #include <mach/gpio.h>
 #include <mach/gpio_data.h>
 #include <mach/pinmux.h>
 extern void sdio_reinit(void);
 extern void extern_wifi_set_enable(int is_on);

#elif defined(PLAT_ALLWINNER_A10)
 extern void sunximmc_rescan_card(unsigned id, unsigned insert);
 extern void mmc_pm_power(int mode, int* updown);
 extern int mmc_pm_get_io_val(char* name);
 extern int mmc_pm_gpio_ctrl(char* name, int level);
 #define NMC1000_SDIO_CARD_ID	3
 int nmc1000_power_val = 0;
 
#elif defined(PLAT_ALLWINNER_A20)
 extern void sw_mci_rescan_card(unsigned id, unsigned insert);
 extern void wifi_pm_power(int on);	// tony to keep allwinner's rule
#define NMC1000_SDIO_CARD_ID	3

#elif defined(PLAT_WM8880)
 /*simulate virtual sdio card insert and removal*/
 extern void force_remove_sdio2(void);
 extern void wmt_detect_sdio2(void);
 
#endif

/*The 1st function called after module inserted*/
static int __init init_nmc_driver(void){

	
#if defined (NMC_DEBUGFS)
	if(nmc_debugfs_init() < 0) {
		NMI_PRINTF("fail to create debugfs for nmc driver\n");
		return -1;
	}
#endif

	printk("IN INIT FUNCTION\n");
	printk("*** NMC1000 driver VERSION=[%s] REVISON=[%s] FW_VER=[%s] ***\n", NMC1000_DRIVER_VERSION, SVNREV, STA_FIRMWARE);

#if defined(PLAT_AML8726_M3)
	extern_wifi_set_enable(1);
	sdio_reinit();

#elif defined(PLAT_ALLWINNER_A10)
	nmc1000_power_val = 1;
	mmc_pm_power(0, &nmc1000_power_val);	// POWER UP
	sunximmc_rescan_card(NMC1000_SDIO_CARD_ID, 1);

#elif defined(PLAT_ALLWINNER_A20)

	wifi_pm_power(1);
	sw_mci_rescan_card(NMC1000_SDIO_CARD_ID, 1);

#elif defined(PLAT_WMS8304)		// added by rachel
	NmiWifiCardPower(0); 
	msleep(100);
	NmiWifiCardPower(1); 

#elif defined(PLAT_WM8880)
	NmiWifiCardPower(1); 
	wmt_detect_sdio2();
#endif

#ifdef NMI_SDIO
	{
		int ret;

		ret = sdio_register_driver(&nmc_bus);
		if (ret < 0) {
			printk("init_nmc_driver: Failed register sdio driver\n");
		}
	
	return ret;
	}
#else	
	PRINT_D(INIT_DBG,"Initializing netdev\n");
	if(nmc_netdev_init()){
		PRINT_ER("Couldn't initialize netdev\n");
	}

	PRINT_D(INIT_DBG,"Device has been initialized successfully\n");
    return 0;
#endif
}
module_init(init_nmc_driver);

static void __exit exit_nmc_driver(void){

	#define CLOSE_TIMEOUT 12*1000

	if((g_linux_wlan != NULL) &&g_linux_wlan->nmc_firmware != NULL) {
		release_firmware(g_linux_wlan->nmc_firmware);
		g_linux_wlan->nmc_firmware = NULL;
	}
	
	if( (g_linux_wlan != NULL)  && ((g_linux_wlan->nmc_netdev) != NULL) )
	{
		PRINT_D(INIT_DBG,"Waiting for mac_close ....\n");

		if(linux_wlan_lock_timeout(&close_exit_sync, CLOSE_TIMEOUT) < 0)
			PRINT_D(INIT_DBG,"Closed TimedOUT\n");
		else
			PRINT_D(INIT_DBG,"mac_closed\n");
	
		PRINT_D(INIT_DBG,"Unregistering netdev...\n");
    		unregister_netdev(g_linux_wlan->nmc_netdev);


		#ifdef USE_WIRELESS
			PRINT_D(INIT_DBG,"Freeing Wiphy...\n");
    			NMI_WFI_WiphyFree(g_linux_wlan->nmc_netdev);
		#endif

		PRINT_D(INIT_DBG,"Freeing netdev...\n");
		free_netdev(g_linux_wlan->nmc_netdev);
		
	
	}

#ifdef USE_WIRELESS
#ifdef NMI_AP_EXTERNAL_MLME
	// Bug 4600 : NMI_WFI_deinit_mon_interface was already called at mac_close
	//NMI_WFI_deinit_mon_interface();
#endif
#endif
	if((&nmc_bus) != NULL)
	{
	#ifndef NMI_SDIO
		PRINT_D(INIT_DBG,"SPI unregsiter...\n");	
		spi_unregister_driver(&nmc_bus);
	#else
		PRINT_D(INIT_DBG,"SDIO unregsiter...\n");	
		sdio_unregister_driver(&nmc_bus);	
	#endif
	}

	linux_wlan_deinit_lock(&close_exit_sync);
	g_linux_wlan = NULL;
	PRINT_D(INIT_DBG,"Module_exit Done.\n");	
#if defined (NMC_DEBUGFS)
	nmc_debugfs_remove();
#endif

#if defined(PLAT_AML8726_M3)
	extern_wifi_set_enable(0);

#elif defined(PLAT_ALLWINNER_A10)
	nmc1000_power_val = 0;
	mmc_pm_power(0, &nmc1000_power_val);	// POWER DOWN
	sunximmc_rescan_card(NMC1000_SDIO_CARD_ID, 0);

#elif defined(PLAT_ALLWINNER_A20)
	wifi_pm_power(0);
	sw_mci_rescan_card(NMC1000_SDIO_CARD_ID, 0);

#elif defined(PLAT_WMS8304)		// added by rachel
	NmiWifiCardPower(0);

#elif defined(PLAT_WM8880)
	force_remove_sdio2();
	NmiWifiCardPower(0); // simon
#endif
}
module_exit(exit_nmc_driver);

MODULE_LICENSE("GPL");
#endif
