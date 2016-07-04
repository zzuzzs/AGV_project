#ifndef __MOTOR_H
#define __MOTOR_H
#include "public.h"


/* Exported types ------------------------------------------------------------*/
enum WHICH_MOTOR {
	LEFT_MOTOR = 1,
	RIGHT_MOTOR,
	UP_DOWN_MOTOR,	
	ROTATION_MOTOR
};

enum CW_CCW{
	CW = 1,
	CCW,
};

enum LEFT_OR_RIGHT{
	LEFT = 1,
	RIGHT = 2,
};

enum AGV_UPDATA_STATUS{
	DRGREE_UPDATA_WRITING = 1,
	LEN_UPDATA_WRITING = 2,
};

/* Exported constants --------------------------------------------------------*/
#define  NOMBER_OF_TURNS_PRE_VOLTAGE_PRE_SECODE 0.2926   //外部调速电压与转速之间的系数:当驱动电压为1v时，电机每秒钟的转数

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void motor_speed_set(u8 WHICH_MOTOR,float Speed);
void motor_run(u8 WHICH_MOTOR,u8 CW_CCW);
void motor_stop(u8 WHICH_MOTOR);


#endif //__MOTOR_H
