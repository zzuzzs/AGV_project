#ifndef __MOTOR_H
#define __MOTOR_H
#include "public.h"

#define D_MOROR		0.16		//轮子直径,单位m
#define CNT_PRE_MIN_PRE_V 625  //外部调速电压与转速之间的系数
#define LEN_WHELLS  1   //轮间距,单位m

#define  V_REF 5   //DAC 参考电压

enum {
	LEFT_MOTOR = 1,
	RIGHT_MOTOR,
	UP_DOWN_MOTOR,	
	ROTATION_MOTOR
} WHICH_MOTOR;

enum {
	CW = 1,
	CCW,
} CW_CCW;

enum {
	LEFT,
	RIGHT,
} LEFT_OR_RIGHT;

#endif //__MOTOR_H
