#include "public.h"
#include "motor.h"
#include "board.h"

static void DAC_8551_set(SPI_TypeDef* SPIx,float Voltage)
{
	int cnt = 0;
	cnt = Voltage * 0xffff / V_REF;
	SPI_I2S_SendData(SPIx,0);
	SPI_I2S_SendData(SPIx,cnt / 0xff);
	SPI_I2S_SendData(SPIx,cnt % 0xff);
	
}


void motor_speed_set(u8 WHICH_MOTOR,float Speed)
{
	float Voltage = 0;
	
	Voltage = Speed / PI / D_MOROR / CNT_PRE_MIN_PRE_V;
	switch(WHICH_MOTOR)
	{
		case LEFT_MOTOR:
			GPIO_ResetBits(GPIOD,MLF_CS);
			break;
		case RIGHT_MOTOR:
			GPIO_ResetBits(GPIOD,MRG_CS);
			break;
		case UP_DOWN_MOTOR:
			GPIO_ResetBits(GPIOE,MUD_CS);
			break;
		case ROTATION_MOTOR:
			GPIO_ResetBits(GPIOE,MRO_CS);
			break;
		
	}
	
	DAC_8551_set(SPI1,Voltage);
	
	switch(WHICH_MOTOR)
	{
		case LEFT_MOTOR:
			GPIO_SetBits(GPIOD,MLF_CS);
			break;
		case RIGHT_MOTOR:
			GPIO_SetBits(GPIOD,MRG_CS);
			break;
		case UP_DOWN_MOTOR:
			GPIO_SetBits(GPIOE,MUD_CS);
			break;
		case ROTATION_MOTOR:
			GPIO_SetBits(GPIOE,MRO_CS);
			break;
	}
	
}

void motor_run(u8 WHICH_MOTOR,u8 CW_CCW)
{
	if(CW_CCW == CW)
	{
		switch(WHICH_MOTOR) 
		{
			case LEFT_MOTOR:
				GPIO_SetBits(GPIOD,MLF_REV);
				GPIO_ResetBits(GPIOD,MLF_FWD);
				break;
			case RIGHT_MOTOR:
				GPIO_SetBits(GPIOD,MRG_REV);
				GPIO_ResetBits(GPIOD,MRG_FWD);
				break;
			case UP_DOWN_MOTOR:
				GPIO_SetBits(GPIOD,MUD_REV);
				GPIO_ResetBits(GPIOD,MUD_FWD);
				break;
			case ROTATION_MOTOR:
				GPIO_ResetBits(GPIOE,MRO_CW_CCW);
				GPIO_ResetBits(GPIOE,MRO_RUN_BREAK | MRO_START_STOP);
				break;
		}
	}
	else 
	{
		switch(WHICH_MOTOR) 
		{
			case LEFT_MOTOR:
				GPIO_SetBits(GPIOD,MLF_FWD);
				GPIO_ResetBits(GPIOD,MLF_REV);
				break;
			case RIGHT_MOTOR:
				GPIO_SetBits(GPIOD,MRG_FWD);
				GPIO_ResetBits(GPIOD,MRG_REV);
				break;
			case UP_DOWN_MOTOR:
				GPIO_SetBits(GPIOD,MUD_FWD);
				GPIO_ResetBits(GPIOD,MUD_REV);
				break;
			case ROTATION_MOTOR:
				GPIO_SetBits(GPIOE,MRO_CW_CCW);
				GPIO_ResetBits(GPIOE,MRO_RUN_BREAK | MRO_START_STOP);
				break;
		}
	}
}

void motor_stop(u8 WHICH_MOTOR)
{
	switch(WHICH_MOTOR) 
	{
		case LEFT_MOTOR:
			GPIO_SetBits(GPIOD,MLF_REV);
			GPIO_SetBits(GPIOD,MLF_FWD);
			break;
		case RIGHT_MOTOR:
			GPIO_SetBits(GPIOD,MRG_REV);
			GPIO_SetBits(GPIOD,MRG_FWD);
			break;
		case UP_DOWN_MOTOR:
			GPIO_SetBits(GPIOD,MUD_REV);
			GPIO_SetBits(GPIOD,MUD_FWD);
			break;
		case ROTATION_MOTOR:
			GPIO_SetBits(GPIOE,MRO_START_STOP);
			break;
	}
}


void AGV_rotation(u8 LEFT_OR_RIGHT,u8 Degree)
{
	u16 Degree_status = 0;
	Degree_status = AGV_status.Directon;
	motor_speed_set(LEFT_MOTOR,0.5);
	motor_speed_set(RIGHT_MOTOR,0.5);
//	motor_speed_set(ROTATION_MOTOR,0.5);
	switch(LEFT_OR_RIGHT)
	{
		case LEFT:
			motor_run(LEFT_MOTOR,CW);
			motor_run(RIGHT_MOTOR,CCW);
			//motor_run(ROTATION_MOTOR,CCW);
			while(AGV_status.Directon  + Degree - DEGREE_OFFSET < Degree_status /*&& AGV_status.Directon  + Degree + DEGREE_OFFSET > Degree_status*/);
			motor_stop(LEFT_MOTOR);
			motor_stop(RIGHT_MOTOR);
			//motor_stop(ROTATION_MOTOR);
			break;
		case RIGHT:
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CW);
			//motor_run(ROTATION_MOTOR,CW);
			while(AGV_status.Directon  - Degree + DEGREE_OFFSET > Degree_status /*&& AGV_status.Directon  - Degree - DEGREE_OFFSET  < Degree_status*/);
			motor_stop(LEFT_MOTOR);
			motor_stop(RIGHT_MOTOR);
			//motor_stop(ROTATION_MOTOR);
			break;
	}
	
}

