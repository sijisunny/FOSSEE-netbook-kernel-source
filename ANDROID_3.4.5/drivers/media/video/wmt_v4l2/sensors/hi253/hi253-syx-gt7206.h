#ifndef YACD511_H
#define YACD511_H

// Scene Mode
uint8_t hi253_scene_mode_auto[] = {
	0x03,0x10,
	0x41,0x00,	
	0x03,0x20,
	0xb2,0x80,
	0xff,0xff,
};

uint8_t hi253_scene_mode_night[] = {
	0x03,0x10,
	0x41,0x15,	
	0x03,0x20,
	0xb2,0xf0,
	0xff,0xff,
};


// White Balance
uint8_t hi253_wb_auto [] = {
	0x03,0x22,
	0x11,0x2e,
	0x83,0x53, // RMAX
	 0x84, 0x16, // RMIN
	 0x85, 0x5a, // BMAX
	 0x86, 0x25, // BMIN
	0xff,0xff,
};

uint8_t hi253_wb_incandescent [] = {
	0x03,0x22,
	0x11,0x28,
	0x80,0x2a,
	0x82,0x3f,
	0x83,0x35,
	0x84,0x28,
	0x85,0x45,
	0x86,0x3b,
	0xff,0xff,
};

uint8_t hi253_wb_fluorescent [] = {
	0x03,0x22,
	0x11,0x28,
	0x80,0x20,
	0x82,0x4d,
	0x83,0x25,
	0x84,0x1b,
	0x85,0x55,
	0x86,0x48,
	0xff,0xff,
};

uint8_t hi253_wb_daylight [] = {
	0x03,0x22,
	0x11,0x28,
	0x80,0x3d,
	0x82,0x2e,
	0x83,0x40,
	0x84,0x33,
	0x85,0x33,
	0x86,0x28,
	0xff,0xff,
};

uint8_t hi253_wb_cloudy [] = {
	0x03,0x22,
	0x11,0x28,
	0x80,0x50,
	0x82,0x25,
	0x83,0x55,
	0x84,0x4b,
	0x85,0x28,
	0x86,0x20,
	0xff,0xff,
};


// Exposure
uint8_t hi253_exposure_neg6[] = {
	0x03,0x10,
	0x40,0xa8,
};

uint8_t hi253_exposure_neg3[] = {
	0x03,0x10,
	0x40,0x98,
};

uint8_t hi253_exposure_zero[] = {
	0x03,0x10,
	       0x40,0x88,
};

uint8_t hi253_exposure_pos3[] = {
	0x03,0x10,
	0x40,0x08,
};

uint8_t hi253_exposure_pos6[] = {
	0x03,0x10,
	0x40,0x28,
};


// Resolution
uint8_t hi253_1600_1200_rets[]={
	0x03, 0x00,
	0x10, 0x00,
	0x12, 0x04,
	0x20, 0x00,
	0x21, 0x0a,
	0x22, 0x00,
	0x23, 0x0a,

	0x03, 0x18,
	0x10, 0x00,
	0xff, 0xff,
};

uint8_t hi253_1280_720_regs[]={
};

uint8_t hi253_800_600_regs[]={

	0x03, 0x00,
	0x10, 0x10,
	0x12, 0x04,

	0x03, 0x18,
	0x10, 0x00,
	0xff, 0xff,
};

uint8_t hi253_640_480_regs[]={
	0x03, 0x00,
	0x10, 0x10,
	0x12, 0x04,

	0x03, 0x18,
	0x12, 0x20,
	0x10, 0x07,
	0x11, 0x00,
	0x20, 0x05,
	0x21, 0x00,
	0x22, 0x03,
	0x23, 0xc0,
	0x24, 0x00,
	0x25, 0x04,
	0x26, 0x00,
	0x27, 0x00,
	0x28, 0x05,
	0x29, 0x04,
	0x2a, 0x03,
	0x2b, 0xc0,
	0x2c, 0x0a,
	0x2d, 0x00,
	0x2e, 0x0a,
	0x2f, 0x00,
	0x30, 0x45,
	0xff, 0xff,
};

