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
	float roll;
	float pitch;
	float yaw;
} tuoluoyi_info_t;


typedef  struct {
	u8 zero_flag;    //标志零飘是否已计算
	float Degree;			//陀螺仪积分出的小车姿态角
	float Degree_kalman;			//陀螺仪卡尔曼滤波的小车姿态角
} tuoluoyi_status_t;


/* Exported constants --------------------------------------------------------*/

#define TUOLUOYI_BUF_LEN  			200	//定义最大接收字节数 200
#define T  0.01             //陀螺仪采样周期
#define TUOLUOYI_PAKLEN  33


/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  TUOLUOYI_RX_BUF[TUOLUOYI_BUF_LEN];
extern int Tuoluoyi_rx_sta;
extern tuoluoyi_status_t tuoluoyi_status;
extern tuoluoyi_info_t  tuoluoyiinfo;

/* Exported functions ------------------------------------------------------- */
void tuoluoyi_process(void);
void tuoluoyi_kalman_init(void);
#endif //__TUOLUOIYI_H

