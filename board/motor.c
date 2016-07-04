#include <stdio.h>
#include "public.h"
#include "motor.h"
#include "board.h"

static float DAC_8551_set(SPI_TypeDef* SPIx,float Voltage)
{
	int cnt = 0;
	
	if(Voltage < 0 )
	{
		Voltage = 0;
	}
	 if(Voltage > 4.5)
		 Voltage = 4.5;
	cnt = Voltage * 0xffff / V_REF;
	SPI_I2S_SendData(SPIx,0);
	SPI_I2S_SendData(SPIx,cnt / 0xff);
	SPI_I2S_SendData(SPIx,cnt % 0xff);
	 
	return  Voltage;
	
}


void motor_speed_set(u8 WHICH_MOTOR,float Speed)
{
	float Voltage = 0;
	Voltage = Speed / PI / (D_MOTOR  / 100.0) / NOMBER_OF_TURNS_PRE_VOLTAGE_PRE_SECODE;
	
	switch(WHICH_MOTOR)
	{
		case LEFT_MOTOR:
			Voltage += 0.09;
			GPIO_ResetBits(GPIOD,MLF_CS);
			AGV_status.left_Voltage = DAC_8551_set(SPI1,Voltage);
			GPIO_SetBits(GPIOD,MLF_CS);
			break;
		case RIGHT_MOTOR:
			Voltage += 0.096;
			GPIO_ResetBits(GPIOD,MRG_CS);
			AGV_status.right_Voltage = DAC_8551_set(SPI1,Voltage);
			GPIO_SetBits(GPIOD,MRG_CS);
			break;
		case UP_DOWN_MOTOR:
			GPIO_ResetBits(GPIOE,MUD_CS);
			DAC_8551_set(SPI1,Voltage);
			GPIO_SetBits(GPIOE,MUD_CS);
			break;
		case ROTATION_MOTOR:
			GPIO_ResetBits(GPIOE,MRO_CS);
			DAC_8551_set(SPI1,Voltage);
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