uint8_t hi253_320_240_regs[]= {
	0x01, 0xf9,
	0x08, 0x0f,
	0x01, 0xf8,

	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,

	0x0e, 0x00,

	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,

	0x0e, 0x00,
	0x01, 0xf1,
	0x08, 0x00,
	0x01, 0xf3,
	0x01, 0xf1,

	0x03, 0x20,
	0x10, 0x0c,
	0x03, 0x22,
	0x10, 0x69,

	//Page 00
	0x03, 0x00,
	0x10, 0x11, //lxh
	0x11, 0x90,
	0x12, 0x04,
	0x0b, 0xaa,
	0x0c, 0xaa,
	0x0d, 0xaa,
	0x20, 0x00,
	0x21, 0x0a, //lxh
	0x22, 0x00,
	0x23, 0x0a, //lxh
	0x24, 0x04,
	0x25, 0xb0,
	0x26, 0x06,
	0x27, 0x40,

	0x40, 0x00,
	0x41, 0xe8,//lxh
	0x42, 0x00,
	0x43, 0x14,//lxh

	0x45, 0x04,
	0x46, 0x18,
	0x47, 0xd8,

	0xe1, 0x0f,

	//BLC
	0x80, 0x2e,
	0x81, 0x7e,
	0x82, 0x90,
	0x83, 0x00,
	0x84, 0x0c,
	0x85, 0x00,
	0x90, 0x0c,
	0x91, 0x0c,
	0x92, 0x98,
	0x93, 0x90,
	0x94, 0x75,
	0x95, 0x70,
	0x96, 0xdc,
	0x97, 0xfe,
	0x98, 0x20,

	//OutDoor BLC
	0x99,0x42,
	0x9a,0x42,
	0x9b,0x42,
	0x9c,0x42,

	//Dark BLC
	0xa0, 0x02,
	0xa2, 0x02,
	0xa4, 0x02,
	0xa6, 0x02,

	//Normal BLC
	0xa8, 0x42,
	0xaa, 0x42,
	0xac, 0x42,
	0xae, 0x42,

	//Page 02
	0x03, 0x02,
	0x12, 0x03,
	0x13, 0x03,
	0x16, 0x00,
	0x17, 0x8C,
	0x18, 0x4c,
	0x19, 0x00,
	0x1a, 0x39,
	0x1c, 0x09,
	0x1d, 0x40,
	0x1e, 0x30,
	0x1f, 0x10,
	0x20, 0x77,
	0x21, 0xde,
	0x22, 0xa7,
	0x23, 0x30,
	0x27, 0x3c,
	0x2b, 0x80,
	0x2e, 0x00,
	0x2f, 0x00,
	0x30, 0x05,
	0x50, 0x20,
	0x52, 0x01,
	0x53, 0xc1,
	0x55, 0x1c,
	0x56, 0x11,
	0x5d, 0xA2,
	0x5e, 0x5a,
	0x60, 0x87,
	0x61, 0x99,
	0x62, 0x88,
	0x63, 0x97,
	0x64, 0x88,
	0x65, 0x97,
	0x67, 0x0c,
	0x68, 0x0c,
	0x69, 0x0c,
	0x72, 0x89,
	0x73, 0x96,
	0x74, 0x89,
	0x75, 0x96,
	0x76, 0x89,
	0x77, 0x96,
	0x7C, 0x85,
	0x7d, 0xaf,
	0x80, 0x01,
	0x81, 0x7f,
	0x82, 0x13,
	0x83, 0x24,
	0x84, 0x7d,
	0x85, 0x81,
	0x86, 0x7d,
	0x87, 0x81,
	0x92, 0x48,
	0x93, 0x54,
	0x94, 0x7d,
	0x95, 0x81,
	0x96, 0x7d,
	0x97, 0x81,
	0xa0, 0x02,
	0xa1, 0x7b,
	0xa2, 0x02,
	0xa3, 0x7b,
	0xa4, 0x7b,
	0xa5, 0x02,
	0xa6, 0x7b,
	0xa7, 0x02,
	0xa8, 0x85,
	0xa9, 0x8c,
	0xaa, 0x85,
	0xab, 0x8c,
	0xac, 0x10,
	0xad, 0x16,
	0xae, 0x10,
	0xaf, 0x16,
	0xb0, 0x99,
	0xb1, 0xa3,
	0xb2, 0xa4,
	0xb3, 0xae,
	0xb4, 0x9b,
	0xb5, 0xa2,
	0xb6, 0xa6,
	0xb7, 0xac,
	0xb8, 0x9b,
	0xb9, 0x9f,
	0xba, 0xa6,
	0xbb, 0xaa,
	0xbc, 0x9b,
	0xbd, 0x9f,
	0xbe, 0xa6,
	0xbf, 0xaa,
	0xc4, 0x2c,
	0xc5, 0x43,
	0xc6, 0x63,
	0xc7, 0x79,
	0xc8, 0x2d,
	0xc9, 0x42,
	0xca, 0x2d,
	0xcb, 0x42,
	0xcc, 0x64,
	0xcd, 0x78,
	0xce, 0x64,
	0xcf, 0x78,
	0xd0, 0x0a,
	0xd1, 0x09,
	0xd4, 0x0c,
	0xd5, 0x0c,
	0xd6, 0x98,
	0xd7, 0x90,//lxh
	0xe0, 0xc4,
	0xe1, 0xc4,
	0xe2, 0xc4,
	0xe3, 0xc4,
	0xe4, 0x00,
	0xe8, 0x80,
	0xe9, 0x40,
	0xea, 0x7f,
	0xf0, 0x01,
	0xf1, 0x01,
	0xf2, 0x01,
	0xf3, 0x01,
	0xf4, 0x01,

	//PAGE10
	0x03, 0x10,
	0x10, 0x03, //lxh
	0x11, 0x03, //lxh,normal
	0x12, 0x30,
	0x13, 0x02,
	0x20, 0x00,

	0x40, 0x88,
	0x41, 0x05,
	0x48, 0x88,// 84
	0x50, 0x90,
	0x30, 0x00,
	0x31, 0x00,
	0x32, 0x00,
	0x33, 0x00,

	0x34, 0x30,
	0x35, 0x00,
	0x36, 0x00,
	0x38, 0x00,
	0x3e, 0x58,
	0x3f, 0x00,

	//Saturation
	0x60, 0x67,
	0x61, 0x84,// 74
	0x62, 0x80,// 76
	0x63, 0x50,
	0x64, 0x41,

	0x66, 0x33,
	0x67, 0x00,

	0x6a, 0x90,
	0x6b, 0x80,
	0x6c, 0x80,
	0x6d, 0xa0,

	0x76, 0x01,
	0x74, 0x66,
	0x79, 0x06,

	//Page 11
	0x03, 0x11,
	0x10, 0x7f,//lxh,3f
	0x11, 0x40,
	0x12, 0xba,
	0x13, 0xcb,
	0x26, 0x20,
	0x27, 0x22,
	0x28, 0x0f,
	0x29, 0x10,
	0x2b, 0x30,
	0x2c, 0x32,

	//Out2 D-LPF th
	0x30, 0x70,
	0x31, 0x10,
	0x32, 0x65,
	0x33, 0x09,
	0x34, 0x06,
	0x35, 0x04,

	//Out1 D-LPF th
	0x36, 0x70,
	0x37, 0x18,
	0x38, 0x65,
	0x39, 0x09,
	0x3a, 0x06,
	0x3b, 0x04,

	//Indoor D-LPF th
	0x3c, 0x80,
	0x3d, 0x18,
	0x3e, 0x80,
	0x3f, 0x0c,
	0x40, 0x09,
	0x41, 0x06,

	0x42, 0x80,
	0x43, 0x18,
	0x44, 0x80,
	0x45, 0x12,
	0x46, 0x10,
	0x47, 0x10,
	0x48, 0x90,
	0x49, 0x40,
	0x4a, 0x80,
	0x4b, 0x13,
	0x4c, 0x10,
	0x4d, 0x11,
	0x4e, 0x80,
	0x4f, 0x30,
	0x50, 0x80,
	0x51, 0x13,
	0x52, 0x10,
	0x53, 0x13,
	0x54, 0x11,
	0x55, 0x17,
	0x56, 0x20,
	0x57, 0x20,
	0x58, 0x20,
	0x59, 0x30,
	0x5a, 0x18,
	0x5b, 0x00,
	0x5c, 0x00,
	0x60, 0x3f,
	0x62, 0x50,
	0x70, 0x06,

	//Page 12
	0x03, 0x12,
	0x20, 0x0f,
	0x21, 0x0f,
	0x25, 0x30,
	0x28, 0x00,
	0x29, 0x00,
	0x2a, 0x00,
	0x30, 0x50,
	0x31, 0x18,
	0x32, 0x32,
	0x33, 0x40,
	0x34, 0x50,
	0x35, 0x70,
	0x36, 0xa0,

	//Out2 th
	0x40, 0xa0,
	0x41, 0x40,
	0x42, 0xa0,
	0x43, 0x90,
	0x44, 0x90,
	0x45, 0x80,

	//Out1 th
	0x46, 0xb0,
	0x47, 0x55,
	0x48, 0xa0,
	0x49, 0x90,
	0x4a, 0x90,
	0x4b, 0x80,

	//In door th
	0x4c, 0xb0,
	0x4d, 0x40,
	0x4e, 0x90,
	0x4f, 0x90,
	0x50, 0xe6,
	0x51, 0x80,

	//Dark1 th
	0x52, 0xb0,
	0x53, 0x60,
	0x54, 0xc0,
	0x55, 0xc0,
	0x56, 0xc0,
	0x57, 0x80,

	//Dark2 th
	0x58, 0x90,
	0x59, 0x40,
	0x5a, 0xd0,
	0x5b, 0xd0,
	0x5c, 0xe0,
	0x5d, 0x80,

	//Dark3 th
	0x5e, 0x88,
	0x5f, 0x40,
	0x60, 0xe0,
	0x61, 0xe6,
	0x62, 0xe6,
	0x63, 0x80,

	0x70, 0x15,
	0x71, 0x01,

	0x72, 0x18,
	0x73, 0x01,

	0x74, 0x25,
	0x75, 0x15,
	0x80, 0x30,
	0x81, 0x50,
	0x82, 0x80,
	0x85, 0x1a,
	0x88, 0x00,
	0x89, 0x00,
	0x90, 0x5d,

	0xc5, 0x30,
	0xc6, 0x2a,

	0xD0, 0x0c,
	0xD1, 0x80,
	0xD2, 0x67,
	0xD3, 0x00,
	0xD4, 0x00,
	0xD5, 0x02,
	0xD6, 0xff,
	0xD7, 0x18,
	0x3b, 0x06,
	0x3c, 0x06,

	0xc5, 0x30,
	0xc6, 0x2a,

	//Page 13
	0x03, 0x13,
	0x10, 0xcb,
	0x11, 0x7b,
	0x12, 0x07,
	0x14, 0x00,

	0x20, 0x15,
	0x21, 0x13,
	0x22, 0x33,
	0x23, 0x04,
	0x24, 0x09,
	0x25, 0x08,
	0x26, 0x18,
	0x27, 0x30,
	0x29, 0x12,
	0x2a, 0x50,

	//Low clip th
	0x2b, 0x06,
	0x2c, 0x06,
	0x25, 0x08,
	0x2d, 0x0c,
	0x2e, 0x12,
	0x2f, 0x12,

	//Out2 Edge
	0x50, 0x10,
	0x51, 0x14,
	0x52, 0x10,
	0x53, 0x0c,
	0x54, 0x0f,
	0x55, 0x0c,

	//Out1 Edge
	0x56, 0x10,
	0x57, 0x13,
	0x58, 0x10,
	0x59, 0x0c,
	0x5a, 0x0f,
	0x5b, 0x0c,

	//Indoor Edge
	0x5c, 0x0a,
	0x5d, 0x0b,
	0x5e, 0x0a,
	0x5f, 0x08,
	0x60, 0x09,
	0x61, 0x08,

	//Dark1 Edge
	0x62, 0x08,
	0x63, 0x08,
	0x64, 0x08,
	0x65, 0x06,
	0x66, 0x06,
	0x67, 0x06,

	//Dark2 Edge
	0x68, 0x07,
	0x69, 0x07,
	0x6a, 0x07,
	0x6b, 0x05,
	0x6c, 0x05,
	0x6d, 0x05,

	//Dark3 Edge
	0x6e, 0x07,
	0x6f, 0x07,
	0x70, 0x07,
	0x71, 0x05,
	0x72, 0x05,
	0x73, 0x05,

	//2DY
	0x80, 0xfd,
	0x81, 0x1f,
	0x82, 0x05,
	0x83, 0x01,

	0x90, 0x15,
	0x91, 0x15,
	0x92, 0x33,
	0x93, 0x30,
	0x94, 0x03,
	0x95, 0x14,
	0x97, 0x30,
	0x99, 0x30,

	0xa0, 0x04,
	0xa1, 0x05,
	0xa2, 0x04,
	0xa3, 0x05,
	0xa4, 0x07,
	0xa5, 0x08,
	0xa6, 0x07,
	0xa7, 0x08,
	0xa8, 0x07,
	0xa9, 0x08,
	0xaa, 0x07,
	0xab, 0x08,

	//Out2
	0xb0, 0x22,
	0xb1, 0x2a,
	0xb2, 0x28,
	0xb3, 0x22,
	0xb4, 0x2a,
	0xb5, 0x28,

	//Out1
	0xb6, 0x22,
	0xb7, 0x2a,
	0xb8, 0x28,
	0xb9, 0x22,
	0xba, 0x2a,
	0xbb, 0x28,

	0xbc, 0x17,
	0xbd, 0x17,
	0xbe, 0x17,
	0xbf, 0x17,
	0xc0, 0x17,
	0xc1, 0x17,

	//Dark1
	0xc2, 0x1e,
	0xc3, 0x12,
	0xc4, 0x10,
	0xc5, 0x1e,
	0xc6, 0x12,
	0xc7, 0x10,

	//Dark2
	0xc8, 0x18,
	0xc9, 0x05,
	0xca, 0x05,
	0xcb, 0x18,
	0xcc, 0x05,
	0xcd, 0x05,

	//Dark3
	0xce, 0x18,
	0xcf, 0x05,
	0xd0, 0x05,
	0xd1, 0x18,
	0xd2, 0x05,
	0xd3, 0x05,

	//Page 14
	0x03, 0x14,
	0x10, 0x11,
	0x20, 0x40,
	0x21, 0x80,
	0x23, 0x80,
	0x22, 0x80,
	0x23, 0x80,
	0x24, 0x80,

	0x30, 0xc8,
	0x31, 0x2b,
	0x32, 0x00,
	0x33, 0x00,
	0x34, 0x90,

	0x40, 0x40,
	0x50, 0x2d,
	0x60, 0x28,
	0x70, 0x2d,

	//Page 15
	0x03, 0x15,
	0x10, 0x0f,
	0x14, 0x52,
	0x15, 0x42,
	0x16, 0x32,
	0x17, 0x2f,

	//CMC
	0x30, 0x8f,
	0x31, 0x59,
	0x32, 0x0a,
	0x33, 0x15,
	0x34, 0x5b,
	0x35, 0x06,
	0x36, 0x07,
	0x37, 0x40,
	0x38, 0x86,

	//CMC OFS
	0x40, 0x95,
	0x41, 0x1f,
	0x42, 0x8a,
	0x43, 0x86,
	0x44, 0x0a,
	0x45, 0x84,
	0x46, 0x87,
	0x47, 0x9b,
	0x48, 0x23,

	//CMC POFS
	0x50, 0x8c,
	0x51, 0x0c,
	0x52, 0x00,
	0x53, 0x07,
	0x54, 0x17,
	0x55, 0x9d,
	0x56, 0x00,
	0x57, 0x0b,
	0x58, 0x89,

	0x80, 0x03,
	0x85, 0x40,
	0x87, 0x02,
	0x88, 0x00,
	0x89, 0x00,
	0x8a, 0x00,

	0x03, 0x16,
	0x10, 0x31,//GMA_CTL
	0x18, 0x5e,//AG_ON
	0x19, 0x5d,//AG_OFF
	0x1A, 0x0e,//TIME_ON
	0x1B, 0x01,//TIME_OFF
	0x1C, 0xdc,//OUT_ON
	0x1D, 0xfe,//OUT_OFF
	//GMA
	0x30, 0x00,
	0x31, 0x09,
	0x32, 0x15,
	0x33, 0x29,
	0x34, 0x53,
	0x35, 0x6c,
	0x36, 0x81,
	0x37, 0x94,
	0x38, 0xa4,
	0x39, 0xb3,
	0x3a, 0xc0,
	0x3b, 0xcb,
	0x3c, 0xd5,
	0x3d, 0xde,
	0x3e, 0xe6,
	0x3f, 0xee,
	0x40, 0xf5,
	0x41, 0xfc,
	0x42, 0xff,
	//RGMA
	0x50, 0x00,
	0x51, 0x09,
	0x52, 0x1f,
	0x53, 0x37,
	0x54, 0x5b,
	0x55, 0x76,
	0x56, 0x8d,
	0x57, 0xa1,
	0x58, 0xb2,
	0x59, 0xbe,
	0x5a, 0xc9,
	0x5b, 0xd2,
	0x5c, 0xdb,
	0x5d, 0xe3,
	0x5e, 0xeb,
	0x5f, 0xf0,
	0x60, 0xf5,
	0x61, 0xf7,
	0x62, 0xf8,
	//BGMA
	0x70, 0x00,
	0x71, 0x08,
	0x72, 0x17,
	0x73, 0x2f,
	0x74, 0x53,
	0x75, 0x6c,
	0x76, 0x81,
	0x77, 0x94,
	0x78, 0xa4,
	0x79, 0xb3,
	0x7a, 0xc0,
	0x7b, 0xcb,
	0x7c, 0xd5,
	0x7d, 0xde,
	0x7e, 0xe6,
	0x7f, 0xee,
	0x80, 0xf4,
	0x81, 0xfa,
	0x82, 0xff,

	0x03, 0x17,
	0xc4, 0x6e,
	0xc5, 0x5c,


	0x03, 0x18,
	0x12, 0x20,
	0x10, 0x07,
	0x11, 0x00,
	0x20, 0x02,
	0x21, 0x80,
	0x22, 0x00,
	0x23, 0xf0,
	0x24, 0x00,
	0x25, 0x06,
	0x26, 0x00,
	0x27, 0x00,
	0x28, 0x02,
	0x29, 0x86,
	0x2a, 0x00,
	0x2b, 0xf0,
	0x2c, 0x14,
	0x2d, 0x00,
	0x2e, 0x14,
	0x2f, 0x00,
	0x30, 0x65,
	0x03, 0x20,
	0x10, 0x1c,
	0x18, 0x38,
	0x20, 0x01,
	0x21, 0x30,
	0x22, 0x10,
	0x23, 0x00,
	0x24, 0x04,

	0x28, 0xff,
	0x29, 0xad,

	0x2a, 0xf0,
	0x2b, 0x34,
	0x30, 0x78,
	0x2c, 0xc3,
	0x2d, 0x5f,
	0x2e, 0x33,
	// 0x30, 0xf8,
	0x32, 0x03,
	0x33, 0x2e,
	0x34, 0x30,
	0x35, 0xd4,
	0x36, 0xfe,
	0x37, 0x32,
	0x38, 0x04,
	0x47, 0xf0,

	//Y_Frame TH
	0x50, 0x45,
	0x51, 0x88,

	0x56, 0x10,
	0x57, 0xb7,
	0x58, 0x14,
	0x59, 0x88,
	0x5a, 0x04,

	0x60, 0x55,
	0x61, 0x55,
	0x62, 0x6a,
	0x63, 0xa9,
	0x64, 0x6a,
	0x65, 0xa9,
	0x66, 0x6a,
	0x67, 0xa9,
	0x68, 0x6b,
	0x69, 0xe9,
	0x6a, 0x6a,
	0x6b, 0xa9,
	0x6c, 0x6a,
	0x6d, 0xa9,
	0x6e, 0x55,
	0x6f, 0x55,
	// 0x70, 0x42,
	0x70, 0x45,
	0x71, 0xBb,

	// haunting control
	0x76, 0x21,
	0x77, 0x02,
	0x78, 0x22,
	0x79, 0x2a,

	0x78, 0x24,
	0x79, 0x23,
	0x7a, 0x23,
	0x7b, 0x22,
	0x7d, 0x23,
	0x83, 0x01, //EXP Normal 33.33 fps
	0x84, 0x5f,
	0x85, 0x00,
	0x86, 0x01, //EXPMin 6410.26 fps
	0x87, 0xd4,
	0x88, 0x05, //EXP Max 8.33 fps
	0x89, 0x7c,
	0x8a, 0x00,
	0x8B, 0x75, //EXP100
	0x8C, 0x00,
	0x8D, 0x60, //EXP120
	0x8E, 0xe4,


	0x98, 0xdc,
	0x99, 0x45,
	0x9a, 0x0d,
	0x9b, 0xde,
	0x9c, 0x08,
	0x9d, 0x0a,
	0x9e, 0x01,
	0x10, 0x9c,
	0x18, 0x30,
	0x90, 0x0c,
	0x91, 0x0c,
	0x92, 0xd8,
	0x93, 0xd0,

	0x9c, 0x0c, //EXP Limit 915.75 fps
	0x9d, 0xcc,
	0x9e, 0x01, //EXP Unit
	0x9f, 0xd4,
	0xa0, 0x03,
	0xa1, 0xa9,
	0xa2, 0x80,
	0xb0, 0x1d,
	0xb1, 0x1a,
	0xb2, 0xb0,
	0xb3, 0x1a,
	0xb4, 0x1a,
	0xb5, 0x44,
	0xb6, 0x2f,
	0xb7, 0x28,
	0xb8, 0x25,
	0xb9, 0x22,
	0xba, 0x21,
	0xbb, 0x20,
	0xbc, 0x1f,
	0xbd, 0x1f,
	0xc0, 0x30,
	0xc1, 0x20,
	0xc2, 0x20,
	0xc3, 0x20,
	0xc4, 0x08,
	0xc8, 0x60,
	0xc9, 0x40,

	//Page 22
	0x03, 0x22, // Page 22
	0x10, 0xfd, // AWBCTL1
	0x11, 0x2e, // AWBCTL2
	0x19, 0x01,
	0x20, 0x30,
	0x21, 0x80,
	0x23, 0x08,
	0x24, 0x01,

	0x30, 0x80, // ULVL
	0x31, 0x80, // VLVL
	0x38, 0x11, // UVTH1
	0x39, 0x34, // UVTH2
	0x40, 0xf7, // YRANGE

	0x41, 0x77, // CDIFF
	0x42, 0x55, // CSUM2
	0x43, 0xf0,
	0x44, 0x66,
	0x45, 0x33,
	0x46, 0x01, // WHTPXLTH1
	0x47, 0x94,

	0x50, 0xb2,
	0x51, 0x81,
	0x52, 0x98,

	0x80, 0x38, // RGAIN
	0x81, 0x20, // GGAIN
	0x82, 0x38, // BGAIN

	0x83, 0x5e, // RMAX
	0x84, 0x20, // RMIN
	0x85, 0x53, // BMAX
	0x86, 0x15, // BMIN

	0x87, 0x54, // RMAXM
	0x88, 0x20, // RMINM
	0x89, 0x3f, // BMAXM
	0x8a, 0x1c, // BMINM

	0x8b, 0x54, // RMAXB
	0x8c, 0x3f, // RMINB
	0x8d, 0x24, // BMAXB
	0x8e, 0x1c, // BMINB

	0x8f, 0x60, // BGAINPARA1
	0x90, 0x5f, // BGAINPARA2
	0x91, 0x5c, // BGAINPARA3
	0x92, 0x4C, // BGAINPARA4
	0x93, 0x41, // BGAINPARA5
	0x94, 0x3b, // BGAINPARA6
	0x95, 0x36, // BGAINPARA7
	0x96, 0x30, // BGAINPARA8
	0x97, 0x27, // BGAINPARA9
	0x98, 0x20, // BGAINPARA10
	0x99, 0x1C, // BGAINPARA11
	0x9a, 0x19, // BGAINPARA12

	0x9b, 0x88, // BGAINBND1
	0x9c, 0x88, // BGAINBND2
	0x9d, 0x48, // RGAINTH1
	0x9e, 0x38, // RGAINTH2
	0x9f, 0x30, // RGAINTH3

	0xa0, 0x74, // RDELTA1
	0xa1, 0x35, // BDELTA1
	0xa2, 0xaf, // RDELTA2
	0xa3, 0xf7, // BDELTA2

	0xa4, 0x10, // AWBEXPLMT1
	0xa5, 0x50, // AWBEXPLMT2
	0xa6, 0xc4, // AWBEXPLMT3

	0xad, 0x40,
	0xae, 0x4a,

	0xaf, 0x2a,
	0xb0, 0x29,

	0xb1, 0x20,
	0xb4, 0xff,
	0xb8, 0x6b,
	0xb9, 0x00,


	0x03, 0x24,
	0x10, 0x01,
	0x18, 0x06,
	0x30, 0x06,
	0x31, 0x90,
	0x32, 0x25,
	0x33, 0xa2,
	0x34, 0x26,
	0x35, 0x58,
	0x36, 0x60,
	0x37, 0x00,
	0x38, 0x50,
	0x39, 0x00,

	0x03, 0x20,
	0x10, 0x9c,
	0x03, 0x22,
	0x10, 0xe9,

	//Page 00
	0x03, 0x00,
	0x0e, 0x03,
	0x0e, 0x73,

	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,
	0x03, 0x00,

	0x03, 0x00,
	0x01, 0xf8,

};

