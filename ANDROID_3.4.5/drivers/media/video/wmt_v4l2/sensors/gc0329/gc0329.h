#ifndef GC0329_H
#define GC0329_H


// Scene Mode
uint8_t gc0329_scene_mode_auto[] = {
};

uint8_t gc0329_scene_mode_night[] = {
};


// White Balance
uint8_t gc0329_wb_auto [] = {
	0xfe, 0x00,
	0x77, 0x57,
	0x78, 0x4d,
	0x79, 0x45,
	0x42, 0xfe, //AWB on
};

uint8_t gc0329_wb_incandescent [] = {
	0xfe, 0x00,
	0x42, 0xfc,
	0x77, 0x48,
	0x78, 0x40,
	0x79, 0x5c,

};

uint8_t gc0329_wb_fluorescent [] = {
	0xfe, 0x00,
	0x42, 0xfc,
	0x77, 0x40,
	0x78, 0x42,
	0x79, 0x50,

};

uint8_t gc0329_wb_daylight [] = {
	0xfe, 0x00,
	0x42, 0xfc,
	0x77, 0x74,
	0x78, 0x52,
	0x79, 0x40,

};

uint8_t gc0329_wb_cloudy [] = {
	0xfe, 0x00,
	0x42, 0xfc,
	0x77, 0x8c,
	0x78, 0x50,
	0x79, 0x40,

};

uint8_t gc0329_wb_tungsten [] = {
	0xfe, 0x00,
	0x42, 0xfc,
	0x77, 0x40,
	0x78, 0x54,
	0x79, 0x70,

};


// Exposure
uint8_t gc0329_exposure_neg6[] = {
	0xfe, 0x00,
	0xd5, 0xe0,
	0xfe, 0x01,
	0x13, 0x40,
	0xfe, 0x00,

};

uint8_t gc0329_exposure_neg3[] = {
	0xfe, 0x00,
	0xd5, 0xf0,
	0xfe, 0x01,
	0x13, 0x48,
	0xfe, 0x00,

};

uint8_t gc0329_exposure_zero[] = {
	0xfe, 0x00,
	0xd5, 0x04,  /// 00
	0xfe, 0x01,
	0x13, 0x6a,
	0xfe, 0x00,

};

uint8_t gc0329_exposure_pos3[] = {

	0xfe, 0x00,
	0xd5, 0x10,
	0xfe, 0x01,
	0x13, 0x70,
	0xfe, 0x00,

};

uint8_t gc0329_exposure_pos6[] = {
	0xfe, 0x00,
	0xd5, 0x20,
	0xfe, 0x01,
	0x13, 0x80,
	0xfe, 0x00,

};


// Color Effect
uint8_t gc0329_colorfx_none[] = {
	 0xfe, 0x00,
	 0x43 , 0x00,
};

uint8_t gc0329_colorfx_bw[] = {
	0xfe, 0x00,
	0x43 , 0x02,
	0xda , 0x00,
	0xdb , 0x00,

};

uint8_t gc0329_colorfx_sepia[] = {
	0xfe, 0x00,
	0x43 , 0x02,
	0xda , 0xd0,
	0xdb , 0x28,

};

uint8_t gc0329_colorfx_negative[] = {
	0xfe, 0x00,
	0x43 , 0x01,
};

uint8_t gc0329_colorfx_emboss[] = {
};

uint8_t gc0329_colorfx_sketch[] = {
 
};

uint8_t gc0329_colorfx_sky_blue[] = {
	0xfe, 0x00,
	0x43 , 0x02,
	0xda , 0x50,
	0xdb , 0xe0,

};

uint8_t gc0329_colorfx_grass_green[] = {
	
	0xfe, 0x00,	
	0x43 , 0x02,
	0xda , 0xc0,
	0xdb , 0xc0,

};

uint8_t gc0329_colorfx_skin_whiten[] = {
};

uint8_t gc0329_colorfx_vivid[] = {
};

// Brightness
uint8_t gc0329_brightness_neg4[] = {
};

uint8_t gc0329_brightness_neg3[] = {
};

uint8_t gc0329_brightness_neg2[] = {
};

uint8_t gc0329_brightness_neg1[] = {
};

uint8_t gc0329_brightness_zero[] = {
};

uint8_t gc0329_brightness_pos1[] = {
};

