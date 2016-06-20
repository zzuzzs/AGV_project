/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "string.h"
#include "tuoluoyi.h" 
#include "camera.h"
#include "motor.h"
#include "public.h"
#include "board.h"
#include "includes.h"

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}

 
 

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
#ifdef CON_TUOLUOYI_USART1
#define USART1_RX_BUF		TUOLUOYI_RX_BUF
#define USART1_BUF_LEN  TUOLUOYI_BUF_LEN
#define USART1_INDEX		Tuoluoyi_rx_sta
#endif /*CON_TUOLUOYI_USART1*/

#ifdef CON_CAMERA_USART1
#define USART1_RX_BUF		CAMERA_RX_BUF
#define USART1_BUF_LEN  CAMERA_BUF_LEN
#define USART1_INDEX		Camera_rx_sta
#endif /*CON_CAMERA_USART1*/

void USART1_IRQHandler(void)
{
	u8 res;
	
	if(USART1->SR&(1<<5))	
	{	 
		
		res=USART1->DR; 

		USART1_RX_BUF[USART1_INDEX]=res;
		USART1_INDEX++;
		
		if(USART1_INDEX >= USART1_BUF_LEN)    //循环存储
		{
			USART1_INDEX = 0;
		}
	}
	
}

#ifdef CON_TUOLUOYI_USART2
#define USART2_RX_BUF		TUOLUOYI_RX_BUF
#define USART2_BUF_LEN  TUOLUOYI_BUF_LEN
#define USART2_INDEX		Tuoluoyi_rx_sta
#endif /*CON_TUOLUOYI_USART2*/

#ifdef CON_CAMERA_USART2
#define USART2_RX_BUF		CAMERA_RX_BUF
#define USART2_BUF_LEN  CAMERA_BUF_LEN
#define USART2_INDEX		Camera_rx_sta
#endif /*CON_CAMERA_USART2*/

void USART2_IRQHandler(void)
{
	u8 res;
	
	if(USART2->SR&(1<<5))	
	{	 
		res=USART2->DR; 

    USART2_RX_BUF[USART2_INDEX]=res;
		USART2_INDEX++;
		
		if(USART2_INDEX >= USART2_BUF_LEN)    //循环存储
		{
			USART2_INDEX = 0;
		}
	}
	
}

#ifdef CON_DEBUG_USART3

#define USART3_RX_BUF command_buf
#define USART3_BUF_LEN  9
u8  USART3_INDEX	= 0;

void USART3_IRQHandler(void)
{
	u8 res;
	
	if(USART3->SR&(1<<5))	
	{	 
		res=USART3->DR; 

    USART3_RX_BUF[USART3_INDEX]=res;
		USART3_INDEX++;
		
		
		if(USART3_INDEX >= USART3_BUF_LEN)    //循环存储
		{
			USART3_INDEX = 0;
		}
		if(res == 0xff){
			/*
			//关闭后台任务，初始化相关变量
			TUOLUOYI_IRQ_Set(DISABLE);

			
			memset((void *)&AGV_status,0,sizeof(AGV_status));
			memset((void*)&tuoluoyi_status,0,sizeof(tuoluoyi_status));
			memset((void*)&camera_status,0,sizeof(camera_status));	
			*/
			USART3_INDEX = 0;
			command_process();
			
		}
	}
	
}
#endif    //CON_DEBUG_USART3

void SysTick_Handler(void)
{
	int tmp = 0,cnt = 0;
	float LEN_right = 0,LEN_left = 0;
	systick++;
	
	if(systick % ACON_PID_CONTROL_TIME == 0)
	{
		if(AGV_status.runing_status || AGV_status.rotating_status)
		{
			tmp = CON_ENCODE_RIGHT->CNT;
			cnt = (tmp + (ACON_TIM_CONT- AGV_status.encode_right_cnt)) % ACON_TIM_CONT;
			if(cnt > ACON_TIM_CONT / 2)
			{
				cnt = 0;
			}
			AGV_status.V_right  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000;
			AGV_status.encode_right_cnt = tmp;
			
			tmp = CON_ENCODE_LEFT->CNT;
			cnt = (tmp - AGV_status.encode_left_cnt + ACON_TIM_CONT) % ACON_TIM_CONT;
			if(cnt > ACON_TIM_CONT / 2)
			{
				cnt = 0;
			}
			AGV_status.V_left  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000;
			AGV_status.encode_left_cnt = tmp;

			if(LEN_UPDATA_WRITING == AGV_status.updata_waitting_status)
			{
				LEN_right = AGV_status.V_right * ACON_PID_CONTROL_TIME / 1000 * 100;
				LEN_left = AGV_status.V_left * ACON_PID_CONTROL_TIME / 1000 * 100;
				switch(AGV_status.runing_towards)
				{
					case 0:
						AGV_status.X_location += (LEN_right + LEN_left) / 2;
					break;
					case 90:
						AGV_status.Y_location += (LEN_right + LEN_left) / 2;
					break;
					case 180:
						AGV_status.X_location -= (LEN_right + LEN_left) / 2;
					break;
					case 270:
						AGV_status.Y_location -= (LEN_right + LEN_left) / 2;
					break;
				}

			}
			
			AGV_control();
		}
	}
}


void START_BUTTON_IRQ_Handler
{
	EXTI_ClearFlag(START_BUTTON_IRQ_LINE);
	AGV_status.runbutton_status = 1;
	AGV_status.suspendbutton_status = 0;
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}


void STOP_BUTTON_IRQ_Handler
{
	EXTI_ClearFlag(STOP_BUTTON_IRQ_LINE);
	AGV_status.runbutton_status = 0;
	AGV_status.suspendbutton_status = 1;
	AGV_stop();
}

