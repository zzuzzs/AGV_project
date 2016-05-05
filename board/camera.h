#ifndef __CAMERA_H
#define __CAMERA_H

#include "public.h"

#define USART_REC_LEN  			200	//定义最大接收字节数 200

#define PAKLEN  27



extern camera_data_t cameradata;
extern char tmp[PAKLEN] ;

extern void camera_data_tan(char * data,u8 st,u8 len);
#endif  //__CAMERA_H
