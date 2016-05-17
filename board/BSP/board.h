#ifndef __BOARD_H
#define __BOARD_H

#include "app_config.h" 
#include "board_config.h"

#define SYSCLK_FREQ  72000000


/*********************************key**************************************************/
#define MCUKEY1				GPIO_Pin_0 			//PA
#define MCUKEY2				GPIO_Pin_1			//PA
#define MCUKEY1LED		GPIO_Pin_7			//PE
#define MCUKEY2LED		GPIO_Pin_2			//PB
/**************************************************************************************/



/*********************************LED**************************************************/
#define MCU_LED1			GPIO_Pin_12				//PA
#define MCU_LED2			GPIO_Pin_11				//PA
/**************************************************************************************/


/*********************************encode(TIM)************************************************/

//TIM3 REMAPED
#define MENCODER_AJ1		GPIO_Pin_6		//PC
#define MENCODER_BJ1		GPIO_Pin_7		//PC
#define MENCODER_CJ1		GPIO_Pin_8		//PC

//TIM1
#define MENCODER_AJ2		GPIO_Pin_8		//PA
#define MENCODER_BJ2		GPIO_Pin_9		//PA
#define MENCODER_CJ2		GPIO_Pin_10		//PA

//TIM4 REMAPED
#define MENCODER_AJ3		GPIO_Pin_12		//PD
#define MENCODER_BJ3		GPIO_Pin_13		//PD
#define MENCODER_CJ3		GPIO_Pin_14		//PD
/**************************************************************************************/



/************************************uart**********************************************/
//UART1 REMAPPED
#define UART1_GPIO			GPIOB
#define MCOM1_TXD				GPIO_Pin_6		//PB
#define MCOM1_RXD				GPIO_Pin_7		//PB

//UART2
#define UART2_GPIO			GPIOA
#define MCOM2_TXD				GPIO_Pin_2				//PA
#define MCOM2_RXD				GPIO_Pin_3				//PA

//UART3
#define UART3_GPIO			GPIOB
#define MCOM3_TXD				GPIO_Pin_10		//PB
#define MCOM3_RXD				GPIO_Pin_11		//PB

//UART4
#define UART4_GPIO			GPIOC
#define MCOM4_TXD				GPIO_Pin_10		//PC
#define MCOM4_RXD				GPIO_Pin_11		//PC
/**************************************************************************************/


/*************************************motor********************************************/
#define MUD_FWD					GPIO_Pin_8		//PB      //备忘：建议硬件用同组的管脚，这样就能定义 MUD_GPIO了，便于软件代码维护
#define MUD_REV					GPIO_Pin_9		//PB
#define MUD_STOP_MODE		GPIO_Pin_0		//PE
#define MUD_CS					GPIO_Pin_1		//PE

#define MUD_RUN_BREAK 	MUD_FWD
#define MUD_START_STOP	MUD_REV
#define MUD_CW_CCW			MUD_STOP_MODE

#define MLF_FWD					GPIO_Pin_0	//PD
#define MLF_REV					GPIO_Pin_1	//PD
#define MLF_STOP_MODE		GPIO_Pin_2	//PD
#define MLF_CS					GPIO_Pin_3	//PD

#define MLF_RUN_BREAK 	MLF_FWD
#define MLF_START_STOP	MLF_REV
#define MLF_CW_CCW			MLF_STOP_MODE


#define MRG_FWD					GPIO_Pin_5	//PD
#define MRG_REV					GPIO_Pin_6	//PD
#define MRG_STOP_MODE		GPIO_Pin_7	//PD
#define MRG_CS					GPIO_Pin_4	//PD

#define MRG_RUN_BREAK 	MRG_FWD
#define MRG_START_STOP	MRG_REV
#define MRG_CW_CCW			MRG_STOP_MODE


#define MRO_RUN_BREAK		GPIO_Pin_9	//PE
#define MRO_START_STOP	GPIO_Pin_10	//PE
#define MRO_CW_CCW 			GPIO_Pin_11	//PE
#define MRO_CS					GPIO_Pin_8	//PE

/**************************************************************************************/


/************************************spi***********************************************/
#define SPI2_NSS			GPIO_Pin_12		//PB
#define SPI2_SCK			GPIO_Pin_13		//PB
#define SPI2_MISO			GPIO_Pin_14		//PB
#define SPI2_MOSI			GPIO_Pin_15		//PB
/**************************************************************************************/


/*********************************unconfimed********************************************/
#define MLR_SCLK		GPIO_Pin_5			//PA
#define MLR_DI			GPIO_Pin_7			//PA

#define MCUDI0			GPIO_Pin_11			//PD
#define MCUDI1			GPIO_Pin_10			//PD
#define MCUDI2			GPIO_Pin_9			//PD
#define MCUDI3			GPIO_Pin_8			//PD
#define MCUDI4			GPIO_Pin_15			//PE
#define MCUDI5			GPIO_Pin_14			//PE
#define MCUDI6			GPIO_Pin_13			//PE
#define MCUDI7			GPIO_Pin_12			//PE
/**************************************************************************************/

void boardinit(void);
void BspInit(void);

#endif //__BOARD_H 

