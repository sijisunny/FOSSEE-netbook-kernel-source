
#include "../cmos-subdev.h"
#include "../../wmt-vid.h"
#include "siv120d.h"

#define sensor_write_array(sd, arr, sz) \
	cmos_init_8bit_addr(arr, sz, (sd)->i2c_addr)

#define sensor_read(sd, reg) \
	wmt_vid_i2c_read(sd->i2c_addr, reg)

#define sensor_write(sd, reg, val) \
	wmt_vid_i2c_write(sd->i2c_addr, reg, val)

struct cmos_win_size {
	char		*name;
	int		width;
	int		height;
	uint8_t 	*regs;
	size_t		size;
};

#define CMOS_WIN_SIZE(n, w, h, r) \
	{.name = n, .width = w , .height = h, .regs = r, .size = ARRAY_SIZE(r) }

static const struct cmos_win_size cmos_supported_win_sizes[] = {
	CMOS_WIN_SIZE("QVGA", 320, 240, siv120d_320_240_regs),
	CMOS_WIN_SIZE("VGA",  640, 480, siv120d_640_480_regs),
};

static const struct cmos_win_size *cmos_select_win(u32 *width, u32 *height)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cmos_supported_win_sizes); i++) {
		if (cmos_supported_win_sizes[i].width == *width &&
		    cmos_supported_win_sizes[i].height == *height) {
			*width = cmos_supported_win_sizes[i].width;
			*height = cmos_supported_win_sizes[i].height;
			return &cmos_supported_win_sizes[i];
		}
	}
	return NULL;
}

static int sensor_s_wb(struct cmos_subdev *sd, enum v4l2_wb_mode value)
{
	uint8_t *regs;
	size_t size;

	switch (value) {
	case WHITE_BALANCE_AUTO:
		regs = siv120d_wb_auto;
		size = ARRAY_SIZE(siv120d_wb_auto);
		break;
	case WHITE_BALANCE_INCANDESCENCE:
		regs = siv120d_wb_incandescent;
		size = ARRAY_SIZE(siv120d_wb_incandescent);
		break;
	case WHITE_BALANCE_DAYLIGHT:
		regs = siv120d_wb_daylight;
		size = ARRAY_SIZE(siv120d_wb_daylight);
		break;
	case WHITE_BALANCE_CLOUDY:
		regs = siv120d_wb_cloudy;
		size = ARRAY_SIZE(siv120d_wb_cloudy);
		break;
	case WHITE_BALANCE_FLUORESCENT:
		regs = siv120d_wb_fluorescent;
		size = ARRAY_SIZE(siv120d_wb_fluorescent);
		break;
	default:
		return -EINVAL;
	}

	sensor_write_array(sd, regs, size);
	return 0;
}

static int sensor_s_scenemode(struct cmos_subdev *sd, enum v4l2_scene_mode value)
{
	uint8_t *regs;
	size_t size;

	switch (value) {
	case SCENE_MODE_AUTO:
		regs = siv120d_scene_mode_auto;
		size = ARRAY_SIZE(siv120d_scene_mode_auto);
		break;
	case SCENE_MODE_NIGHTSHOT:
		regs = siv120d_scene_mode_night;
		size = ARRAY_SIZE(siv120d_scene_mode_night);
		break;
	default:
		return -EINVAL;
	}

	sensor_write_array(sd, regs, size);
	return 0;
}

static int sensor_s_exposure(struct cmos_subdev *sd, enum v4l2_exposure_mode value)
{
	uint8_t *regs;
	size_t size;

	switch (value) {
	case -2:
		regs = siv120d_exposure_neg6;
		size = ARRAY_SIZE(siv120d_exposure_neg6);
		break;
	case -1:
		regs = siv120d_exposure_neg3;
		size = ARRAY_SIZE(siv120d_exposure_neg3);
		break;
	case 0:
		regs = siv120d_exposure_zero;
		size = ARRAY_SIZE(siv120d_exposure_zero);
		break;
	case 1:
		regs = siv120d_exposure_pos3;
		size = ARRAY_SIZE(siv120d_exposure_pos3);
		break;
	case 2:
		regs = siv120d_exposure_pos6;
		size = ARRAY_SIZE(siv120d_exposure_pos6);
		break;
	default:
		return -EINVAL;
	}

	sensor_write_array(sd, regs, size);
	return 0;
}

static int sensor_s_hflip(struct cmos_subdev *sd, int value)
{
	uint8_t data;

	sensor_write(sd, 0x00, 0x00);

	data = sensor_read(sd, 0x04);

	switch (value) {
	case 0:
		data &= ~0x01;
		break;
	case 1:
		data |= 0x01;
		break;
	default:
		return -EINVAL;
	}

	return sensor_write(sd, 0x04, data);
}

