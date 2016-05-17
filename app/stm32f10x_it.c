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
#include "tuoluoyi.h" 
#include "camera.h"

 
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
		
		if(USART1_INDEX >= USART1_BUF_LEN)    //Ñ­»·´æ´¢
		{
			USART1_INDEX = 0;
		}
	}
	
}

#ifdef CON_TUOLUOYI_USART1
#define USART2_RX_BUF		TUOLUOYI_RX_BUF
#define USART2_BUF_LEN  TUOLUOYI_BUF_LEN
#define USART2_INDEX		Tuoluoyi_rx_sta
#endif /*CON_TUOLUOYI_USART1*/

#ifdef CON_CAMERA_USART1
#define USART2_RX_BUF		CAMERA_RX_BUF
#define USART2_BUF_LEN  CAMERA_BUF_LEN
#define USART2_INDEX		Camera_rx_sta
#endif /*CON_CAMERA_USART1*/

void USART2_IRQHandler(void)
{
	u8 res;
	
	if(USART2->SR&(1<<5))	
	{	 
		res=USART2->DR; 

		USART2_RX_BUF[USART2_INDEX]=res;
		USART2_INDEX++;
		
		if(USART2_INDEX >= USART2_BUF_LEN)    //Ñ­»·´æ´¢
		{
			USART2_INDEX = 0;
		}
	}
	
}
