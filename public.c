#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include <stdio.h>
#include "camera.h"
#include "tuoluoyi.h"
#include "motor.h"


AGV_status_t AGV_status = {0};
AGV_control_t AGV_control_data_1 = {0};
AGV_control_t AGV_control_data_2 = {0};
AGV_control_t AGV_control_data_3 = {0};
AGV_control_t AGV_control_data_4 = {0};
PID_data_t PID_data_run = {0};
PID_data_t PID_data_rotate = {0};
u32 systick = 0;

u8 command_buf[9] = {0};


void AGV_control_data_init(void)
{
	AGV_control_data_1.available_flag  = 1;
	AGV_control_data_1.data_type = RUNING_TYPE;
	AGV_control_data_1.data.dest_data.dest_X = 160;
	AGV_control_data_1.data.dest_data.dest_Y = 480;
	
	AGV_control_data_2.available_flag = 1;
	AGV_control_data_2.data_type = ROTATION_TYPE;
	AGV_control_data_2.data.rotating_data.rotating_towards = LEFT;
	AGV_control_data_2.data.rotating_data.rotating_degree = 90;
	
	AGV_control_data_1.next = &AGV_control_data_2;
	AGV_control_data_2.next = &AGV_control_data_1;
	AGV_control_data_3.next = &AGV_control_data_4;
	AGV_control_data_4.next = &AGV_control_data_1;

}



void delay_ms(u32 cnt)
{
	u32 tmp = systick;
	
	while(!(systick - tmp  == cnt));
	
}


void START_BUTTON_IRQ_Set(FunctionalState status)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	NVIC_InitStructure.NVIC_IRQChannel = START_BUTTON_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = START_BUTTON_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = status;
  NVIC_Init(&NVIC_InitStructure);
}


void CAMERA_IRQ_Set(FunctionalState status)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	NVIC_InitStructure.NVIC_IRQChannel = CAMERA_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAMERA_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = status;
  NVIC_Init(&NVIC_InitStructure);
}


void TUOLUOYI_IRQ_Set(FunctionalState status)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	NVIC_InitStructure.NVIC_IRQChannel = TUOLUOYI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TUOLUOYI_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = status;
  NVIC_Init(&NVIC_InitStructure);
}

void PID_run_init()
{
	PID_data_run.Kp = PID_RUN_KP;
	PID_data_run.Ti = PID_RUN_TI;

	PID_data_run.err_now = 0;
	PID_data_run.err_pre_1 = 0;
	//PID_data_run.err_pre_2 = 0;

}


void PID_rotate_init()
{
	PID_data_rotate.Kp = PID_RUN_KP;
	PID_data_rotate.Ti = PID_RUN_TI;

	PID_data_rotate.err_now = 0;
	PID_data_rotate.err_pre_1 = 0;
	//PID_data_rotate.err_pre_2 = 0;

}


float  PID_process(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * PID_data_p->err_now;//(PID_data_p->err_now - PID_data_p->err_pre_1)  +  \
				PID_data_p->Kp * (ACON_PID_CONTROL_TIME / 1000) * PID_data_p->err_now / PID_data_p->Ti;  // +   \
				PID_data_p->Kd * ACON_PID_CONTROL_RATE * (PID_data_p->err_now + PID_data_p->err_pre_2 - 2 * PID_data_p->err_pre_1);
	
	PID_data_p->err_pre_1 = PID_data_p->err_now;
	//PID_data_p->err_pre_2 = PID_data_p->err_pre_1;
	return tmp;
}

float PID_process_tmp(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * (PID_data_p->err_now - PID_data_p->err_pre_1)  +  \
				PID_data_p->Kp * (ACON_PID_CONTROL_TIME / 1000) * PID_data_p->err_now / PID_data_p->Ti; 
	
	PID_data_p->err_pre_1 = PID_data_p->err_now;
	return tmp;
}

void command_process(void)
{
	u8 int_part = 0;
	float float_part = 0;
	
	switch(command_buf[0])
	{
	/*
		case PID_VALUE:
			int_part = command_buf[1];
			float_part = (((u16)command_buf[2] << 8) + command_buf[3]) / 1000.0;
			PID_data.Kp = int_part +  float_part;
			
			int_part = command_buf[4];
			float_part = (((u16)command_buf[5] << 8) + command_buf[6]) / 1000.0;
			PID_data.Ti = int_part +  float_part;
	
			break;
		*/
		case START_BUTTON:
			AGV_status.runbutton_status = 1;
			AGV_status.suspendbutton_status = 0;
			break;
	}
	

}


void usart_sent(const char *p)
{
	while(*p != '\0')
	{
		while((USART3->SR&0X40)==0);//等待上一次串口数据发送完成
		USART3->DR = (u8)(*p);
		p++;
	}
}


void status_printf(AGV_status_t *p)
{
	char tmp[100]  = {0};//= "**\n";
//	usart_sent(tmp);
	/*
	sprintf(tmp,"A%f ",p->V_right);
	usart_sent(tmp);
	
	sprintf(tmp,"B%f\n",p->V_left);
	usart_sent(tmp);
		
	sprintf(tmp,"C%f ",p->right_Voltage);
	usart_sent(tmp);
	
	sprintf(tmp,"D%f\n",p->left_Voltage);
	usart_sent(tmp);  
	
	sprintf(tmp,"E%d\n ",p->encode_right_cnt);
	usart_sent(tmp);
	
	sprintf(tmp,"F%d\n",p->encode_left_cnt);
	usart_sent(tmp);
	
	sprintf(tmp,"G%f\n",p->Directon < 180 ? p->Directon : p->Directon - 360);
	usart_sent(tmp);

	sprintf(tmp,"H%f\n",p->X_location);
	usart_sent(tmp);
	*/
	
	sprintf(tmp,"I%f\n",tuoluoyiinfo.yaw);
	usart_sent(tmp);
	
}