static int sensor_s_vflip(struct cmos_subdev *sd, int value)
{
	uint8_t data;

	sensor_write(sd, 0x00, 0x00);

	data = sensor_read(sd, 0x04);

	switch (value) {
	case 0:
		data &= ~0x02;
		break;
	case 1:
		data |= 0x02;
		break;
	default:
		return -EINVAL;
	}

	return sensor_write(sd, 0x04, data);
}

static int sensor_queryctrl(struct cmos_subdev *sd, struct v4l2_queryctrl *qc)
{
	switch (qc->id) {
	case V4L2_CID_VFLIP:
	case V4L2_CID_HFLIP:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
	case V4L2_CID_CAMERA_SCENE_MODE:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
	case V4L2_CID_DO_WHITE_BALANCE:
		return v4l2_ctrl_query_fill(qc, 0, 3, 1, 0);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, -2, 2, 1, 0);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct cmos_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_CAMERA_SCENE_MODE:
		return sensor_s_scenemode(sd, ctrl->value);
	case V4L2_CID_DO_WHITE_BALANCE:	
		return sensor_s_wb(sd, ctrl->value);	
	case V4L2_CID_EXPOSURE:
		return sensor_s_exposure(sd, ctrl->value);	
	case V4L2_CID_HFLIP:
		return sensor_s_hflip(sd, ctrl->value);
	case V4L2_CID_VFLIP:
		return sensor_s_vflip(sd, ctrl->value);
	default:
	case WMT_V4L2_CID_CAMERA_ANTIBANDING:
		return -EINVAL;
	}
	return -EINVAL;
}

static int sensor_g_mbus_fmt(struct cmos_subdev *sd,
			     struct v4l2_mbus_framefmt *mf)
{
	return -EINVAL;
}

static int sensor_s_mbus_fmt(struct cmos_subdev *sd,
			     struct v4l2_mbus_framefmt *mf)
{
	const struct cmos_win_size *win;

	win = cmos_select_win(&mf->width, &mf->height);
	if (!win) {
		pr_err("%s, s_mbus_fmt failed, width %d, height %d\n", 
		       sd->name, mf->width, mf->height);
		return -EINVAL;
	}

	sensor_write_array(sd, win->regs, win->size);
	return 0;
}

static int sensor_try_mbus_fmt(struct cmos_subdev *sd,
			       struct v4l2_mbus_framefmt *mf)
{
	return 0;
}

static int sensor_enum_framesizes(struct cmos_subdev *sd,
		struct v4l2_frmsizeenum *fsize)
{
	int i;
	int num_valid = -1;
	__u32 index = fsize->index;

	for (i = 0; i < ARRAY_SIZE(cmos_supported_win_sizes); i++) {
		const struct cmos_win_size *win = &cmos_supported_win_sizes[index];
		if (index == ++num_valid) {
			fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
			fsize->discrete.width = win->width;
			fsize->discrete.height = win->height;
			return 0;
		}
	}

	return -EINVAL;
}

static int sensor_identify(struct cmos_subdev *sd)
{
	int data;

	sensor_write(sd, 0x00, 0x0);

	data = sensor_read(sd, 0x01);

	return (data == sd->id) ? 0 : -EINVAL;
}

static int sensor_init(struct cmos_subdev *sd)
{
	int version;

	if (sensor_identify(sd)) {
		return -1;
	}

	version = sensor_read(sd, 0x02);
	printk("version:0x%x\n",version);
	sensor_write_array(sd, siv120d_default_regs_init_120d,
				   ARRAY_SIZE(siv120d_default_regs_init_120d));

	return 0;
}

static int sensor_exit(struct cmos_subdev *sd)
{
	return 0;
}

static struct cmos_subdev_ops siv120d_ops = {
	.identify	= sensor_identify,
	.init		= sensor_init,
	.exit		= sensor_exit,
	.queryctrl	= sensor_queryctrl,
	.s_ctrl		= sensor_s_ctrl,
	.s_mbus_fmt     = sensor_s_mbus_fmt,
	.g_mbus_fmt     = sensor_g_mbus_fmt,
	.try_mbus_fmt	= sensor_try_mbus_fmt,
	.enum_framesizes = sensor_enum_framesizes,
};

struct cmos_subdev siv120d = {
	.name		= "siv120d",
	.i2c_addr	= 0x33,
	.id		= 0x12,
	.max_width	= 640,
	.max_height	= 480,
	.ops		= &siv120d_ops,
};

#if 0
static int __init siv120d_init(void)
{
	return cmos_register_sudbdev(&siv120d);
}

static void __exit siv120d_exit(void)
{
	return cmos_unregister_subdev(&siv120d);
}

module_init(siv120d_init);
module_exit(siv120d_exit);

MODULE_LICENSE("GPL");
#endif