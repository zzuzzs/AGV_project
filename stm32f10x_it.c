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
			//关闭后台任务，初始化相关变量
			TUOLUOYI_IRQ_Set(DISABLE);

			USART3_INDEX = 0;
			memset((void *)&AGV_status,0,sizeof(AGV_status));
			memset((void*)&tuoluoyi_status,0,sizeof(tuoluoyi_status));
			memset((void*)&camera_status,0,sizeof(camera_status));	
			command_process();
		}
	}
	
}
#endif    //CON_DEBUG_USART3



void SysTick_Handler(void)
{
	u16 tmp,cnt;
	systick++;
	
	if(AGV_status.V_right_test_request)
	{
		tmp = CON_ENCODE_RIGHT->CNT;
		cnt = (encode_right_cnt - tmp + ACON_TIM_CONT) % ACON_TIM_CONT;
		AGV_status.V_right  = cnt * PI * D_MOTOR / CON_ENCODE_CNT / ACON_SYSIRQ_TIME * 1000;
		AGV_status.V_right_test_request = 0;
	}
	if(AGV_status.V_left_test_request)
	{
		tmp = CON_ENCODE_LEFT->CNT;
		cnt = (tmp + ACON_TIM_CONT - encode_left_cnt) % ACON_TIM_CONT;
		AGV_status.V_left  = cnt * PI * D_MOTOR / CON_ENCODE_CNT / ACON_SYSIRQ_TIME * 1000;
		AGV_status.V_left_test_request = 0;
	}
	encode_right_cnt = CON_ENCODE_RIGHT->CNT;
	encode_left_cnt = CON_ENCODE_LEFT->CNT;
}


void START_BUTTON_IRQ_Handler
{
	EXTI_ClearFlag(START_BUTTON_IRQ_LINE);
	AGV_status.runbutton_status = 1;
	AGV_status.suspendbutton_status = 0;
}


void STOP_BUTTON_IRQ_Handler
{
	EXTI_ClearFlag(STOP_BUTTON_IRQ_LINE);
	AGV_status.runbutton_status = 0;
	AGV_status.suspendbutton_status = 1;
	AGV_stop();
}

