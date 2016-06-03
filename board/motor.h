#ifndef __MOTOR_H
#define __MOTOR_H
#include "public.h"

#define CNT_PRE_MIN_PRE_V 625  //外部调速电压与转速之间的系数

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
	LEFT,
	RIGHT,
};

enum AGV_UPDATA_STATUS{
	DRGREE_UPDATA_WRITING = 1,
	LEN_UPDATA_WRITING = 2,
};


void AGV_control(AGV_control_t * AGV_control_data_p);

void AGV_run(void);
void motor_speed_set(u8 WHICH_MOTOR,float Speed);
void AGV_stop(void);

#endif //__MOTOR_H
