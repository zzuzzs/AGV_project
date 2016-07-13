#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include <stdio.h>
#include "camera.h"
#include "gyro.h"
#include "motor.h"


AGV_status_t AGV_status = {0};
AGV_control_t AGV_control_data_1 = {0};
AGV_control_t AGV_control_data_2 = {0};
AGV_control_t AGV_control_data_3 = {0};
AGV_control_t AGV_control_data_4 = {0};
AGV_control_t AGV_control_data_5 = {0};
AGV_control_t AGV_control_data_6 = {0};
AGV_control_t AGV_control_data_7 = {0};
AGV_control_t AGV_control_data_8 = {0};


PID_data_t PID_data_run = {0};
PID_data_t PID_data_V = {0};

u32 systick = 0;
kalman_data_t Encode_kalman_data = {0};
Encode_data_t Encode_data = {0};

kalman_data_t Degree_kalman_data = {0};

u8 command_buf[9] = {0};


void AGV_control_data_init(void)
{
	AGV_control_data_1.available_flag  = 1;
	AGV_control_data_1.data_type = RUNING_TYPE;
	AGV_control_data_1.data.dest_data.dest_X = 800;
	AGV_control_data_1.data.dest_data.dest_Y = 0;
	
	AGV_control_data_2.available_flag = 1;
	AGV_control_data_2.data_type = ROTATION_TYPE;
	AGV_control_data_2.data.rotating_data.rotating_towards = RIGHT;
	AGV_control_data_2.data.rotating_data.rotating_degree = 90;
	
	AGV_control_data_3.available_flag = 1;
	AGV_control_data_3.data_type = RUNING_TYPE;
	AGV_control_data_3.data.dest_data.dest_X = 800;
	AGV_control_data_3.data.dest_data.dest_Y = 400;
	
	AGV_control_data_4.available_flag = 1;
	AGV_control_data_4.data_type = ROTATION_TYPE;
	AGV_control_data_4.data.rotating_data.rotating_towards = RIGHT;
	AGV_control_data_4.data.rotating_data.rotating_degree = 90;
	
	AGV_control_data_5.available_flag = 1;
	AGV_control_data_5.data_type = RUNING_TYPE;
	AGV_control_data_5.data.dest_data.dest_X = 0;
	AGV_control_data_5.data.dest_data.dest_Y = 400;
	
	AGV_control_data_6.available_flag = 1;
	AGV_control_data_6.data_type = ROTATION_TYPE;
	AGV_control_data_6.data.rotating_data.rotating_towards = RIGHT;
	AGV_control_data_6.data.rotating_data.rotating_degree = 90;
	
	AGV_control_data_7.available_flag = 1;
	AGV_control_data_7.data_type = RUNING_TYPE;
	AGV_control_data_7.data.dest_data.dest_X = 0;
	AGV_control_data_7.data.dest_data.dest_Y = 0;
	
	AGV_control_data_8.available_flag = 1;
	AGV_control_data_8.data_type = ROTATION_TYPE;
	AGV_control_data_8.data.rotating_data.rotating_towards = RIGHT;
	AGV_control_data_8.data.rotating_data.rotating_degree = 90;
	
	AGV_control_data_1.next = &AGV_control_data_2;
	AGV_control_data_2.next = &AGV_control_data_3;
	AGV_control_data_3.next = &AGV_control_data_4;
	AGV_control_data_4.next = &AGV_control_data_5;
	AGV_control_data_5.next = &AGV_control_data_6;
	AGV_control_data_6.next = &AGV_control_data_7;
	AGV_control_data_7.next = &AGV_control_data_8;
	AGV_control_data_8.next = &AGV_control_data_1;
	

}



void delay_ms(u32 cnt)
{
	u32 tmp = systick;
	
	while(!(systick - tmp  == cnt));
	
}

void Encode_kalman_init(void)
{
	Encode_kalman_data.data_type = ENCODE_DATA ;
	Encode_kalman_data.P = 0.001;
	Encode_kalman_data.H.Encode_Weight = 1;
	Encode_kalman_data.Q = EMNSTD * EMNSTD;
	Encode_kalman_data.R.R_Encode = EMNSTD * EMNSTD;
	//Encode_kalman_data.V.V_Encode = EMNSTD;
	Encode_kalman_data.X = &Encode_data.Degree_T_kalman;
	Encode_kalman_data.EG.Encode_measure = &Encode_data.Degree_T;
	
}


void Degree_kalman_init(void)
{
	Degree_kalman_data.data_type = DEGREE_DATA;
	Degree_kalman_data.EG.Encode_measure = &AGV_status.Direction_Enco;
	Degree_kalman_data.EG.Gyro_measure = &AGV_status.Direction_Gyro;
	Degree_kalman_data.X = &AGV_status.Direction;
	Degree_kalman_data.H.Encode_Weight = 1;
	Degree_kalman_data.H.Gyro_Weight = 1;
	Degree_kalman_data.P = 0.001;
	Degree_kalman_data.Q = 0.001;  //PNSTD * PNSTD;
	Degree_kalman_data.R.R_Encode = RA_ENCO * RA_ENCO;
	Degree_kalman_data.R.R_Gyro  = RA_GYRO * RA_GYRO;
//	Degree_kalman_data.V.V_Encode  = EMNSTD * EMNSTD;
//	Degree_kalman_data.V.V_Gyro = GMNSTD * GMNSTD;
}


