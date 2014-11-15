#ifndef GC2035_H
#define GC2035_H


// Scene Mode
uint8_t gc2035_scene_mode_auto[] = {
	0xfe,0x01,
	0x3e,0x40,
	0x1f,0xa0,//max_post_gain c0 2013-7-22 dustin
	0x20,0x50,//max_pre_gain 60 2013-7-22 dustin
	0xfe,0x00,
};

uint8_t gc2035_scene_mode_night[] = {
	0xfe, 0x01,
	0x3e, 0x60,
	0x1f,0xe0,//max_post_gain c0 2013-7-22 dustin
	0x20,0x80,//max_pre_gain 60 2013-7-22 dustin
	0xfe, 0x00,
};


// White Balance
uint8_t gc2035_wb_auto [] = {
	0xfe, 0x00,
//	0xb3, 0x61, //2013-8-16 dustin
//	0xb4, 0x40,
//	0xb5, 0x61,
	0x82, 0xfe,
};

uint8_t gc2035_wb_incandescent [] = {
	0xfe, 0x00,
	0x82, 0xfc,
	0xb3, 0x50,
	0xb4, 0x40,
	0xb5, 0xa8,
};

uint8_t gc2035_wb_fluorescent [] = {
	0xfe, 0x00,
	0x82, 0xfc,
	0xb3, 0x50,
	0xb4, 0x40,
	0xb5, 0x78,
};

uint8_t gc2035_wb_daylight [] = {
	0xfe, 0x00,
	0x82, 0xfc,
	0xb3, 0x78,
	0xb4, 0x40,
	0xb5, 0x50,
};

uint8_t gc2035_wb_cloudy [] = {
	0xfe, 0x00,
	0x82, 0xfc,
	0xb3, 0x58,
	0xb4, 0x40,
	0xb5, 0x50,
};


// Exposure
uint8_t gc2035_exposure_neg6[] = {
	0xfe, 0x01,
	0x13, 0x60,
	0xfe, 0x02,
	0xd5, 0xe0,
	0xfe, 0x00,
	
};

uint8_t gc2035_exposure_neg3[] = {
	0xfe, 0x01,
	0x13, 0x78,
	0xfe, 0x02,
	0xd5, 0xf0,
	0xfe, 0x00,

};

uint8_t gc2035_exposure_zero[] = {
	0xfe, 0x01,
	0x13, 0x84,
	0xfe, 0x02,
	0xd5, 0xf8,
	0xfe, 0x00,
};

uint8_t gc2035_exposure_pos3[] = {
	0xfe, 0x01,
	0x13, 0x88,
	0xfe, 0x02,
	0xd5, 0x10,
	0xfe, 0x00,


};

uint8_t gc2035_exposure_pos6[] = {
	0xfe, 0x01,
	0x13, 0x90,
	0xfe, 0x02,
	0xd5, 0x20,
	0xfe, 0x00,
};

uint8_t gc2035_176_144_regs[]={
	0xfe,0x00,

	0x99,0x33,
	0x9b,0x00,
	0x9f,0x00,
	0x90,0x01,

	0x95,0x00,
	0x96,0x92,//600
	0x97,0x00,
	0x98,0xb0,//800

	0xc8,0x15,
	0xf7,0x11,

	0xfe,0x03,
	0x42,0x60,
	0x43,0x01,
	0x41,0x02,
	0x40,0x40,
	0x17,0x00,
	0xfe,0x00,
	0xc8,0x54,
};

uint8_t gc2035_320_240_regs[]={
	0xfe,0x00,

	0x99,0x22,
	0x9b,0x00,
	0x9f,0x00,

	0x90,0x01,

	0x95,0x00,
	0x96,0xf2,//600
	0x97,0x01,
	0x98,0x40,//800

	0xc8,0x15,
	0xf7,0x11,
	0xfe,0x03,
	0x42,0x80,
	0x43,0x02,
	0x41,0x02,
	0x40,0x40,
	0x17,0x00,
	0xfe,0x00,
	0xc8,0x54,
};

