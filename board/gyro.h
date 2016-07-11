#ifndef  __TUOLUOIYI_H
#define __TUOLUOIYI_H
#include "public.h"

/* Exported types ------------------------------------------------------------*/
typedef struct Gyro_data{
	
//	float Xacc;
//	float Yacc;
//	float Zacc;
//	float RollRate;
//	float PitchRate;
	float YawRate;
	
	float YawRate_kalman;			//陀螺仪角速度卡尔曼滤波
} Gyro_data_t;

typedef struct {
//	float Xv;
//	float Yv;
//	float Zv;
//	float roll;
//	float pitch;
	float yaw;
} Gyro_info_t;


/* Exported constants --------------------------------------------------------*/

#define GYRO_BUF_LEN  			200	//定义最大接收字节数 200
#define T  0.0125             //陀螺仪采样周期
#define GYRO_PAKLEN  33


/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  GYRO_RX_BUF[GYRO_BUF_LEN];
extern int Gyro_rx_sta;
extern Gyro_info_t  Gyroinfo;

/* Exported functions ------------------------------------------------------- */
void Gyro_process(void);
void Gyro_kalman_init(void);
#endif //__TUOLUOIYI_H