void Kalman_process(kalman_data_t * kalman_data_p)
{
	float K = 0;
	float P = 0;
	float H[2] = {0};
	float WE = kalman_data_p->H.Encode_Weight;
	float WG = kalman_data_p->H.Gyro_Weight;
	float RG = kalman_data_p->R.R_Gyro;
	float RE = kalman_data_p->R.R_Encode;
	float E = *kalman_data_p->EG.Encode_measure;
	float G = *kalman_data_p->EG.Gyro_measure;
	float a = 0;
	float b = 0;
	float c = 0;
	float d = 0;
	float detA = 0;
	float A[2][2] = {0};
	float K2[2] = {0};
	float X = 0;
	H[0] = kalman_data_p->H.Encode_Weight;
	H[1] = kalman_data_p->H.Gyro_Weight;
	
	*kalman_data_p->X = *kalman_data_p->X;  //待完善
	X = *kalman_data_p->X;
	kalman_data_p->P += kalman_data_p->Q;
	P = kalman_data_p->P;
	

	switch(kalman_data_p->data_type)
	{
		case GYRO_DATA:
			K = P * WG / (WG * P * WG + RG);
			*kalman_data_p->X += K *(G - WG * X);
			kalman_data_p->P -= K * WG * P;
			break;
		case ENCODE_DATA:
			K = P * WE / (WE * P * WE + RE);
			*kalman_data_p->X += K *(E - WE * X);
			kalman_data_p->P -= K * WE * P;
			break;
		case DEGREE_DATA:
					
			if(0 == AGV_status.runing_towards)
			{
				if(X < 180)
				{
					if(E > 180)
						E -= 360;
					if(G > 180)
						G -= 360;
				}else
				{ 
					if(E < 180)
						E += 360;
					if(G < 180)
						G += 360;
				}
			}
			a = P * WE * WE + RE;
			b = P * WE *WG;
			c = b;
			d = P * WG * WG + RG;
			detA = a * d - b * c;
			A[0][0] = d / detA;
			A[0][1] = -c / detA;
			A[1][0] = -b / detA;
			A[1][1] = a / detA;
			K2[0] = P * (H[0] * A[0][0] + H[1] * A[1][0]);
			K2[1] = P * (H[0] * A[0][1] + H[1] * A[1][1]);
			*kalman_data_p->X += K2[0] * ( E - H[0] * X) + K2[1] * (G - H[1] * X);
			kalman_data_p->P -= P * (K2[0] * H[0] + K2[1] * H[1]);
			if(*kalman_data_p->X >= 360)
			{
				*kalman_data_p->X -= 360;
			}else	if(*kalman_data_p->X < 0)
			{
				*kalman_data_p->X += 360;
			}
			
			break;
	
	}
	
	
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


void GYRO_IRQ_Set(FunctionalState status)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	NVIC_InitStructure.NVIC_IRQChannel = GYRO_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GYRO_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = status;
  NVIC_Init(&NVIC_InitStructure);
}

void PID_run_data_init()
{
	PID_data_run.Kp = PID_RUN_KP;
	PID_data_run.Ti = PID_RUN_TI;
	PID_data_run.Td = PID_RUN_TD;
	PID_data_run.err_now = 0;
	PID_data_run.err_pre_1 = 0;
	PID_data_run.err_pre_2 = 0;

}


void PID_V_data_init()
{
	PID_data_V.Kp = PID_V_KP;
	PID_data_V.Ti = PID_V_TI;
	PID_data_V.Td = PID_V_TD;
	PID_data_V.err_now = 0;
	PID_data_V.err_pre_1 = 0;
	PID_data_V.err_pre_2 = 0;

}


float  PID_process(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * (PID_data_p->err_now - PID_data_p->err_pre_1)  +  \
				PID_data_p->Kp * (ACON_PID_CONTROL_TIME / 1000.0) * PID_data_p->err_now / PID_data_p->Ti +   \
				PID_data_p->Kp * PID_data_p->Td * ACON_PID_CONTROL_RATE * (PID_data_p->err_now + PID_data_p->err_pre_2 - 2 * PID_data_p->err_pre_1);
	PID_data_p->err_pre_2 = PID_data_p->err_pre_1;
	PID_data_p->err_pre_1 = PID_data_p->err_now;
	return tmp;
}


float  PID_process_tmp(PID_data_t * PID_data_p)
{
	float tmp = 0;
	tmp = PID_data_p->Kp * (PID_data_p->err_now - PID_data_p->err_pre_1);  // +  \
				PID_data_p->Kp * (ACON_PID_CONTROL_TIME / 1000.0) * PID_data_p->err_now / PID_data_p->Ti +   \
				PID_data_p->Kp * PID_data_p->Td * ACON_PID_CONTROL_RATE * (PID_data_p->err_now + PID_data_p->err_pre_2 - 2 * PID_data_p->err_pre_1);
	
	
	PID_data_p->err_pre_2 = PID_data_p->err_pre_1;
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
	
	sprintf(tmp,"A%f ",p->V_right);
	usart_sent(tmp);
	
	sprintf(tmp,"B%f\n",p->V_left);
	usart_sent(tmp);
	/*	
	sprintf(tmp,"C%f ",p->right_Voltage);
	usart_sent(tmp);
	
	sprintf(tmp,"D%f\n",p->left_Voltage);
	usart_sent(tmp); 
	
	sprintf(tmp,"E%d\n ",p->encode_right_cnt);
	usart_sent(tmp);
	
	sprintf(tmp,"F%d\n",p->encode_left_cnt);
	usart_sent(tmp);
	
	sprintf(tmp,"H%f\n",p->X_location);
	usart_sent(tmp);
	*/

	sprintf(tmp,"G%f\n",p->Direction > 180 ? p->Direction - 360 : p->Direction);
	usart_sent(tmp);
	
	sprintf(tmp,"I%f\n",p->X_offset);
	usart_sent(tmp);
	
	sprintf(tmp,"J%f\n",p->V_Set);
	usart_sent(tmp);
	
}




