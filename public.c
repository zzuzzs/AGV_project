#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"


AGV_status_t AGV_status = {0};
AGV_control_t AGV_control_data = {8,4};
PID_data_t PID_data = {0};
u32 systick = 0;

u16 encode_right_cnt = 0;
u16 encode_left_cnt = 0;

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
	PID_data.Ki = PID_KI;
	PID_data.Kd = PID_KD;
	PID_data.err_now = 0;
	PID_data.err_pre_1 = 0;
	PID_data.err_pre_2 = 0;

}


float  PID_process(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * (PID_data_p->err_now - PID_data_p->err_pre_1)  +  \
				PID_data_p->Ki * ACON_PID_CONTROL_TIME * PID_data_p->err_now   +   \
				PID_data_p->Kd * ACON_PID_CONTROL_RATE * (PID_data_p->err_now + PID_data_p->err_pre_2 - 2 * PID_data_p->err_pre_1);
	
	PID_data_p->err_pre_1 = PID_data_p->err_now;
	PID_data_p->err_pre_2 = PID_data_p->err_pre_1;
	
	return tmp;
}

void command_process(void)
{
	u8 int_part = 0;
	u16 float_part = 0;
	
	int_part = command_buf[0];
	float_part = (command_buf[1] * 0x100 + command_buf[2]) % 1000;
	PID_data.Kp = int_part +  float_part / 1000;
	
	int_part = command_buf[3];
	float_part = (command_buf[4] * 0x100 + command_buf[5]) % 1000;
	PID_data.Ki = int_part +  float_part / 1000;
	
	int_part = command_buf[6];
	float_part = (command_buf[7] * 0x100 + command_buf[8]) % 1000;
	PID_data.Kd = int_part +  float_part / 1000;
	
	
	
	
}
