#ifndef __MOTOR_H
#define __MOTOR_H
#include "public.h"

#define D_MOROR		0.16		//����ֱ��,��λm
#define CNT_PRE_MIN_PRE_V 625  //�ⲿ���ٵ�ѹ��ת��֮���ϵ��
#define LEN_WHELLS  1   //�ּ��,��λm

#define  V_REF 5   //DAC �ο���ѹ

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
