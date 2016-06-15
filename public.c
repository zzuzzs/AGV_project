#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include <stdio.h>


AGV_status_t AGV_status = {0};
AGV_control_t AGV_control_data = {800,400};
PID_data_t PID_data = {0};
u32 systick = 0;



u8 command_buf[9] = {0};



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

void PID_init()
{
	PID_data.Kp = PID_KP;
	PID_data.Ti = PID_TI;
//	PID_data.Ki = PID_KI;
	//PID_data.Kd = PID_KD;
	PID_data.err_now = 0;
	PID_data.err_pre_1 = 0;
//	PID_data.err_pre_2 = 0;

}


float  PID_process(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * PID_data_p->err_now;//(PID_data_p->err_now - PID_data_p->err_pre_1)  +  \
				PID_data_p->Kp * (ACON_PID_CONTROL_TIME / 1000) * PID_data_p->err_now / PID_data.Ti;  // +   \
				PID_data_p->Kd * ACON_PID_CONTROL_RATE * (PID_data_p->err_now + PID_data_p->err_pre_2 - 2 * PID_data_p->err_pre_1);
	
	PID_data_p->err_pre_1 = PID_data_p->err_now;
	//PID_data_p->err_pre_2 = PID_data_p->err_pre_1;
	return tmp;
}

void command_process(void)
{
	u8 int_part = 0;
	float float_part = 0;
	
	switch(command_buf[0])
	{
		case PID_VALUE:
			int_part = command_buf[1];
			float_part = (((u16)command_buf[2] << 8) + command_buf[3]) / 1000.0;
			PID_data.Kp = int_part +  float_part;
			
			int_part = command_buf[4];
			float_part = (((u16)command_buf[5] << 8) + command_buf[6]) / 1000.0;
			PID_data.Ti = int_part +  float_part;
	
			break;
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
	*/
	sprintf(tmp,"G%f\n",p->Directon < 180 ? p->Directon : p->Directon - 360);
	usart_sent(tmp);

}