uint8_t gc2035_1280_720_regs[]={
	0xfe,0x00,
	0x0a,0xf0,
	0x0c,0xa0,
	0x0d,0x02,
	0x0e,0xd8,
	0x0f,0x05,
	0x10,0x18,
	0xfe,0x00,
	0xec,0x04,
	0xed,0x04,
	0xee,0x50,
	0xef,0x58,
	0x05,0x01,
	0x06,0xaa,
	0x07,0x00,
	0x08,0xf6,
	0xfe,0x01,
	0x27,0x00,
	0x28,0xa1,
	0x29,0x03,
	0x2a,0xc6,
	0x2b,0x04,
	0x2c,0x67,
	0x2d,0x06,
	0x2e,0x4a,
	0x2f,0x07,
	0x30,0x8c,
	0x3e,0x40,
	0xfe,0x00,
	0xb6,0x03,
	0xec,0x04,
	0xed,0x04,
	0xee,0x50,
	0xef,0x58,
	0x99,0x11,
	0x90,0x01,
	0x95,0x02,
	0x96,0xd0,
	0x97,0x05,
	0x98,0x00,
	0xfe,0x03,
	0x42,0x80,
	0x43,0x06,
	0x41,0x00,
	0x40,0x00,
	0x17,0x01,
	0xfe,0x00,
	0xc8,0x00,
	0xfa,0x11,
};

uint8_t gc2035_1600_1200_regs[]={
	0xfe,0x00,
	0x0c,0x02,

	0x0d,0x04,
	0x0e,0xc0,
	0x0f,0x06,
	0x10,0x58,

	0x90,0x01,
	0x99,0x11,
	0x95,0x04,
	0x96,0xb0,
	0x97,0x06,
	0x98,0x40,

	0xc8,0x00,
	0xf7,0x15,
	0xfe,0x03,
	0x42,0x80,
	0x43,0x06,
	0x41,0x00,
	0x40,0x00,
	0x17,0x01,
	0xfe,0x00,
	0xc8,0x00,
};

uint8_t gc2035_800_600_regs[]={
	0xfe,0x00,	
	0xfa,0x00,
	0xc8,0x00,

	0x99,0x22,
	0x9a,0x07,
	0x9b,0x00,  
	0x9c,0x00,
	0x9d,0x00,
	0x9e,0x00,
	0x9f,0x00,  
	0xa0,0x00,
	0xa1,0x00,
	0xa2,0x00,

	0x90,0x01,
	0x95,0x02,
	0x96,0x58,  
	0x97,0x03,
	0x98,0x20,
};

uint8_t gc2035_640_480_regs[]={
	0xfe,0x00,
	0x0c,0x00,

	0x0d,0x04,
	0x0e,0xc0,
	0x0f,0x06,
	0x10,0x58,

	0x90,0x01,
	0x99,0x11,
	0x95,0x01,
	0x96,0xe0,
	0x97,0x02,
	0x98,0x80,

	0xc8,0x15,
	0xf7,0x15,
	0xfe,0x03,
	0x42,0x00,
	0x43,0x05,
	0x41,0x02,
	0x40,0x40,
	0x17,0x00,
	0xfe,0x00,
	0xc8,0x55,
	0xb6,0x03,
};