uint8_t gc0329_brightness_pos2[] = {
};

uint8_t gc0329_brightness_pos3[] = {
};

uint8_t gc0329_brightness_pos4[] = {
};

// Contrast
uint8_t gc0329_contrast_neg4[] = {
};

uint8_t gc0329_contrast_neg3[] = {
};

uint8_t gc0329_contrast_neg2[] = {
};

uint8_t gc0329_contrast_neg1[] = {
};

uint8_t gc0329_contrast_zero[] = {
};

uint8_t gc0329_contrast_pos1[] = {
};

uint8_t gc0329_contrast_pos2[] = {
};

uint8_t gc0329_contrast_pos3[] = {
};

uint8_t gc0329_contrast_pos4[] = {
};

// Saturation
uint8_t gc0329_saturation_neg4[] = {
};

uint8_t gc0329_saturation_neg3[] = {
};

uint8_t gc0329_saturation_neg2[] = {
};

uint8_t gc0329_saturation_neg1[] = {
};

uint8_t gc0329_saturation_zero[] = {
};

uint8_t gc0329_saturation_pos1[] = {
};

uint8_t gc0329_saturation_pos2[] = {
};

uint8_t gc0329_saturation_pos3[] = {
};

uint8_t gc0329_saturation_pos4[] = {
};


// Resolution

// QCIF
uint8_t gc0329_176x144[]={
};

uint8_t gc0329_320x240[]={
	0x59, 0x22, 
};

uint8_t gc0329_640x480[]={
	0x59, 0x11, 
};


