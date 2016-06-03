#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H
#include "app_config.h"


#define CON_ENCODE_CNT  8000
#define D_MOTOR		0.16		//轮子直径,单位m


#define LEN_WHELLS  1   //轮间距,单位m
#define LEN_CAMERA_TO_CENTRE     0.5   //相机中心到小车中心的距离


//#define CON_TUOLUOYI_USART1	 
#define CON_TUOLUOYI_USART2

#define CON_CAMERA_USART1
//#define CON_CAMERA_USART2

#define CON_DEBUG_USART3

#define START_BUTTON_IRQ  EXTI0_IRQn
#define START_BUTTON_IRQ_LINE EXTI_Line0
#define START_BUTTON_IRQ_Handler  EXTI0_IRQHandler(void)
	
#define STOP_BUTTON_IRQ  EXTI1_IRQn
#define STOP_BUTTON_IRQ_LINE EXTI_Line1
#define STOP_BUTTON_IRQ_Handler  EXTI1_IRQHandler(void)

#define CON_ENCODE_RIGHT    	 TIM1
#define CON_ENCODE_LEFT    		 TIM3
#define CON_ENCODE_ROTATING    TIM4

#ifdef CON_TUOLUOYI_USART1
#define TUOLUOYI_IRQ USART1_IRQn;
#endif //CON_TUOLUOYI_USART1

#ifdef CON_TUOLUOYI_USART2
#define TUOLUOYI_IRQ USART2_IRQn;
#endif //CON_TUOLUOYI_USART2


#ifdef CON_CAMERA_USART1
#define CAMERA_IRQ USART1_IRQn;
#endif //CON_CAMERA_USART1

#ifdef CON_CAMERA_USART2
#define CAMERA_IRQ USART2_IRQn;
#endif //CON_CAMERA_USART2

#ifdef CON_DEBUG_USART3 
#define DEBUG_USART_IRQ  USART3_IRQn;
#endif  //CON_DEBUG_USART3 

/**********************check************************************/

#if defined(CON_TUOLUOYI_USART1)  && defined(CON_CAMERA_USART1)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#if defined(CON_TUOLUOYI_USART2)  && defined(CON_CAMERA_USART2)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#ifndef  ACON_TIM_CONT
#error "please check TIM CONT set in board_config.h"
#endif  /*CON_TIM_CONT*/

#endif //__BOARD_CONFIG_H
