#include "public.h"
#include "motor.h"
#include "board.h"
#include "camera.h"

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
	Voltage = Speed / PI / (D_MOTOR  / 100) / NOMBER_OF_TURNS_PRE_VOLTAGE_PRE_SECODE;
	
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

void AGV_run(void)
{
	motor_run(LEFT_MOTOR,CW);
	motor_run(RIGHT_MOTOR,CCW);
	AGV_status.runing_status = 1;
	AGV_status.rotating_status = 0;
	
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


void AGV_stop(void)
{
	motor_stop(LEFT_MOTOR);
	motor_stop(RIGHT_MOTOR);
	AGV_status.runing_status = 0;
	AGV_status.rotating_status = 0;

}

void AGV_rotation_cotrol(u8 LEFT_OR_RIGHT,u8 Degree)
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
			AGV_status.updata_waitting_status = DRGREE_UPDATA_WRITING;
			while(AGV_status.Directon  + Degree - ACON_DEGREE_OFFSET < Degree_status /*&& AGV_status.Directon  + Degree + ACON_DEGREE_OFFSET > Degree_status*/);
			AGV_status.updata_waitting_status = 0;
			motor_stop(LEFT_MOTOR);
			motor_stop(RIGHT_MOTOR);
			//motor_stop(ROTATION_MOTOR);
			break;
		case RIGHT:
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CW);
			//motor_run(ROTATION_MOTOR,CW);
			AGV_status.updata_waitting_status = DRGREE_UPDATA_WRITING;
			while(AGV_status.Directon  - Degree + ACON_DEGREE_OFFSET > Degree_status /*&& AGV_status.Directon  - Degree - ACON_DEGREE_OFFSET  < Degree_status*/);
			AGV_status.updata_waitting_status = 0;
			motor_stop(LEFT_MOTOR);
			motor_stop(RIGHT_MOTOR);
			//motor_stop(ROTATION_MOTOR);
			break;
	}
	
}

void V_left_set(float degree_alignment)
{
	float V_Left_targer = 0; 
	V_Left_targer = degree_alignment * PI / 180 * (LEN_WHELLS / 100) * ACON_PID_CONTROL_RATE + AGV_status.V_right;
	motor_speed_set(LEFT_MOTOR,V_Left_targer);

}



void AGV_run_control(float len_offset, float degree_offset,float len_dest)
{
		float alignment;
	
	#if 0
		if(len_dest < ACON_DEST_CONTROL_LEN)
		{
			AGV_status.updata_waitting_status = LEN_UPDATA_WRITING;
		}
	#endif
		//PID 调整
		if(len_offset < ACON_PID_CONTROL_LEN_OFFSET && len_offset > -ACON_PID_CONTROL_LEN_OFFSET)
		{
			//航线偏差在许可范围内，调整航向角，使之为零
			PID_data.err_now = -degree_offset / 10.0;
			
		}
		else
		{
			//航线偏差过大，调整航向角，使之在下一个二维码的位置回到航线
			PID_data.err_now = -(degree_offset + 180 * len_offset / len_dest / PI) / 10.0;
		}
		alignment = PID_process(&PID_data);
		V_left_set(alignment);


			//zzs debug
		#if 0
		if(len_dest <  ACON_DEST_LEN_OFFSET && len_dest > -ACON_DEST_LEN_OFFSET)
		{
			motor_stop(LEFT_MOTOR);
			motor_stop(RIGHT_MOTOR);
			AGV_status.runing_status = 0;
		}
		#endif
		
}


void AGV_control(AGV_control_t * AGV_control_data_p)
{
	if(AGV_status.runing_status)
	{
		float degree_offset = 0;
		float len_dest = 0;
		float len_offset = 0;
		switch(AGV_status.runing_towards)
		{
			case 0:
				len_dest = AGV_control_data_p->dest_Y - AGV_status.Y_location;  //始终为正
				len_offset = AGV_status.X_offset;      //左偏为负，右偏为正
				degree_offset =  AGV_status.Directon > 180 ? AGV_status.Directon - 360 : AGV_status.Directon;   //航向角偏差：向左偏位负，向右偏为正
				break;
			case 90:
				len_dest = AGV_control_data_p->dest_X - AGV_status.X_location;
				len_offset = AGV_status.Y_offset;
				degree_offset = AGV_status.Directon - 90;
				break;
			case 180:
				len_dest = AGV_status.Y_location - AGV_control_data_p->dest_Y;
				len_offset = AGV_status.X_offset;
				degree_offset = AGV_status.Directon - 180;
				break;
			case 270:
				len_dest = AGV_status.X_location - AGV_control_data_p->dest_X;
				len_offset = AGV_status.Y_offset;
				degree_offset = AGV_status.Directon - 270;
				break;
		}
		AGV_run_control(len_offset,degree_offset,len_dest);
		
	}

	if(AGV_status.rotating_status)
	{
		
		//PID调节左轮速度，使之与右轮相同，若处于举升状态，托盘同步回转
		
	}
	
	if(!(AGV_status.rotating_status || AGV_status.runing_status))
	{
		//停止状态下，下步行动的部署
		
		
	}


}

void AGV_V_set(float speed)
{
	motor_speed_set(RIGHT_MOTOR,speed);
	motor_speed_set(LEFT_MOTOR,speed);
}



void AGV_pre_set(void)
{
	AGV_V_set(0.01);
	if(AGV_status.Directon > 0)
	{
		if(AGV_status.Directon > 1)
		{
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CCW);
		}
		else
		{
			AGV_status.init_Directon_flag = 1;
		}
	}
	else
	{
		if(AGV_status.Directon  < -1)
		{
			motor_run(RIGHT_MOTOR,CW);
			motor_run(RIGHT_MOTOR,CW);
			
		}
		else
		{
			AGV_status.init_Directon_flag = 1;
		}
	}
	if(AGV_status.init_Directon_flag)
	{
		motor_stop(LEFT_MOTOR);
		motor_stop(RIGHT_MOTOR);
		CON_ENCODE_LEFT->CNT  = 0;
		CON_ENCODE_RIGHT->CNT  = 0;
		
	}
		
	
}

