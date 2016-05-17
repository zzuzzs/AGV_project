#ifndef __CAMERA_H
#define __CAMERA_H

#include "public.h"

#define CAMERA_BUF_LEN  			200	//定义最大接收字节数 200
#define CAMERA_PAKLEN  27
#define LEN_PER_PIC     0.375  //画面每一像素代表的实际距离（待标定）


extern u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];
extern int Camera_rx_sta;

camera_data_t* camera_data_tan(u8 * data,u8 st,u8 len);
#endif  //__CAMERA_H