uint8_t hi253_default_regs_init[]={

0x01, 0xf9, //sleep on
0x08, 0x0f, //Hi-Z on
0x01, 0xf8, //sleep off
0x03, 0x00, // Dummy 750us START
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00, // Dummy 750us END
0x0e, 0x04, //PLL On
0x0e, 0x74, //PLLx2
0x03, 0x00, // Dummy 750us START
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00, // Dummy 750us END
0x0e, 0x00, //PLL off
0x01, 0xf1, //sleep on
0x08, 0x00, //Hi-Z off
0x01, 0xf3,
0x01, 0xf1,
// PAGE 20
0x03, 0x20, //page 20
0x10, 0x1c, //ae off
// PAGE 22
0x03, 0x22, //page 22
0x10, 0x69, //awb off
/////// PAGE 0 START ///////
0x03, 0x00,
0x10, 0x10, // Sub1/2_Preview2 Mode_H binning
0x11, 0x90,
0x12, 0x04,//00
0x0b, 0xaa, // ESD Check Register
0x0c, 0xaa, // ESD Check Register
0x0d, 0xaa, // ESD Check Register
0x20, 0x00, // Windowing start point Y
0x21, 0x04,
0x22, 0x00, // Windowing start point X
0x23, 0x07,
0x24, 0x04,
0x25, 0xb0,
0x26, 0x06,
0x27, 0x40, // WINROW END
0x40, 0x01, //Hblank 408
0x41, 0x68, 
0x42, 0x00, //Vblank 20
0x43, 0x14,
0x45, 0x04,
0x46, 0x18,
0x47, 0xd8,
//BLC
0x80, 0x2e,
0x81, 0x7e,
0x82, 0x90,
0x83, 0x00,
0x84, 0x0c,
0x85, 0x00,
0x90, 0x0c, //BLC_TIME_TH_ON
0x91, 0x0c, //BLC_TIME_TH_OFF 
0x92, 0x78, //BLC_AG_TH_ON
0x93, 0x70, //BLC_AG_TH_OFF
0x94, 0x75,
0x95, 0x70,
0x96, 0xdc,
0x97, 0xfe,
0x98, 0x38,
//OutDoor  BLC
0x99, 0x42,
0x9a, 0x42,
0x9b, 0x42,
0x9c, 0x42,
//Dark BLC
0xa0, 0x08,
0xa2, 0x08,
0xa4, 0x08,
0xa6, 0x08,
//Normal BLC
0xa8, 0x40,
0xaa, 0x40,
0xac, 0x40,
0xae, 0x40,
/////// PAGE 2 START ///////
0x03, 0x02, //Page 02
0x10, 0x00, //Mode_test
0x11, 0x00, //Mode_dead_test
0x12, 0x03, //pwr_ctl_ctl1
0x13, 0x03, //Mode_ana_test
0x14, 0x00, //mode_memory
0x16, 0x00, //dcdc_ctl1
0x17, 0x8c, //dcdc_ctl2
0x18, 0x4C, //analog_func1
0x19, 0x00, //analog_func2
0x1a, 0x39, //analog_func3
0x1b, 0x00, //analog_func4
0x1c, 0x09, //dcdc_ctl3
0x1d, 0x40, //dcdc_ctl4
0x1e, 0x30, //analog_func7
0x1f, 0x10, //analog_func8
0x20, 0x77, //pixel bias
0x21, 0xde, //adc,asp bias
0x22, 0xa7, //main,bus bias
0x23, 0x30, //clamp
0x24, 0x4a,		
0x25, 0x10,		
0x27, 0x3c,		
0x28, 0x00,		
0x29, 0x0c,		
0x2a, 0x80,		
0x2b, 0x80,		
0x2c, 0x02,		
0x2d, 0xa0,		
0x2e, 0x00,	//11	
0x2f, 0x00,	//a1	
0x30, 0x05, //swap_ctl
0x31, 0x99,		
0x32, 0x00,		
0x33, 0x00,		
0x34, 0x22,		
0x38, 0x88,		
0x39, 0x88,		
0x50, 0x20,		
0x51, 0x03,	//00	don't touch 
0x52, 0x01,		
0x53, 0xc1,		
0x54, 0x10,		
0x55, 0x1c,		
0x56, 0x11,		
0x58, 0x10,		
0x59, 0x0e,		
0x5d, 0xa2,
0x5e, 0x5a,
0x60, 0x87,
0x61, 0x99,
0x62, 0x88,
0x63, 0x97,
0x64, 0x88,
0x65, 0x97,
0x67, 0x0c,
0x68, 0x0c,
0x69, 0x0c,
0x6a, 0xb4,		
0x6b, 0xc4,		
0x6c, 0xb5,		
0x6d, 0xc2,		
0x6e, 0xb5,		
0x6f, 0xc0,		
0x70, 0xb6,		
0x71, 0xb8,		
0x72, 0x89,
0x73, 0x96,
0x74, 0x89,
0x75, 0x96,
0x76, 0x89,
0x77, 0x96,
0x7c, 0x85,
0x7d, 0xaf,
0x80, 0x01,
0x81, 0x7f,
0x82, 0x13,
0x83, 0x24,
0x84, 0x7d,
0x85, 0x81,
0x86, 0x7d,
0x87, 0x81,
0x88, 0xab,		
0x89, 0xbc,		
0x8a, 0xac,		
0x8b, 0xba,		
0x8c, 0xad,		
0x8d, 0xb8,		
0x8e, 0xae,		
0x8f, 0xb2,		
0x90, 0xb3,		
0x91, 0xb7,		
0x92, 0x48,
0x93, 0x54,
0x94, 0x7d,
0x95, 0x81,
0x96, 0x7d,
0x97, 0x81,
0xa0, 0x02,
0xa1, 0x7b,
0xa2, 0x02,
0xa3, 0x7b,
0xa4, 0x7b,
0xa5, 0x02,
0xa6, 0x7b,
0xa7, 0x02,
0xa8, 0x85,
0xa9, 0x8c,
0xaa, 0x85,
0xab, 0x8c,
0xac, 0x10,
0xad, 0x16,
0xae, 0x10,
0xaf, 0x16,
0xb0, 0x99,
0xb1, 0xa3,
0xb2, 0xa4,
0xb3, 0xae,
0xb4, 0x9b,
0xb5, 0xa2,
0xb6, 0xa6,
0xb7, 0xac,
0xb8, 0x9b,
0xb9, 0x9f,
0xba, 0xa6,
0xbb, 0xaa,
0xbc, 0x9b,
0xbd, 0x9f,
0xbe, 0xa6,
0xbf, 0xaa,
0xc4, 0x2c,
0xc5, 0x43,
0xc6, 0x63,
0xc7, 0x79,
0xc8, 0x2d,
0xc9, 0x42,
0xca, 0x2d,
0xcb, 0x42,
0xcc, 0x64,
0xcd, 0x78,
0xce, 0x64,
0xcf, 0x78,
0xd0, 0x0a,
0xd1, 0x09,
0xd2, 0x20,		
0xd3, 0x00,	
0xd4, 0x0c, //DCDC_TIME_TH_ON  don't touch
0xd5, 0x0c, //DCDC_TIME_TH_OFF don't touch
0xd6, 0x78, //DCDC_AG_TH_ON      78
0xd7, 0x70, //DCDC_AG_TH_OFF     70
0xe0, 0xc4,
0xe1, 0xc4,
0xe2, 0xc4,
0xe3, 0xc4,
0xe4, 0x00,
0xe8, 0x80,
0xe9, 0x40,
0xea, 0x7f,
0xf0, 0x01,   
0xf1, 0x01,
0xf2, 0x01,
0xf3, 0x01,
0xf4, 0x01,
/////// PAGE 3 ///////
0x03, 0x03,
0x10, 0x10,
/////// PAGE 10 START ///////
0x03, 0x10,
0x10, 0x03, // CrYCbY // For Demoset 0x03
0x12, 0x30,
0x13, 0x0a,//add jacky open constrast
0x20, 0x00,
0x30, 0x00,
0x31, 0x00,
0x32, 0x00,
0x33, 0x00,
0x34, 0x30,
0x35, 0x00,
0x36, 0x00,
0x38, 0x00,
0x3e, 0x58,
0x3f, 0x00,
0x40, 0x80, // YOFS
0x41, 0x00, // DYOFS
0x48, 0x80,// 84
0x60, 0x67,//6b AG ratio  jacky

0x61, 0x70, //7e //8e //88 //70
0x62, 0x70, //7e //8e //88 //80
0x63, 0x50, //Double_AG 50->30
0x64, 0x41,
0x66, 0x42,
0x67, 0x20,
0x6a, 0x80, //8a
0x6b, 0x84, //74
0x6c, 0x80, //7e //7a
0x6d, 0x80, //8e
/////// PAGE 11 START ///////
0x03, 0x11,
0x10, 0x7f,
0x11, 0x40,
0x12, 0x0a, // Blue Max-Filter Delete
0x13, 0xbb,
0x26, 0x51, // Double_AG 31->20
0x27, 0x54, // Double_AG 34->22
0x28, 0x0f,
0x29, 0x10,
0x2b, 0x30,
0x2c, 0x32,
//Out2 D-LPF th
0x30, 0x70,
0x31, 0x10,
0x32, 0x58,
0x33, 0x09,
0x34, 0x06,
0x35, 0x03,
//Out1 D-LPF th
0x36, 0x70,
0x37, 0x18,
0x38, 0x58,
0x39, 0x09,
0x3a, 0x06,
0x3b, 0x03,
//Indoor D-LPF th
0x3c, 0x80,
0x3d, 0x18,
	0x3e, 0x90,
0x3f, 0x0c,
0x40, 0x09,
0x41, 0x06,
0x42, 0x80,
0x43, 0x18,
	0x44, 0x90,
0x45, 0x12,
0x46, 0x10,
0x47, 0x10,
0x48, 0x90,
0x49, 0x40,
0x4a, 0x80,
0x4b, 0x13,
0x4c, 0x10,
0x4d, 0x11,
0x4e, 0x80,
0x4f, 0x30,
0x50, 0x80,
0x51, 0x13,
0x52, 0x10,
0x53, 0x13,
0x54, 0x11,
0x55, 0x17,
0x56, 0x20,
0x57, 0x01,
0x58, 0x00,
0x59, 0x00,
0x5a, 0x1f, //18
0x5b, 0x00,
0x5c, 0x00,
0x60, 0x3f,
0x62, 0x60,
0x70, 0x06,
/////// PAGE 12 START ///////
0x03, 0x12,
0x20, 0x0f,
0x21, 0x0f,
0x25, 0x00, //0x30
0x28, 0x00,
0x29, 0x00,
0x2a, 0x00,
0x30, 0x50,
0x31, 0x18,
0x32, 0x32,
0x33, 0x40,
0x34, 0x50,
0x35, 0x70,
0x36, 0xa0,
//Out2 th
0x40, 0xa0,
0x41, 0x40,
0x42, 0xa0,
0x43, 0x90,
0x44, 0x90,
0x45, 0x80,
//Out1 th
0x46, 0xb0,
0x47, 0x55,
0x48, 0xa0,
0x49, 0x90,
0x4a, 0x90,
0x4b, 0x80,
//Indoor th
0x4c, 0xb0,
0x4d, 0x40,
0x4e, 0x90,
0x4f, 0x90,
0x50, 0xa0,
0x51, 0x80,
//Dark1 th
0x52, 0x00, //b0
0x53, 0x50, //60
0x54, 0xd4, //c0
0x55, 0xc0, //c0
0x56, 0x70, //b0
0x57, 0xec, //70
//Dark2 th
0x58, 0x60, //90
0x59, 0x40, //
0x5a, 0xd0, 
0x5b, 0xd0, 
0x5c, 0xc0, 
0x5d, 0x9b, //70
//Dark3 th
0x5e, 0x70,//88
0x5f, 0x40,
0x60, 0xe0,
0x61, 0xe0,
0x62, 0xe0,
0x63, 0xb4,//80
0x70, 0x15,
0x71, 0x01, //Don't Touch register
0x72, 0x18,
0x73, 0x01, //Don't Touch register
0x74, 0x25,
0x75, 0x15,
0x80, 0x20,
0x81, 0x40,
0x82, 0x65,
0x85, 0x1a,
0x88, 0x00,
0x89, 0x00,
0x90, 0x5d, //For capture
0x91, 0x88,		
0x98, 0x7d,		
0x99, 0x28,		
0x9A, 0x14,		
0x9B, 0xc8,		
0x9C, 0x02,		
0x9D, 0x1e,		
0x9E, 0x28,		
0x9F, 0x07,		
0xA0, 0x32,		
0xA4, 0x04,		
0xA5, 0x0e,		
0xA6, 0x0c,		
0xA7, 0x04,		
0xA8, 0x3c,		

0xAA, 0x14,		
0xAB, 0x11,		
0xAC, 0x0f,		
0xAD, 0x16,		
0xAE, 0x15,		
0xAF, 0x14,		

0xB1, 0xaa,		
0xB2, 0x96,		
0xB3, 0x28,		
//Dont Touch register
0xB8, 0x78,		
0xB9, 0xa0,		
0xBA, 0xb4,		
0xBB, 0x14,		
0xBC, 0x14,		
0xBD, 0x00,	//14	 
0xBE, 0x64,		
0xBF, 0x64,		
0xC0, 0x64,		
0xC1, 0x64,		
0xC2, 0x04,		
0xC3, 0x03,		
0xC4, 0x0c,		
0xC5, 0x30,		
0xC6, 0x2a,		
0xD0, 0x0c, //CI Option/CI DPC
0xD1, 0x80,		
0xD2, 0x67,		
0xD3, 0x00,		
0xD4, 0x00,		
0xD5, 0x02,		
0xD6, 0xff,		
0xD7, 0x18,	
//End
0x3b, 0x06,
0x3c, 0x06,
0xc5, 0x00,//55->48
0xc6, 0x00,//48->40
/////// PAGE 13 START ///////
0x03, 0x13,
//Edge
0x10, 0xcb,
0x11, 0x7b,
0x12, 0x07,
0x14, 0x00,
0x20, 0x15,
0x21, 0x13,
0x22, 0x33,
0x23, 0x05,
0x24, 0x09,
0x26, 0x18,
0x27, 0x30,
0x29, 0x12,
0x2a, 0x50,
//Low clip th
0x2b, 0x01, //Out2 02
0x2c, 0x01, //Out1 02
0x25, 0x06,
0x2d, 0x0c,
0x2e, 0x12,
0x2f, 0x12,
//Out2 Edge
0x50, 0x18,
0x51, 0x1c,
0x52, 0x1b,
0x53, 0x15,
0x54, 0x18,
0x55, 0x15,
//Out1 Edge  
0x56, 0x18,
0x57, 0x1c,
0x58, 0x1b,
0x59, 0x15,
0x5a, 0x18,
0x5b, 0x15,
//Indoor Edge
0x5c, 0x0f,//0b
0x5d, 0x21,//21
0x5e, 0x1c,//0a
0x5f, 0x0c,//08
0x60, 0x2f,//19
0x61, 0x1c,//08
//Dark1 Edge
0x62, 0x08,
0x63, 0x28,
0x64, 0x08,
0x65, 0x06,
0x66, 0x26,
0x67, 0x06,
//Dark2 Edge
0x68, 0x07,
0x69, 0x07,
0x6a, 0x07,
0x6b, 0x05,
0x6c, 0x05,
0x6d, 0x05,
//Dark3 Edge
0x6e, 0x07,
0x6f, 0x07,
0x70, 0x07,
0x71, 0x05,
0x72, 0x05,
0x73, 0x05,
0x80, 0xfd,
0x81, 0x1f,
0x82, 0x05,
0x83, 0x31,
0x90, 0x05,
0x91, 0x05,
0x92, 0x33,
0x93, 0x30,
0x94, 0x03,
0x95, 0x14,
0x97, 0x20,
0x99, 0x20,
0xa0, 0x01,
0xa1, 0x02,
0xa2, 0x01,
0xa3, 0x02,
0xa4, 0x05,
0xa5, 0x05,
0xa6, 0x07,
0xa7, 0x08,
0xa8, 0x07,
0xa9, 0x08,
0xaa, 0x07,
0xab, 0x08,
0xb0, 0x22,
0xb1, 0x2a,
0xb2, 0x28,
0xb3, 0x22,
0xb4, 0x2a,
0xb5, 0x28,
0xb6, 0x22,
0xb7, 0x2a,
0xb8, 0x28,
0xb9, 0x22,
0xba, 0x2a,
0xbb, 0x28, 
0xbc, 0x25,
0xbd, 0x2a,
0xbe, 0x27,
0xbf, 0x25,
0xc0, 0x2a,
0xc1, 0x27,
0xc2, 0x1e,
0xc3, 0x24,
0xc4, 0x20,
0xc5, 0x1e,
0xc6, 0x24,
0xc7, 0x20,
0xc8, 0x18,
0xc9, 0x20,
0xca, 0x1e,
0xcb, 0x18,
0xcc, 0x20,
0xcd, 0x1e,
0xce, 0x18,
0xcf, 0x20,
0xd0, 0x1e,
0xd1, 0x18,
0xd2, 0x20,
0xd3, 0x1e,
/////// PAGE 14 START ///////
0x03, 0x14,
0x10, 0x11,
0x14, 0x80, // GX
0x15, 0x80, // GY
0x16, 0x80, // RX
0x17, 0x80, // RY
0x18, 0x80, // BX
0x19, 0x80, // BY
0x20, 0x80, //X 60 //a0
0x21, 0x80, //Y
0x22, 0x80,
0x23, 0x80,
0x24, 0x80,
0x30, 0xc8,
0x31, 0x2b,
0x32, 0x00,
0x33, 0x00,
0x34, 0x90,
0x40, 0x48, //0x48//65
0x50, 0x44, //0x34
0x60, 0x46, //0x29
0x70, 0x44, //0x34
/////// PAGE 15 START ///////
0x03, 0x15,
0x10, 0x0f,
//Rstep H 16
//Rstep L 14
0x14, 0x4f,	//CMCOFSGH 
0x15, 0x32,	//CMCOFSGM
0x16, 0x24,	//CMCOFSGL
0x17, 0x2f,	//CMC SIGN
//CMC
0x30, 0x91,
0x31, 0x6a,
0x32, 0x18,
0x33, 0x17,
0x34, 0x53,
0x35, 0x08,
0x36, 0x08,
0x37, 0x3e,
0x38, 0x87,
//CMC OFS
0x40, 0xa3,
0x41, 0x3a,
0x42, 0x93,
0x43, 0x0b,
0x44, 0x86,
0x45, 0x06,
0x46, 0x00,
0x47, 0xa2,
0x48, 0x22,
//CMC POFS
0x50, 0x82,
0x51, 0x06,
0x52, 0x83,
0x53, 0x03,
0x54, 0x11,
0x55, 0x8e,
0x56, 0x00,
0x57, 0x06,
0x58, 0x87,

0x80, 0x03,
0x85, 0x40,
0x87, 0x02,
0x88, 0x00,
0x89, 0x00,
0x8a, 0x00,



/////// PAGE 16 START ///////
0x03, 0x16,
0x10, 0x31,//GMA_CTL
0x18, 0x5e,//AG_ON
0x19, 0x5d,//AG_OFF
0x1A, 0x0e,//TIME_ON
0x1B, 0x01,//TIME_OFF
0x1C, 0xdc,//OUT_ON
0x1D, 0xfe,//OUT_OFF

//GMA
0x30, 0x00,
0x31, 0x06,
0x32, 0x0e,
0x33, 0x23,
0x34, 0x55,
0x35, 0x7f,
0x36, 0x99,
0x37, 0xac,
0x38, 0xba,
0x39, 0xc5,
0x3a, 0xcd,
0x3b, 0xd4,
0x3c, 0xdb,
0x3d, 0xe2,
0x3e, 0xe7,
0x3f, 0xec,
0x40, 0xf3,
0x41, 0xfa,
0x42, 0xff,
//RGMA
0x50, 0x00,
0x51, 0x07,
0x52, 0x12,
0x53, 0x27,
0x54, 0x57,
0x55, 0x7f,
0x56, 0x99,
0x57, 0xac,
0x58, 0xba,
0x59, 0xc5,
0x5a, 0xcd,
0x5b, 0xd4,
0x5c, 0xdb,
0x5d, 0xe2,
0x5e, 0xe7,
0x5f, 0xec,
0x60, 0xf3,
0x61, 0xfa,
0x62, 0xff,
//BGMA
0x70, 0x00,
0x71, 0x09,
0x72, 0x16,
0x73, 0x2c,
0x74, 0x59,
0x75, 0x7f,
0x76, 0x99,
0x77, 0xac,
0x78, 0xba,
0x79, 0xc5,
0x7a, 0xcd,
0x7b, 0xd4,
0x7c, 0xdb,
0x7d, 0xe2,
0x7e, 0xe7,
0x7f, 0xec,
0x80, 0xf3,
0x81, 0xfa,
0x82, 0xff,
/////// PAGE 17 START ///////
0x03, 0x17,
0x10, 0xf7,
/////// PAGE 18 START ///////

	0x03, 0x18,
	0x12, 0x20,
	0x10, 0x07,
	0x11, 0x00,
	0x20, 0x05,
	0x21, 0x00,
	0x22, 0x03,
	0x23, 0xc0,
	0x24, 0x00,
	0x25, 0x04,
	0x26, 0x00,
	0x27, 0x00,
	0x28, 0x05,
	0x29, 0x04,
	0x2a, 0x03,
	0x2b, 0xc0,
	0x2c, 0x0a,
	0x2d, 0x00,
	0x2e, 0x0a,
	0x2f, 0x00,
	0x30, 0x45,

/////// PAGE 20 START ///////
0x03, 0x20,
0x11, 0x1c,
0x18, 0x30,
0x1a, 0x08,
0x20, 0x01, //05_lowtemp Y Mean off
0x21, 0x30,
0x22, 0x10,
0x23, 0x00,
0x24, 0x00, //Uniform Scene Off
0x28, 0xe7,
0x29, 0x0d, //20100305 ad->0d
0x2a, 0xff,
0x2b, 0x34, //f4->Adaptive on //04 off
0x2c, 0xc2,
0x2d, 0xcf,  //fe->AE Speed option
0x2e, 0x33,
0x30, 0x78, //f8
0x32, 0x03,
0x33, 0x2e,
0x34, 0x30,
0x35, 0xd4,
0x36, 0xfe,
0x37, 0x32,
0x38, 0x04,
0x39, 0x22, //AE_escapeC10
0x3a, 0xde, //AE_escapeC11
0x3b, 0x22, //AE_escapeC1
0x3c, 0xde, //AE_escapeC2
0x50, 0x45,
0x51, 0x88,
0x56, 0x03,
0x57, 0xf7,
0x58, 0x14,
0x59, 0x88,
0x5a, 0x04,

/*
0x60, 0xff,
0x61, 0xff,
0x62, 0xea,
0x63, 0xab,
0x64, 0xea,
0x65, 0xab,
0x66, 0xea,//eb
0x67, 0x2b,//eb
0x68, 0xea,//eb
0x69, 0x2b,//eb
0x6a, 0xea,
0x6b, 0xab,
0x6c, 0xea,
0x6d, 0xab,
0x6e, 0xff,
0x6f, 0xff,
*/

0x60, 0x55,
0x61, 0x55,
0x62, 0x6a,
0x63, 0xa9,
0x64, 0x6a,
0x65, 0xa9,
0x66, 0x6b,//eb
0x67, 0xe9,//eb
0x68, 0x6b,//eb
0x69, 0xe9,//eb
0x6a, 0x6a,
0x6b, 0xa9,
0x6c, 0x6a,
0x6d, 0xa9,
0x6e, 0x55,
0x6f, 0x55,

0x70, 0x6e, //6e
0x71, 0x89, //00 //-4
0x76, 0x43,
0x77, 0xe2, //04 //f2
0x78, 0x23, //Yth1
0x79, 0x46, //Yth2 //46
0x7a, 0x23, //23
0x7b, 0x22, //22
0x7d, 0x23,
0x83, 0x03, //EXP Normal 33.33 fps 
0x84, 0x6e, 
0x85, 0xe8, 
0x86, 0x01, //EXPMin 7500.00 fps
0x87, 0xf4, 
0x88, 0x06, //EXP Max 8.33 fps 
0x89, 0xdd, 
0x8a, 0xd0, 
0x8B, 0x92, //EXP100 
0x8C, 0x7c, 
0x8D, 0x7a, //EXP120 
0x8E, 0x12, 
0x9c, 0x17, //EXP Limit 500.00 fps 
0x9d, 0x70, 
0x9e, 0x01, //EXP Unit 
0x9f, 0xf4, 
0xb0, 0x18,
0xb1, 0x14, //ADC 400->560
0xb2, 0x80,//a0
0xb3, 0x18,
0xb4, 0x1a,
0xb5, 0x44,
0xb6, 0x2f,
0xb7, 0x28,
0xb8, 0x25,
0xb9, 0x22,
0xba, 0x21,
0xbb, 0x20,
0xbc, 0x1f,
0xbd, 0x1f,
0xc0, 0x14,
0xc1, 0x1f,
0xc2, 0x1f,
0xc3, 0x18, //2b
0xc4, 0x10, //08
0xc8, 0x80,
0xc9, 0x40,
/////// PAGE 22 START ///////
0x03, 0x22,
0x10, 0xfd,
0x11, 0x2e,
0x19, 0x01,
0x20, 0x30,
0x21, 0x80,
0x24, 0x01,
0x30, 0x80,
0x31, 0x80,
0x38, 0x11,
0x39, 0x34,
0x40, 0xf7,
0x41, 0x55,
0x42, 0x33, //43

0x43, 0xf6,
0x44, 0x55, //44
0x45, 0x44, //33

0x46, 0x00,
0x50, 0xb2,
0x51, 0x81,
0x52, 0x98,
0x80, 0x38,//40
0x81, 0x20,
0x82, 0x30,

0x83, 0x53,
0x84, 0x16,//1E
0x85, 0x5A,//52
0x86, 0x25,

0x87, 0x45,
0x88, 0x35,
0x89, 0x47,
0x8a, 0x2b,

0x8b, 0x40,
0x8c, 0x33,
0x8d, 0x38,
0x8e, 0x30,

0x8f, 0x53,
0x90, 0x52,
0x91, 0x50,
0x92, 0x4c,
0x93, 0x43,
0x94, 0x37,
0x95, 0x2f,//2a
0x96, 0x26,//24
0x97, 0x21,//20
0x98, 0x1e,
0x99, 0x1f,
0x9a, 0x20,
0x9b, 0x88,
0x9c, 0x88,
0x9d, 0x48,
0x9e, 0x38,
0x9f, 0x30,
0xa0, 0x60,
0xa1, 0x34,
0xa2, 0x6f,
0xa3, 0xff,
0xa4, 0x14,
0xa5, 0x2c,
0xa6, 0xcf,
0xad, 0x40,
0xae, 0x4a,
0xaf, 0x28,
0xb0, 0x26,
0xb1, 0x00,
0xb8, 0xa0,
0xb9, 0x00,
// PAGE 20
0x03, 0x20, //page 20
0x10, 0x9c, //ae off
// PAGE 22
0x03, 0x22, //page 22
0x10, 0xe9, //awb off
// PAGE 0
0x03, 0x00,
0x0e, 0x04, //PLL On
0x0e, 0x74, //PLLx2
0x03, 0x00, // Dummy 750us
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
0x03, 0x00,
	0x03, 0x00,
	0x01, 0xf8,
 
};

#endif

