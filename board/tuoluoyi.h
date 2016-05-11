#ifndef  __TUOLUOIYI_H
#define __TUOLUOIYI_H
#include "public.h"

#define TUOLUOYI_BUF_LEN  			200	//定义最大接收字节数 200
#define T  0.05             //陀螺仪采样周期
#define  PI 3.1415926
#define TUOLUOYI_PAKLEN  33

extern u8  TUOLUOYI_RX_BUF[TUOLUOYI_BUF_LEN];
extern int Tuoluoyi_rx_sta;

void Tuoluoyi_data_tan(u8 * rxdata,  u8 st, u8 len);
void tuoluoyi_process(void);
#endif //__TUOLUOIYI_H

