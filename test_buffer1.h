//segmental select and it`s response

unsigned char smallbuf1[] = { //select
	0x00, 0xa5
};//2
uint32_t buf_smallbuf1_len = sizeof smallbuf1 / sizeof smallbuf1[0];

unsigned char smallbuf2[] = {
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00
};//165
uint32_t buf_smallbuf2_len = sizeof smallbuf2 / sizeof smallbuf2[0];


unsigned char smallbuf3[] = {
	0x00, 0x00, 
	0x11, 0x69, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x03, 0x5e, 0x11, 0x61, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x22, 0x00, 0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x22, 0x73, 0x65, 0x6c, 
	0x65, 0x63, 0x74, 0x20, 0x2a, 0x20, 0x66, 0x72, 
	0x6f, 0x6d, 0x20, 0x65, 0x6d, 0x70, 0x6c, 0x6f, 
	0x79, 0x65, 0x65, 0x20, 0x77, 0x68, 0x65, 0x72, 
	0x65, 0x20, 0x31, 0x20, 0x3d, 0x20, 0x31, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00 
};//165
uint32_t buf_smallbuf3_len = sizeof smallbuf3 / sizeof smallbuf3[0];

unsigned char smallbuf4[] = {//data - select respose
	0x01, 0xab, 
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x17, 0x40, 0xd1, 0xe4, 0x76, 0xa6, 0x91, 
	0xc8, 0xed, 0x39, 0x1d, 0x5c, 0x1e, 0x58, 0x37, 
	0x14, 0x3a, 0x78, 0x73, 0x01, 0x01, 0x05, 0x08, 
	0x2b, 0x4d, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x00, 0x51, 0x01, 0x80, 0x00, 0x00, 0x0f, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xb2, 0x00, 0x01, 0x0f, 0x00, 0x00, 0x00, 0x01, 
	0x09, 0x09, 0x00, 0x00, 0x00, 0x09, 0x46, 0x49, 
	0x52, 0x53, 0x54, 0x4e, 0x41, 0x4d, 0x45, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x80, 0x00, 0x00, 0x14, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 
	0x00, 0x01, 0x14, 0x00, 0x00, 0x00, 0x01, 0x08, 
	0x08, 0x00, 0x00, 0x00, 0x08, 0x4c, 0x41, 0x53, 
	0x54, 0x4e, 0x41, 0x4d, 0x45, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 
	0x00, 0x03, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 
	0x00, 0x00, 0x03, 0x41, 0x47, 0x45, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
	0x01, 0x80, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 
	0x01, 0x14, 0x00, 0x00, 0x00, 0x01, 0x04, 0x04, 
	0x00, 0x00, 0x00, 0x04, 0x43, 0x49, 0x54, 0x59, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x78, 
	0x73, 0x01, 0x01, 0x05, 0x08, 0x2b, 0x01, 0x00, 
	0x00, 0x00, 0xe8, 0x1f, 0x00, 0x00, 0x1a, 0x00, 
	0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x06, 0x22, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x05, 0x7a, 0x68, 
	0x61, 0x6e, 0x67, 0x05, 0x72, 0x75, 0x6e, 0x7a, 
	0x69, 0x02, 0xc1, 0x19, 0x07, 0x62, 0x65, 0x69, 
	0x6a, 0x69, 0x6e, 0x67, 0x08, 0x06, 0x00, 0x44, 
	0xde, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x02, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xd7, 0x6d, 0x01, 0x00, 0x01, 0x00, 
	0x00, 0x61, 0x8c, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00 
};//427
uint32_t buf_smallbuf4_len = sizeof smallbuf4 / sizeof smallbuf4[0];
/*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*****************************************************************************
*/
unsigned char buf1[] = { //connect
	0x01, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x39, 0x01, 0x2c, 0x00, 0x00, 0x08, 0x00, 
	0x7f, 0xff, 0xc6, 0x0e, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0xce, 0x00, 0x3a, 0x00, 0x00, 0x02, 0x00, 
	0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x28, 0x44, 0x45, 0x53, 0x43, 0x52, 
	0x49, 0x50, 0x54, 0x49, 0x4f, 0x4e, 0x3d, 0x28, 
	0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x3d, 
	0x28, 0x50, 0x52, 0x4f, 0x54, 0x4f, 0x43, 0x4f, 
	0x4c, 0x3d, 0x74, 0x63, 0x70, 0x29, 0x28, 0x48, 
	0x4f, 0x53, 0x54, 0x3d, 0x31, 0x39, 0x32, 0x2e, 
	0x31, 0x36, 0x38, 0x2e, 0x38, 0x2e, 0x35, 0x32, 
	0x29, 0x28, 0x50, 0x4f, 0x52, 0x54, 0x3d, 0x31, 
	0x35, 0x32, 0x31, 0x29, 0x29, 0x28, 0x43, 0x4f, 
	0x4e, 0x4e, 0x45, 0x43, 0x54, 0x5f, 0x44, 0x41, 
	0x54, 0x41, 0x3d, 0x28, 0x53, 0x45, 0x52, 0x56, 
	0x49, 0x43, 0x45, 0x5f, 0x4e, 0x41, 0x4d, 0x45, 
	0x3d, 0x6f, 0x61, 0x73, 0x69, 0x73, 0x44, 0x42, 
	0x31, 0x32, 0x63, 0x29, 0x28, 0x43, 0x49, 0x44, 
	0x3d, 0x28, 0x50, 0x52, 0x4f, 0x47, 0x52, 0x41, 
	0x4d, 0x3d, 0x45, 0x3a, 0x5c, 0x3f, 0x3f, 0x3f, 
	0x3f, 0x5c, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x5c, 0x4f, 0x72, 0x61, 0x63, 0x6c, 0x65, 0x3f, 
	0x3f, 0x3f, 0x3f, 0x5c, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x3f, 0x3f, 0x5c, 0x77, 0x69, 0x6e, 0x64, 0x6f, 
	0x77, 0x73, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x5c, 0x31, 0x30, 0x5c, 0x73, 0x71, 0x6c, 0x70, 
	0x6c, 0x75, 0x73, 0x2e, 0x65, 0x78, 0x65, 0x29, 
	0x28, 0x48, 0x4f, 0x53, 0x54, 0x3d, 0x44, 0x44, 
	0x44, 0x29, 0x28, 0x55, 0x53, 0x45, 0x52, 0x3d, 
	0x64, 0x65, 0x6c, 0x6c, 0x29, 0x29, 0x29, 0x29 
};//264
unsigned char buf2[] = { //resend
	0x00, 0x08, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00 
};//8
unsigned char buf3[] = { //connect
	0x01, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x39, 0x01, 0x2c, 0x00, 0x00, 0x08, 0x00, 
	0x7f, 0xff, 0xc6, 0x0e, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0xce, 0x00, 0x3a, 0x00, 0x00, 0x02, 0x00, 
	0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x28, 0x44, 0x45, 0x53, 0x43, 0x52, 
	0x49, 0x50, 0x54, 0x49, 0x4f, 0x4e, 0x3d, 0x28, 
	0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x3d, 
	0x28, 0x50, 0x52, 0x4f, 0x54, 0x4f, 0x43, 0x4f, 
	0x4c, 0x3d, 0x74, 0x63, 0x70, 0x29, 0x28, 0x48, 
	0x4f, 0x53, 0x54, 0x3d, 0x31, 0x39, 0x32, 0x2e, 
	0x31, 0x36, 0x38, 0x2e, 0x38, 0x2e, 0x35, 0x32, 
	0x29, 0x28, 0x50, 0x4f, 0x52, 0x54, 0x3d, 0x31, 
	0x35, 0x32, 0x31, 0x29, 0x29, 0x28, 0x43, 0x4f, 
	0x4e, 0x4e, 0x45, 0x43, 0x54, 0x5f, 0x44, 0x41, 
	0x54, 0x41, 0x3d, 0x28, 0x53, 0x45, 0x52, 0x56, 
	0x49, 0x43, 0x45, 0x5f, 0x4e, 0x41, 0x4d, 0x45, 
	0x3d, 0x6f, 0x61, 0x73, 0x69, 0x73, 0x44, 0x42, 
	0x31, 0x32, 0x63, 0x29, 0x28, 0x43, 0x49, 0x44, 
	0x3d, 0x28, 0x50, 0x52, 0x4f, 0x47, 0x52, 0x41, 
	0x4d, 0x3d, 0x45, 0x3a, 0x5c, 0x3f, 0x3f, 0x3f, 
	0x3f, 0x5c, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x5c, 0x4f, 0x72, 0x61, 0x63, 0x6c, 0x65, 0x3f, 
	0x3f, 0x3f, 0x3f, 0x5c, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x3f, 0x3f, 0x5c, 0x77, 0x69, 0x6e, 0x64, 0x6f, 
	0x77, 0x73, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x5c, 0x31, 0x30, 0x5c, 0x73, 0x71, 0x6c, 0x70, 
	0x6c, 0x75, 0x73, 0x2e, 0x65, 0x78, 0x65, 0x29, 
	0x28, 0x48, 0x4f, 0x53, 0x54, 0x3d, 0x44, 0x44, 
	0x44, 0x29, 0x28, 0x55, 0x53, 0x45, 0x52, 0x3d, 
	0x64, 0x65, 0x6c, 0x6c, 0x29, 0x29, 0x29, 0x29 
};//264
unsigned char buf4[] = { //accept
	0x00, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
	0x01, 0x39, 0x00, 0x01, 0x08, 0x00, 0x7f, 0xff, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x41, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};//32
