#include <stdio.h>
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
	AGV_status.updata_waitting_status = 0;
}

void AGV_rotate(void)
{
	int tmp = 0;
	motor_speed_set(LEFT_MOTOR,ACON_ROTATION_SPEED);
	motor_speed_set(RIGHT_MOTOR,ACON_ROTATION_SPEED);
//	motor_speed_set(ROTATION_MOTOR,ACON_ROTATION_SPEED);
	switch(AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
	{
		case LEFT:
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CCW);
			//motor_run(ROTATION_MOTOR,CCW);
			tmp = AGV_status.runing_towards - AGV_status.AGV_control_p->data.rotating_data.rotating_degree;
			break;
		case RIGHT:
			motor_run(LEFT_MOTOR,CW);
			motor_run(RIGHT_MOTOR,CW);
			//motor_run(ROTATION_MOTOR,CW);
			tmp = AGV_status.runing_towards + AGV_status.AGV_control_p->data.rotating_data.rotating_degree;
			break;
	}
	if(tmp >= 360)
	{
		tmp -= 360;
	}
	else if(tmp < 0)
	{
		tmp += 360;
	}
		
	AGV_status.updata_waitting_status = DRGREE_UPDATA_WRITING;   //由于陀螺仪默认状态下一直在更新角度状态，因此此赋值暂无意义
	AGV_status.rotating_status = 1;	
	AGV_status.runing_status = 0;
	AGV_status.runing_towards = tmp;
}

void V_left_set(float degree_alignment)
{
	float V_Left_targer = 0; 
	V_Left_targer = degree_alignment * PI / 180 * (LEN_WHELLS / 100) * ACON_PID_CONTROL_RATE + AGV_status.V_right;
	motor_speed_set(LEFT_MOTOR,V_Left_targer);

}

static void init_next_run_control(void)
{
	int tmp = 0;
	AGV_status.AGV_control_p->available_flag = 0;
	AGV_status.AGV_control_p = AGV_status.AGV_control_p->next;
	switch(AGV_status.AGV_control_p->data_type)
	{
		case RUNING_TYPE:
			AGV_run();
			break;
		case STOP_TYPE:
			AGV_stop();
			break;
		case ROTATION_TYPE:
			AGV_rotate();
			break;
	}
}

void AGV_run_control(float len_offset, float degree_offset,float len_dest)
{
		float alignment;
	
		if(len_dest < ACON_DEST_CONTROL_LEN)
		{
			AGV_status.updata_waitting_status = LEN_UPDATA_WRITING;
		}
		//PID 调整
		if(len_offset < ACON_PID_CONTROL_LEN_OFFSET && len_offset > -ACON_PID_CONTROL_LEN_OFFSET)
		{
			//航线偏差在许可范围内，调整航向角，使之为零
			PID_data_run.err_now = -degree_offset / 10.0;
			
		}
		else
		{
			//航线偏差过大，调整航向角，使之在下一个二维码的位置回到航线
			PID_data_run.err_now = -(degree_offset + 180 * len_offset / ACON_LEN_QR / PI) / 10.0;
		}
		alignment = PID_process(&PID_data_run);
		V_left_set(alignment);

		if(len_dest <  ACON_DEST_LEN_OFFSET)// && len_dest > -ACON_DEST_LEN_OFFSET)
		{
			AGV_stop();
		}
		
}

void AGV_rotating_control(void)
{
	/*
	//PID调节左轮速度，使之与右轮相同，若处于举升状态，托盘同步回转
	float tmp = 0;
	PID_data_rotate.err_now  = AGV_status.V_right - AGV_status.V_left;
	tmp = PID_process_tmp(&PID_data_rotate);
	
	tmp += AGV_status.V_left;
	motor_speed_set(LEFT_MOTOR,tmp);
	*/
	
}



void AGV_control(void)
{
	if(AGV_status.runing_status)
	{
		float degree_offset = 0;
		float len_dest = 0;
		float len_offset = 0;
		switch(AGV_status.runing_towards)
		{
			case 0:
				len_dest = AGV_status.AGV_control_p->data.dest_data.dest_X - AGV_status.X_location;  //始终为正
				degree_offset =  AGV_status.Directon > 180 ? AGV_status.Directon - 360 : AGV_status.Directon;   //航向角偏差：向左偏位负，向右偏为正
				break;
			case 90:
				len_dest = AGV_status.AGV_control_p->data.dest_data.dest_Y - AGV_status.Y_location;
				degree_offset = AGV_status.Directon - 90;
				break;
			case 180:
				len_dest = AGV_status.X_location - AGV_status.AGV_control_p->data.dest_data.dest_X;
				degree_offset = AGV_status.Directon - 180;
				break;
			case 270:
				len_dest = AGV_status.Y_location - AGV_status.AGV_control_p->data.dest_data.dest_Y;
				degree_offset = AGV_status.Directon - 270;
				break;
		}
		len_offset = AGV_status.X_offset;      //左偏为负，右偏为正 
		AGV_run_control(len_offset,degree_offset,len_dest);
		
	}

	if(AGV_status.rotating_status)
	{
		float degree_offset = 0;
		
		AGV_rotating_control();
		
		
		switch(AGV_status.runing_towards)
		{
			case 0:
				degree_offset = AGV_status.Directon > 180 ? AGV_status.Directon - 360 : AGV_status.Directon;
				break;
			default:
				degree_offset = AGV_status.Directon - AGV_status.runing_towards;
		}
		if(degree_offset < 1 && degree_offset > -1)
		{
			AGV_stop();
		}
		
		
	}
	
	if(!(AGV_status.rotating_status || AGV_status.runing_status))
	{
		
		//停止状态下，下步行动的部署
		if(AGV_status.AGV_control_p->next && AGV_status.AGV_control_p->next->available_flag)
			{
				init_next_run_control();
			}
		
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
		if(AGV_status.Directon > 0.5)
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
		if(AGV_status.Directon  < -0.5)
		{
			motor_run(LEFT_MOTOR,CW);
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