// Initiliztion
uint8_t gc0329_default_regs_init[] = {
	0xfe, 0x80,
	0xfc, 0x12,
	0xfc, 0x12,
	0xfe, 0x00,

	0xfa, 0x00,

	0xf0, 0x07,
	0xf1, 0x01,
	0x73, 0x90,
	0x74, 0x80,
	0x75, 0x80,
	0x76, 0x94,
	0xfc, 0x16,

	0x05, 0x01,
	0x06, 0xfa,
	0x07, 0x00,
	0x08, 0x70,
	0xfe, 0x01,
	0x29, 0x00,
	0x2a, 0x64,
	0x2b, 0x02,
	0x2c, 0x58,
    0x2d, 0x04,

	0x2e, 0xb0,
	0x2f, 0x06,
	0x30, 0x40,
	0x31, 0x09,
	0x32, 0xc4,
	0x33, 0x20,
	0xfe, 0x00,



	
	
	0x0a, 0x00,
	0x0c, 0x00,
	0x17, 0x14,
	0x19, 0x05,
	0x1b, 0x24,
	0x1c, 0x04,
	0x1e, 0x00,
	0x1f, 0xc8,
	0x20, 0x00,
	0x21, 0x48,
	0x23, 0x22,
	0x24, 0x16,

	
	0x26, 0xf7,
	0x28, 0x7f,
	0x29, 0x00,
	0x32, 0x00,
	0x33, 0x20,
	0x34, 0x20,
	0x35, 0x20,
	0x36, 0x20,
	0x3b, 0x04,
	0x3c, 0x04,
	0x3d, 0x04,
	0x3e, 0x04,
	
	0x40, 0xff,
	0x41, 0x00,
	0x42, 0xfe,
	0x46, 0x02,
	0x4b, 0xcb,
	0x4d, 0x01,
	0x4f, 0x01,
	0x70, 0x48,
	0x80, 0xe7,
	0x82, 0x55,
	0x87, 0x4a,
	0xfe, 0x01,
	0x18, 0x22,
	0xfe, 0x00,
	0x9c, 0x0a,
	0xa4, 0x50,
	0xa5, 0x21,
	0xa7, 0x35,
	0xdd, 0x54,
	0x95, 0x35,
	0xfe, 0x00,
	///gamma
	/*
	0xbf, 0x06,
	0xc0, 0x14,
	0xc1, 0x27,
	0xc2, 0x3b,
	0xc3, 0x4f,
	0xc4, 0x62,
	0xc5, 0x72,
	0xc6, 0x8d,
	0xc7, 0xa4,
	0xc8, 0xb8,
	0xc9, 0xc9,
	0xca, 0xd6,
	0xcb, 0xe0,
	0xcc, 0xe8,
	0xcd, 0xf4,
	0xce, 0xfc,
	0xcf, 0xff,
	*/

	0xBF, 0x14,
	0xc0, 0x28,
	0xc1, 0x44,
	0xc2, 0x5D,
	0xc3, 0x72,
	0xc4, 0x86,
	0xc5, 0x95,
	0xc6, 0xB1,
	0xc7, 0xC6,
	0xc8, 0xD5,
	0xc9, 0xE1,
	0xcA, 0xEA,
	0xcB, 0xF1,
	0xcC, 0xF5,
	0xcD, 0xFB,
	0xcE, 0xFE,
	0xcF, 0xFF,

	
	0xfe, 0x00,
	0xb3, 0x44,
	0xb4, 0xfd,
	0xb5, 0x02,
	0xb6, 0xfa,
	0xb7, 0x48,
	0xb8, 0xf0,
	0x50, 0x01,
	0x19, 0x05,
	0x20, 0x01,
	0x22, 0xba,
	0x21, 0x48,
	0xfe, 0x00,
	0xd1, 0x34,
	0xd2, 0x34,
	0xfe, 0x01,
	0x10, 0x40,
	0x12, 0x17,
	0x13, 0x60,
	0x17, 0x88,
	0x21, 0xb0,
	0x22, 0x60,
	0x3c, 0x82,
	0x3d, 0x55,
	0x3e, 0x48,
	0xfe, 0x01,
	0x06, 0x08,
	0x07, 0x06,
	0x08, 0xa6,
	0x09, 0xee,
	0x6a, 0x40,
	0x6b, 0x39,
	0xd0, 0x00,
	0xd2, 0x2c,
	0xd3, 0x80,
	0x9c, 0x00,
	0x9d, 0x10,
	0x9e, 0xc0,
	0x9f, 0x40,
	0xfe, 0x01,
	0xa0, 0x00,
	0xa1, 0x3c,
	0xa2, 0x50,
	0xa3, 0x00,
	0xa8, 0x0f,
	0xa9, 0x08,
	0xaa, 0x00,
	0xab, 0x04,
	0xac, 0x00,
	0xad, 0x07,
	0xae, 0x0e,
	0xaf, 0x00,
	0xb0, 0x00,
	0xb1, 0x09,
	0xb2, 0x00,
	0xb3, 0x00,
	0xb4, 0x31,
	0xb5, 0x19,
	0xb6, 0x24,
	0xba, 0x3a,
	0xbb, 0x24,
	0xbc, 0x2a,
	0xc0, 0x17,
	0xc1, 0x13,
	0xc2, 0x17,
	0xc6, 0x21,
	0xc7, 0x1c,
	0xc8, 0x1c,
	0xb7, 0x00,
	0xb8, 0x00,
	0xb9, 0x00,
	0xbd, 0x00,
	0xbe, 0x00,
	0xbf, 0x00,
	0xc3, 0x00,
	0xc4, 0x00,
	0xc5, 0x00,
	0xc9, 0x00,
	0xca, 0x00,
	0xcb, 0x00,
	0xa4, 0x00,
	0xa5, 0x00,
	0xa6, 0x00,
	0xa7, 0x00,
	0xfe, 0x00,

	0xfe,0x01, 
	0x18,0x22, 
	0x21,0xc0, 
	0x06,0x12, 
	0x08,0x9c, 
	0x51,0x28, 
	0x52,0x10, 
	0x53,0x20, 
	0x54,0x40, 
	0x55,0x16, 
	0x56,0x30, 
	0x58,0x60, 
	0x59,0x08, 
	0x5c,0x35, 
	0x5d,0x72, 
	0x67,0x80, 
	0x68,0x60, 
	0x69,0x90, 
	0x6c,0x30, 
	0x6d,0x60, 
	0x70,0x10, 
	0xfe,0x00, 
	0x9c,0x0a, 
	0xa0,0xaf, 
	0xa2,0xff, 
	0xa4,0x60, 
	0xa5,0x31, 
	0xa7,0x35, 
	0x42,0xfe, 
	0xd1,0x34, 
	0xd2,0x34, 
	0xd3,0x3b,
	0xd5,0x0c,
	0xfe,0x00, 

	0xa0, 0xaf,
	0xa2, 0xff,
	0x44, 0xa2,
};

uint8_t gc0329_default_regs_exit[] = {
};

#endif
