 #ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H
#include "app_config.h"


#define CON_ENCODE_CNT  8000
#define D_MOTOR		17.5		//轮子直径,单位cm


#define LEN_WHELLS  65.4   //轮间距,单位cm
#define LEN_CAMERA_TO_CENTRE    40    //相机中心到小车中心的距离 单位cm


//#define CON_GYRO_USART1	 
#define CON_GYRO_USART2

#define CON_CAMERA_USART1
//#define CON_CAMERA_USART2

#define CON_DEBUG_USART3

#define CON_ENCODE_RIGHT    	 TIM1
#define CON_ENCODE_LEFT    		 TIM3
#define CON_ENCODE_ROTATING    TIM4

#define START_BUTTON_IRQ  EXTI0_IRQn
#define START_BUTTON_IRQ_LINE EXTI_Line0
#define START_BUTTON_IRQ_Handler  EXTI0_IRQHandler(void)
	
#define STOP_BUTTON_IRQ  EXTI1_IRQn
#define STOP_BUTTON_IRQ_LINE EXTI_Line1
#define STOP_BUTTON_IRQ_Handler  EXTI1_IRQHandler(void)


#define CON_AVOID_OBJ_LEFT_IRQPIN   MCUDI0
#ifdef CON_AVOID_OBJ_LEFT_IRQPIN
	//#define CON_AVOID_OBJ_LEFT_IRQTRIGGER_MODE FALLINT_MODE
	//#define CON_AVOID_OBJ_LEFT_IRQTRIGGER_MODE RISING_MODE
	#define CON_AVOID_OBJ_LEFT_IRQTRIGGER_MODE FALLINT_AND_RISING_MODE
#endif

#define CON_AVOID_OBJ_RIGHT_IRQPIN   MCUDI1
#ifdef CON_AVOID_OBJ_RIGHT_IRQPIN
	#define CON_AVOID_OBJ_RIGHT_IRQTRIGGER_MODE FALLINT_AND_RISING_MODE
#endif

#define CON_PALLET_UP_IRQPIN      MCUDI7
#ifdef CON_PALLET_UP_IRQPIN
	#define CON_PALLET_UP_IRQTRIGGER_MODE FALLINT_MODE
#endif

#define CON_PALLET_DOWN_IRQPIN      MCUDI6
#ifdef CON_PALLET_DOWN_IRQPIN
	#define CON_PALLET_DOWN_IRQ TRIGGER_MODE FALLINT_MODE
#endif


/************************autoconfig************************************/
#ifdef CON_GYRO_USART1
#define GYRO_IRQ USART1_IRQn;
#endif //CON_GYRO_USART1

#ifdef CON_GYRO_USART2
#define GYRO_IRQ USART2_IRQn;
#endif //CON_GYRO_USART2

#ifdef CON_CAMERA_USART1
#define CAMERA_IRQ USART1_IRQn;
#endif //CON_CAMERA_USART1

#ifdef CON_CAMERA_USART2
#define CAMERA_IRQ USART2_IRQn;
#endif //CON_CAMERA_USART2

#ifdef CON_DEBUG_USART3 
#define DEBUG_USART_IRQ  USART3_IRQn;
#endif  //CON_DEBUG_USART3 

#if  (CON_AVOID_OBJ_LEFT_IRQPIN == MCUDI0)
//#define AVOID_OBJ_LEFT_IRQ  EXTI15_10_IRQn
#define AVOID_OBJ_LEFT_IRQPIN_GPIO GPIOD
#define AVOID_OBJ_LEFT_IRQ_LINE EXTI_Line11
#endif

#if  (CON_AVOID_OBJ_RIGHT_IRQPIN == MCUDI1)
//#define AVOID_OBJ_RIGHT_IRQ  EXTI15_10_IRQn
#define AVOID_OBJ_RIGHT_IRQPIN_GPIO  GPIOD
#define AVOID_OBJ_RIGHT_IRQ_LINE EXTI_Line10
#endif

#if (CON_PALLET_UP_IRQPIN == MCUDI7)
//#define PALLET_UP_IRQ 	EXTI15_10_IRQn
#define PALLET_UP_IRQ_LINE EXTI_Line12
#endif

#if (CON_PALLET_DOWN_IRQPIN == MCUDI6)
//#define PALLET_DOWN_IRQ 	EXTI15_10_IRQn
#define PALLET_DOWN_IRQ_LINE EXTI_Line13
#endif

#define FALLINT_MODE  EXTI_Trigger_Falling
#define RISING_MODE   EXTI_Trigger_Rising
#define FALLINT_AND_RISING_MODE  EXTI_Trigger_Rising_Falling


/**********************check************************************/

#if defined(CON_GYRO_USART1)  && defined(CON_CAMERA_USART1)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#if defined(CON_GYRO_USART2)  && defined(CON_CAMERA_USART2)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#ifndef  ACON_TIM_CONT
#error "please check TIM CONT set in board_config.h"
#endif  /*CON_TIM_CONT*/

#endif //__BOARD_CONFIG_H
