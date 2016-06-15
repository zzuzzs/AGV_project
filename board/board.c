#include "board_config.h"
#include "board.h"
#include "includes.h"

static void RCC_Configuration(void)
{
	u8 ret;
		
	/*复位时钟配置*/
	RCC_DeInit();
	
	/*使能外部时钟*/
	RCC_HSEConfig(RCC_HSE_ON);
	
	/*等待外部时钟稳定*/
	ret = RCC_WaitForHSEStartUp();
	if(ERROR == ret)
	{
		/*时钟出错处理*/
	}
	
	/*将PLL作为系统主时钟，同时关闭HSI*/
	RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	if(0x08 ==  RCC_GetSYSCLKSource()){
		RCC_HSICmd(DISABLE);
	}
	else{
		/*与预期不符时的处理*/
	}
	
	/*配置外设总时钟*/
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
}


static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	/*初始为复位时的状态（不是必须的）*/
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		
	
	/*motor*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0xFF;     //GPIO_Pin0 ~ GPIO_Pin7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0x3 | (0xf << 8);   //GPIO_Pin0,GPIO_Pin1,GPIO_Pin8 ~ GPIO_Pin11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,MLF_CS);
	GPIO_SetBits(GPIOD,MRG_CS);
	GPIO_SetBits(GPIOE,MUD_CS);
	GPIO_SetBits(GPIOE,MRO_CS);
	
	/*uart*/
	//uart1
	GPIO_InitStructure.GPIO_Pin = MCOM1_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(UART1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCOM1_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);
	/* uart1管脚重映射*/
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	
	//uart2
	GPIO_InitStructure.GPIO_Pin = MCOM2_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(UART2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCOM2_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART2_GPIO, &GPIO_InitStructure);
	
	//uart3
	GPIO_InitStructure.GPIO_Pin = MCOM3_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(UART3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCOM3_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART3_GPIO, &GPIO_InitStructure);
	
	//uart4
	GPIO_InitStructure.GPIO_Pin = MCOM4_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(UART4_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCOM4_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART4_GPIO, &GPIO_InitStructure);
	
	
	
	/*TIM*/
	//TIM1
	GPIO_InitStructure.GPIO_Pin = MENCODER_AJ2 | MENCODER_BJ2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	//TIM3
	GPIO_InitStructure.GPIO_Pin = MENCODER_AJ1 | MENCODER_BJ1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*管脚REMAP*/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	//TIM4
	GPIO_InitStructure.GPIO_Pin = MENCODER_AJ3 | MENCODER_BJ3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*管脚REMAP*/
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);

	/*SPI1*/
	GPIO_InitStructure.GPIO_Pin = MLR_SCLK | MLR_DI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
		
	/*SPI2*/
	GPIO_InitStructure.GPIO_Pin = SPI2_SCK | SPI2_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 
	GPIO_InitStructure.GPIO_Pin = SPI2_NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,SPI2_NSS);
	
	
	
	/*misc*/
	//LED
	GPIO_InitStructure.GPIO_Pin = MCU_LED1 | MCU_LED2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCUKEY1LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCUKEY2LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//Interrupt
	/*
	#define MCUKEY1				GPIO_Pin_0 			//PA
	#define MCUKEY2				GPIO_Pin_1			//PA
	#define MENCODER_CJ1		GPIO_Pin_8		//PC
	#define MENCODER_CJ2		GPIO_Pin_10		//PA
	#define MENCODER_CJ3		GPIO_Pin_14		//PD
	#define MCUDI0			GPIO_Pin_11			//PD
	#define MCUDI1			GPIO_Pin_10			//PD 	//失能
	#define MCUDI2			GPIO_Pin_9			//PD
	#define MCUDI3			GPIO_Pin_8			//PD	//失能
	#define MCUDI4			GPIO_Pin_15			//PE
	#define MCUDI5			GPIO_Pin_14			//PE	//失能
	#define MCUDI6			GPIO_Pin_13			//PE
	#define MCUDI7			GPIO_Pin_12			//PE
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource14);
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource11);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource15);
	
}

static void TIM_Configuration(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM1,ACON_TIM_CONT);
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3,ACON_TIM_CONT);
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM4,ACON_TIM_CONT);
	TIM_Cmd(TIM4,ENABLE);
	
}


/*使用库默认配置，波特率为9600*/
static void UART_Configuration(void)
{
	USART_InitTypeDef USART_InitStruct  = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_Init(USART1,&USART_InitStruct);
	USART_Init(USART2,&USART_InitStruct);
	USART_Init(USART3,&USART_InitStruct);
	USART_Init(UART4,&USART_InitStruct);
	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_Cmd(UART4,ENABLE);
	
}
static void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; 
	SPI_InitStruct.SPI_DataSize  = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_NSS =  SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
	
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStruct);
  SPI_Cmd(SPI2, ENABLE);

	
}

static void Interrupt_Configuration(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	EXTI_StructInit(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = START_BUTTON_IRQ_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = STOP_BUTTON_IRQ_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
#if 0	
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line8;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line15;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAMERA_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAMERA_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_IRQ_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	

#if 0
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
	
}


static void SysTickInit (u32  cnts)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->LOAD = cnts - 1u;
	SysTick->CTRL |= 3;   /* Enable timer && Enable timer interrupt*/
}



void boardinit(void)
{
	RCC_Configuration();
	
	GPIO_Configuration();
	
	TIM_Configuration();
	
	UART_Configuration();
	
	SPI_Configuration();
	
	Interrupt_Configuration();
	
	SysTickInit(CNT_STK);
	
}