unsigned char buf5[] = { //data - bad - request
	0x00, 0x9c, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xde, 0xad, 0xbe, 0xef, 0x00, 0x92, 
	0x0a, 0x20, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x05, 0x0a, 0x20, 0x04, 0x00, 0x00, 
	0x08, 0x00, 0x01, 0x00, 0x01, 0xc6, 0x9c, 0x0e, 
	0xf9, 0xd2, 0x14, 0x00, 0x12, 0x00, 0x01, 0xde, 
	0xad, 0xbe, 0xef, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x05, 0x0a, 0x20, 0x04, 
	0x00, 0x00, 0x02, 0x00, 0x03, 0xe0, 0xe1, 0x00, 
	0x02, 0x00, 0x06, 0xfc, 0xff, 0x00, 0x02, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
	0x05, 0x0a, 0x20, 0x04, 0x00, 0x00, 0x0c, 0x00, 
	0x01, 0x00, 0x11, 0x06, 0x10, 0x0c, 0x0f, 0x0a, 
	0x0b, 0x08, 0x02, 0x01, 0x03, 0x00, 0x03, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
	0x05, 0x0a, 0x20, 0x04, 0x00, 0x00, 0x03, 0x00, 
	0x01, 0x00, 0x03, 0x01 
};//156
unsigned char buf6[] = { //data - bad - response
	0x00, 0x7f, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xde, 0xad, 0xbe, 0xef, 0x00, 0x75, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x05, 0x0c, 0x10, 0x02, 0x00, 0x00, 
	0x02, 0x00, 0x06, 0x00, 0x1f, 0x00, 0x0e, 0x00, 
	0x01, 0xde, 0xad, 0xbe, 0xef, 0x00, 0x03, 0x00, 
	0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x05, 0x0c, 0x10, 0x02, 0x00, 0x00, 
	0x02, 0x00, 0x06, 0xfb, 0xff, 0x00, 0x02, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
	0x05, 0x0c, 0x10, 0x02, 0x00, 0x00, 0x01, 0x00, 
	0x02, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x04, 0x00, 0x05, 0x0c, 0x10, 
	0x02, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00 
};//127
unsigned char buf7[] = {//sys - request
	0x00, 0xc5, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0x76, 0x02, 0x01, 0x03, 0x00, 
	0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x05, 
	0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x73, 0x79, 
	0x73, 0x0d, 0x00, 0x00, 0x00, 0x0d, 0x41, 0x55, 
	0x54, 0x48, 0x5f, 0x54, 0x45, 0x52, 0x4d, 0x49, 
	0x4e, 0x41, 0x4c, 0x03, 0x00, 0x00, 0x00, 0x03, 
	0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x0f, 
	0x00, 0x00, 0x00, 0x0f, 0x41, 0x55, 0x54, 0x48, 
	0x5f, 0x50, 0x52, 0x4f, 0x47, 0x52, 0x41, 0x4d, 
	0x5f, 0x4e, 0x4d, 0x0b, 0x00, 0x00, 0x00, 0x0b, 
	0x6e, 0x61, 0x76, 0x69, 0x63, 0x61, 0x74, 0x2e, 
	0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x0c, 
	0x00, 0x00, 0x00, 0x0c, 0x41, 0x55, 0x54, 0x48, 
	0x5f, 0x4d, 0x41, 0x43, 0x48, 0x49, 0x4e, 0x45, 
	0x07, 0x00, 0x00, 0x00, 0x07, 0x44, 0x53, 0x50, 
	0x5c, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x08, 0x41, 0x55, 0x54, 
	0x48, 0x5f, 0x50, 0x49, 0x44, 0x0d, 0x00, 0x00, 
	0x00, 0x0d, 0x31, 0x32, 0x35, 0x31, 0x38, 0x30, 
	0x3a, 0x31, 0x32, 0x36, 0x32, 0x38, 0x30, 0x00, 
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 
	0x41, 0x55, 0x54, 0x48, 0x5f, 0x53, 0x49, 0x44, 
	0x04, 0x00, 0x00, 0x00, 0x04, 0x64, 0x65, 0x6c, 
	0x6c, 0x00, 0x00, 0x00, 0x00 
};//197
unsigned char buf8[] = { //bad data
	0x00, 0xbd, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x06, 0x00, 0x78, 0x38, 0x36, 
	0x5f, 0x36, 0x34, 0x2f, 0x4c, 0x69, 0x6e, 0x75, 
	0x78, 0x20, 0x32, 0x2e, 0x34, 0x2e, 0x78, 0x78, 
	0x00, 0xb2, 0x00, 0x01, 0x00, 0x00, 0x00, 0x64, 
	0x00, 0x00, 0x00, 0x60, 0x01, 0x24, 0x0f, 0x05, 
	0x0b, 0x0c, 0x03, 0x0c, 0x0c, 0x05, 0x04, 0x05, 
	0x0d, 0x06, 0x09, 0x07, 0x08, 0x05, 0x05, 0x05, 
	0x05, 0x05, 0x0f, 0x05, 0x05, 0x05, 0x05, 0x05, 
	0x0a, 0x05, 0x05, 0x05, 0x05, 0x05, 0x04, 0x05, 
	0x06, 0x07, 0x08, 0x08, 0x23, 0x47, 0x23, 0x47, 
	0x08, 0x11, 0x23, 0x08, 0x11, 0x41, 0xb0, 0x47, 
	0x00, 0x83, 0x00, 0xb2, 0x07, 0xd0, 0x03, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x28, 0x06, 0x01, 0x01, 
	0x01, 0x2f, 0x01, 0x01, 0x07, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01, 0x7f, 0xff, 0x03, 0x0b, 
	0x03, 0x03, 0x01, 0x01, 0xff, 0x01, 0xff, 0xff, 
	0x01, 0x0a, 0x01, 0x01, 0xff, 0x01, 0x06, 0x09, 
	0x60, 0x01, 0x7f, 0x04, 0x00, 0x07, 0x02, 0x01, 
	0x00, 0x01, 0x18, 0x00, 0x03 
};//189
unsigned char buf9[] = { //select
	0x00, 0xa5, 
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x11, 0x69, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x03, 0x5e, 0x11, 0x61, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x22, 0x00, 0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x22, 0x73, 0x65, 0x6c, 
	0x65, 0x63, 0x74, 0x20, 0x2a, 0x20, 0x66, 0x72, 
	0x6f, 0x6d, 0x20, 0x65, 0x6d, 0x70, 0x6c, 0x6f, 
	0x79, 0x65, 0x65, 0x20, 0x77, 0x68, 0x65, 0x72, 
	0x65, 0x20, 0x31, 0x20, 0x3d, 0x20, 0x31, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00 
};//165
unsigned char buf10[] = {//data - select respose
	0x01, 0xab, 
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x17, 0x40, 0xd1, 0xe4, 0x76, 0xa6, 0x91, 
	0xc8, 0xed, 0x39, 0x1d, 0x5c, 0x1e, 0x58, 0x37, 
	0x14, 0x3a, 0x78, 0x73, 0x01, 0x01, 0x05, 0x08, 
	0x2b, 0x4d, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x00, 0x51, 0x01, 0x80, 0x00, 0x00, 0x0f, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xb2, 0x00, 0x01, 0x0f, 0x00, 0x00, 0x00, 0x01, 
	0x09, 0x09, 0x00, 0x00, 0x00, 0x09, 0x46, 0x49, 
	0x52, 0x53, 0x54, 0x4e, 0x41, 0x4d, 0x45, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x80, 0x00, 0x00, 0x14, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 
	0x00, 0x01, 0x14, 0x00, 0x00, 0x00, 0x01, 0x08, 
	0x08, 0x00, 0x00, 0x00, 0x08, 0x4c, 0x41, 0x53, 
	0x54, 0x4e, 0x41, 0x4d, 0x45, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 
	0x00, 0x03, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 
	0x00, 0x00, 0x03, 0x41, 0x47, 0x45, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
	0x01, 0x80, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 
	0x01, 0x14, 0x00, 0x00, 0x00, 0x01, 0x04, 0x04, 
	0x00, 0x00, 0x00, 0x04, 0x43, 0x49, 0x54, 0x59, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x78, 
	0x73, 0x01, 0x01, 0x05, 0x08, 0x2b, 0x01, 0x00, 
	0x00, 0x00, 0xe8, 0x1f, 0x00, 0x00, 0x1a, 0x00, 
	0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x06, 0x22, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x05, 0x7a, 0x68, 
	0x61, 0x6e, 0x67, 0x05, 0x72, 0x75, 0x6e, 0x7a, 
	0x69, 0x02, 0xc1, 0x19, 0x07, 0x62, 0x65, 0x69, 
	0x6a, 0x69, 0x6e, 0x67, 0x08, 0x06, 0x00, 0x44, 
	0xde, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x02, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xd7, 0x6d, 0x01, 0x00, 0x01, 0x00, 
	0x00, 0x61, 0x8c, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00 
};//427
unsigned char buf11[] = {//data truncate packet
	0x00, 0x9a, 
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x11, 0x69, 0x35, 0x01, 0x01, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x03, 0x5e, 0x36, 0x21, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x17, 0x00, 0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x17, 0x74, 0x72, 0x75, 
	0x6e, 0x63, 0x61, 0x74, 0x65, 0x20, 0x74, 0x61, 
	0x62, 0x6c, 0x65, 0x20, 0x65, 0x6d, 0x70, 0x6c, 
	0x6f, 0x79, 0x65, 0x65, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  
};//154

unsigned char *buf[] = {buf1, buf2, buf3, buf4, buf5, buf6, buf7, buf8, buf9, buf10, buf11}; 
uint32_t buf_len[BUFFER_NUM] = {264, 8, 264, 32, 156, 127, 197, 189, 165, 427, 154};






























