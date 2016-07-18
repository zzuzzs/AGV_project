#ifndef  __TUOLUOIYI_H
#define __TUOLUOIYI_H
#include "public.h"

#define GYRO_BUF_LEN  			200	//定义最大接收字节数 200
#define T  0.05             //陀螺仪采样周期
#define GYRO_PAKLEN  33

extern u8  GYRO_RX_BUF[GYRO_BUF_LEN];
extern int Gyro_rx_sta;

void Gyro_data_tan(u8 * rxdata,  u8 st, u8 len);
void gyro_process(void);
#endif //__TUOLUOIYI_H

