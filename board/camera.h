#ifndef __CAMERA_H
#define __CAMERA_H

#include "public.h"

#define CAMERA_BUF_LEN  			200	//�����������ֽ��� 200
#define CAMERA_PAKLEN  27

extern u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];
extern int Camera_rx_sta;

camera_data_t* camera_data_tan(u8 * data,u8 st,u8 len);
#endif  //__CAMERA_H
