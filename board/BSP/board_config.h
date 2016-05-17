#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H
#include "app_config.h"

#define CON_TUOLUOYI_USART1	 
//#define CON_TUOLUOYI_UART2

//#define CON_CAMERA_USART1
#define CON_CAMERA_USART2

#define CON_TIM_CONT  0xffff



/**********************check************************************/

#if defined(CON_TUOLUOYI_USART1)  && defined(CON_CAMERA_USART1)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#if defined(CON_TUOLUOYI_USART2)  && defined(CON_CAMERA_USART2)
#error "resource conflict.please check tuoluoyiand &&camera usart set in board_config.h"
#endif

#ifndef  CON_TIM_CONT
#error "please check TIM CONT set in board_config.h"
#endif  /*CON_TIM_CONT*/

#endif //__BOARD_CONFIG_H
