#ifndef  __TUOLUOIYI_H
#define __TUOLUOIYI_H
#include "public.h"

/* Exported types ------------------------------------------------------------*/
typedef struct tuoluoyi_data{
	
	float Xacc;
	float Yacc;
	float Zacc;
	float RollRate;
	float PitchRate;
	float YawRate;
} tuoluoyi_data_t;

typedef struct {
	float Xv;
	float Yv;
	float Zv;
	float Xl;
	float Yl;
	float Zl;
	float roll;
	float pitch;
	float yaw;
	float Xl_pre;
	float Yl_pre;
	float yaw_pre;
} tuoluoyi_info_t;


typedef volatile struct {
	u8 zero_flag;    //标志零飘是否已计算
	u8 clear_flag;   //标志陀螺仪数据是否被相机处理子程序清零
} tuoluoyi_status_t;

/* Exported constants --------------------------------------------------------*/

#define TUOLUOYI_BUF_LEN  			200	//定义最大接收字节数 200
#define T  0.05             //陀螺仪采样周期
#define TUOLUOYI_PAKLEN  33


/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  TUOLUOYI_RX_BUF[TUOLUOYI_BUF_LEN];
extern int Tuoluoyi_rx_sta;
extern tuoluoyi_status_t tuoluoyi_status;
extern tuoluoyi_info_t  tuoluoyiinfo;

/* Exported functions ------------------------------------------------------- */
void tuoluoyi_process(void);
#endif //__TUOLUOIYI_H