uint8_t gc2035_default_regs_init[]={
	0xfe,0x80,
	0xfe,0x80,
	0xfe,0x80,  
	0xfc,0x06,
	0xf2,0x00,
	0xf3,0x00,
	0xf4,0x00,
	0xf5,0x00,
	0xf9,0xfe, //[0] pll enable
	0xfa,0x00,
	0xf6,0x00,
	0xf7,0x15, //pll enable
	
	0xf8,0x84,//85
	0xfe,0x00,
	0x82,0x00,
	0xb3,0x60,
	0xb4,0x40,
	0xb5,0x60,

	0x03,0x02,
	0x04,0x71,

	////,/meare window  ///////////
	0xfe,0x00,
	0xec,0x06,//04 
	0xed,0x06,//04 
	0xee,0x62,//60 
	0xef,0x92,//90 

	////,//anog/////////////
	0x0a,0x00, //row start
	0x0c,0x00, //col start
	0x0d,0x04,
	0x0e,0xc0,
	0x0f,0x06, //Window setting
	0x10,0x58, 
	0x17,0x14, //[0]mirror [1]flip
	0x18,0x0a, //0a 2012.10.26
	0x19,0x0a, //AD pipe number
	0x1a,0x01, //CISCTL mode4
	0x1b,0x8b,
	0x1c,0x05,//add by lanking 20130403
	0x1e,0x88, //analog mode1 [7] tx-high en [5:3]COL_bias
	0x1f,0x08, //[3] tx-low en//
	0x20,0x05, //[0]adclk mode},{0x[1]rowclk_MODE [2]rsthigh_en
	0x21,0x0f, //[6:4]rsg
	0x22,0xf0, //[3:0]vref  20130403
	0x23,0xc3, //f3//ADC_r
	0x24,0x17, //pad drive  16


	0xfe,0x01,
	0x11,0x20,//AEC_out_slope},{0x
	0x1f,0xc0,//max_post_gain
	0x20,0x48,//max_pre_gain
	0x47,0x30,//AEC_outdoor_th
	0x0b,0x10,//
	0x13,0x84,//y_target
	0xfe,0x00,

#if 0
	0x05,0x01,//hb
	0x06,0x11,
	0x07,0x00,//vb
	0x08,0x50,
	0xfe,0x01,
	0x27,0x00,//step
	0x28,0xa0,
	0x29,0x05,//level1
	0x2a,0x00,
	0x2b,0x05,//level2
	0x2c,0x00,
	0x2d,0x06,//6e8//level3
	0x2e,0xe0,
	0x2f,0x0a,//level4
	0x30,0x00,
#endif
	0x05,0x01,//hb
	0x06,0x5c,
	0x07,0x00,//vb
	0x08,0x32,
	0xfe,0x01,
	0x27,0x00,//step
	0x28,0x7d,
	0x29,0x05,//level1
	0x2a,0xdc,
	0x2b,0x06,//level2
	0x2c,0x59,
	0x2d,0x06,//6e8//level3
	0x2e,0xd6,
	0x2f,0x09,//level4
	0x30,0xc4,

	0x3e,0x40,
	0xfe,0x00,
	0xfe,0x00,  //0x},{0x},{0x},{0x},{0x 
	0xb6,0x03, //AEC enable
	0xfe,0x00,
	
	////,BLK////
	0x3f,0x00, //prc close
	0x40,0x77,//
	0x42,0x7f,
	0x43,0x30,
	0x5c,0x08,
	0x5e,0x20,
	0x5f,0x20,
	0x60,0x20,
	0x61,0x20,
	0x62,0x20,
	0x63,0x20,
	0x64,0x20,
	0x65,0x20,

	//blo,/////////
	0x80,0xff,
	0x81,0x26,//38},{0x//skin_Y 8c_debug
	0x87,0x90, //[7]middle gamma 
	0x84,0x02, //output put foramat
	0x86,0x02, //02 //sync plority 
	0x8b,0xbc,
	0xb0,0x80, //globle gain
	0xc0,0x40,//Yuv bypass
  
	////lsc///////////
	0xfe,0x01,
	0xc2,0x38,
	0xc3,0x25,
	0xc4,0x21,
	0xc8,0x19,
	0xc9,0x12,
	0xca,0x0e,
	0xbc,0x43,
	0xbd,0x18,
	0xbe,0x1b,
	0xb6,0x40,
	0xb7,0x2e,
	0xb8,0x26,
	0xc5,0x05,
	0xc6,0x03,
	0xc7,0x04,
	0xcb,0x00,
	0xcc,0x00,
	0xcd,0x00,
	0xbf,0x14,
	0xc0,0x22,
	0xc1,0x1b,
	0xb9,0x00,
	0xba,0x05,
	0xbb,0x05,
	0xaa,0x35,
	0xab,0x33,
	0xac,0x33,
	0xad,0x25,
	0xae,0x22,
	0xaf,0x27,
	0xb0,0x1d,
	0xb1,0x20,
	0xb2,0x22,
	0xb3,0x14,
	0xb4,0x15,
	0xb5,0x16,
	0xd0,0x00,
	0xd2,0x07,
	0xd3,0x08,
	0xd8,0x00,
	0xda,0x13,
	0xdb,0x17,
	0xdc,0x00,
	0xde,0x0a,
	0xdf,0x08,
	0xd4,0x00,
	0xd6,0x00,
	0xd7,0x0c,
	0xa4,0x00,
	0xa5,0x00,
	0xa6,0x00,
	0xa7,0x00,
	0xa8,0x00,
	0xa9,0x00,
	0xa1,0x80,
	0xa2,0x80,
  
	////,/cc////////////
	0xfe,0x02,
	0xc0,0x01,
	0xc1,0x40, //Green_cc for d
	0xc2,0xfc,
	0xc3,0x05,
	0xc4,0xec,
	0xc5,0x42,
	0xc6,0xf8,
	0xc7,0x40,//for cwf 
	0xc8,0xf8,
	0xc9,0x06,
	0xca,0xfd,
	0xcb,0x3e,
	0xcc,0xf3,
	0xcd,0x36,//for A
	0xce,0xf6,
	0xcf,0x04,
	0xe3,0x0c,
	0xe4,0x44,
	0xe5,0xe5,
	0xfe,0x00,
   
	////,b stt ////////////////AWB ,ar
	0xfe,0x01,
	0x4f,0x00,
	0x4d,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x10, // 10
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x20, // 20
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x30,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00, // 30
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x40, // 40
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x50, // 50
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x60, // 60
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x70, // 70
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x80, // 80
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0x90, // 90
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0xa0, // a0
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0xb0, // b0
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0xc0, // c0
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4d,0xd0, // d0
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4e,0x00,
	0x4f,0x01,
	////,wb vue////////
	0xfe,0x01,
	0x4f,0x00,
	0x4d,0x30,
	0x4e,0x00,
	0x4e,0x80,
	0x4e,0x80,
	0x4e,0x02,
	0x4e,0x02,
	0x4d,0x40,
	0x4e,0x00,
	0x4e,0x80,
	0x4e,0x80,
	0x4e,0x02,
	0x4e,0x02,
	0x4e,0x02,
	0x4d,0x53,
	0x4e,0x08,
	0x4e,0x04,
	0x4d,0x62,
	0x4e,0x10,
	0x4d,0x72,
	0x4e,0x20,
	0x4f,0x01,
    
	///a,///
	0xfe,0x01,
	0x50,0x88,//c0//[6]green mode
	0x52,0x40,
	0x54,0x60,
	0x56,0x06,
	0x57,0x20, //pre adjust
	0x58,0x01, 
	0x5b,0x02, //AWB_gain_delta
	0x61,0xaa,//R/G stand
	0x62,0xaa,//R/G stand
	0x71,0x00,
	0x74,0x10,  //0x//AWB_C_max
	0x77,0x08, // 0x//AWB_p2_x
	0x78,0xfd, //AWB_p2_y
	0x86,0x30,
	0x87,0x00,
	0x88,0x04,//06},{0x//[1]dark mode
	0x8a,0xc0,//awb move mode
	0x89,0x75,
	0x84,0x08,  //0x//auto_window
	0x8b,0x00, // 0x//awb compare luma
	0x8d,0x70, //awb gain limit R 
	0x8e,0x70,//G
	0x8f,0xf4,//B
	0xfe,0x00,
	0x82,0x02,//awb_en
  ////wb end /////////////
     
	//===,====de
	0xfe,0x01,

	0x9c,0x04,//00

	0x21,0xbf,
	0xfe,0x02,
	0xa4,0x00,//
	0xa5,0x40, //lsc_th
	0xa2,0xa0, //lsc_dec_slope
	0xa6,0x80, //dd_th
	0xa7,0x80, //ot_th
	0xab,0x31, //
	0xa9,0x6f, //
	0xb0,0x99, //0x//edge effect slope low
	0xb1,0x34,//edge effect slope low
	0xb3,0x80, //saturation dec slope
	0xde,0xb6,  //
	0x38,0x0f, // 
	0x39,0x60, //
	0xfe,0x00,
	0x81,0x26,
	0xfe,0x02,
	0x83,0x00,//
	0x84,0x45,//
	////,///Y//////////
	0xd1,0x34,//saturation_cb
	0xd2,0x34,//saturation_Cr
	0xd3,0x40,//contrast ?	{{0xd4},{0x80}},//contrast center 
	0xd5,0xf8,//luma_offset 
	0xdc,0x30,
	0xdd,0xb8,//edge_sa_g,b
	0xfe,0x00,
	////,dd/////////
	0xfe,0x02,
	0x88,0x1c,//dn_b_base
	0x8c,0xf6, //[2]b_in_dark_inc
	0x89,0x03, //dn_c_weight
	////,E /////////
	0xfe,0x02,
	0x90,0x6c,// EEINTP mode1
	0x97,0x45,// edge effect
	//==,=======RGB Gamma 
	0xfe,0x02,
	0x15,0x0a,
	0x16,0x12,
	0x17,0x19,
	0x18,0x1f,
	0x19,0x2c,
	0x1a,0x38,
	0x1b,0x42,
	0x1c,0x4e,
	0x1d,0x63,
	0x1e,0x76,
	0x1f,0x87,
	0x20,0x96,
	0x21,0xa2,
	0x22,0xb8,
	0x23,0xca,
	0x24,0xd8,
	0x25,0xe3,
	0x26,0xf0,
	0x27,0xf8,
	0x28,0xfd,
	0x29,0xff,
 
	//===,=========y gamma
	0xfe,0x02,
	0x2b,0x00,
	0x2c,0x04,
	0x2d,0x09,
	0x2e,0x18,
	0x2f,0x27,
	0x30,0x37,
	0x31,0x49,
	0x32,0x5c,
	0x33,0x7e,
	0x34,0xa0,
	0x35,0xc0,
	0x36,0xe0,
	0x37,0xff,
	///1,x120ize// 
	0xfe,0x00,//
	0x90,0x01, //0x//crop enable
	0x95,0x04,  //0x//1600x1200
	0x96,0xb0,
	0x97,0x06,
	0x98,0x40,
    
	0xfe,0x03,
	0x42,0x40, 
	0x43,0x06, //output buf width
	0x41,0x02, // Pclk_polarity
	0x40,0x40,  //00  
	0x17,0x00, //widv 
	0xfe,0x00,
	//ou,t DV////
     
	0xfe,0x00,
	0x82,0xfe,  // fe
	0xf2,0x70, 
	0xf3,0xff,
	0xf4,0x00,
	0xf5,0x30,

  #if 0
    //,///ssumple  640X4800//////
	0xfe,0x00,	
	0xfa,0x00,
	0xc8,0x00,
    
	0x99,0x22,
	0x9a,0x07,
	0x9b,0x00,  
	0x9c,0x00,
	0x9d,0x00,
	0x9e,0x00,
	0x9f,0x00,  
	0xa0,0x00,
	0xa1,0x00,
	0xa2,0x00,
    
	  
	0x90,0x01,
	0x95,0x01,
	0x96,0xe0,  
	0x97,0x02,
	0x98,0x80,
  #endif


  	0xfe,0x00,
	0x0c,0x00,

	0x0d,0x04,
	0x0e,0xc0,
	0x0f,0x06,
	0x10,0x58,

	0x90,0x01,
	0x99,0x11,
	0x95,0x01,
	0x96,0xe0,
	0x97,0x02,
	0x98,0x80,

	0xc8,0x15,
	0xf7,0x15,
	0xfe,0x03,
	0x42,0x00,
	0x43,0x05,
	0x41,0x02,
	0x40,0x40,
	0x17,0x00,
	0xfe,0x00,
	0xc8,0x55,
	0xb6,0x03,
	
#if 1 
  ////  re zao///
	0xfe ,0x00,
	0x22 ,0xf0,
	0xfe ,0x01,
	0x21 ,0xff,
	0xfe ,0x02,  
	0x8a ,0x33,
	0x8c ,0x76,
	0x8d ,0x85,
	0xa6 ,0xf0,	
	0xae ,0x9f,
	0xa2 ,0x90,
	0xa5 ,0x40,  
	0xa7 ,0x30,
	0xb0 ,0x88,
	0x38 ,0x0b,
	0x39 ,0x30,
	0xfe ,0x00,  
	0x87 ,0xb0,


  /*
   //, sma  RGB gamma////
	0xfe,0x02,
	0x15,0x0b,
	0x16,0x0e,
	0x17,0x10,
	0x18,0x12,
	0x19,0x19,
	0x1a,0x21,
	0x1b,0x29,
	0x1c,0x31,
	0x1d,0x41,
	0x1e,0x50,
	0x1f,0x5f,
	0x20,0x6d,
	0x21,0x79,
	0x22,0x91,
	0x23,0xa5,
	0x24,0xb9,
	0x25,0xc9,
	0x26,0xe1,
	0x27,0xee,
	0x28,0xf7,
	0x29,0xff,
    */
 	//da,sun///
	0xfe,0x02,
	0x40,0x06,
	0x41,0x23,
	0x42,0x3f,
	0x43,0x06,
	0x44,0x00,
	0x45,0x00,
	0x46,0x14,
	0x47,0x09,
 
  #endif
};

uint8_t gc2035_default_regs_exit[]={
	0xfe, 0x00,
	0xf1, 0x00,
	0xf2, 0x00,
};

#endif
