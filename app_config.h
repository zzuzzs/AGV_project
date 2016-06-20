#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/**********************************************************/
#define DEBUG

#ifdef DEBUG

#endif  /*DEBUG*/
/**********************************************************/
#define ACON_TIM_CONT  0xffff

#define ACON_SYSIRQ_TIME  1  //系统中断时间 单位ms

#define ACON_V_INIT      0.10   //小车初始速度 单位m/s
#define ACON_ROTATION_SPEED 0.05  //原地转弯的速度  单位m/s
#define ACON_DEGREE_OFFSET 2  //小车转弯时角度容错
#define ACON_DEST_CONTROL_LEN    70  //行驶时的控制点
#define ACON_DEST_LEN_OFFSET    1  //行驶时的控制容错

#define ACON_LEN_QR         80.0  //相邻两个二维码之间的间距 单位cm


#define ACON_NUM_EXP  20  //陀螺仪零飘计算采样数

/***UART***/
#define  ACON_UART1_BAUDRATE  115200
#define  ACON_UART2_BAUDRATE  115200
#define  ACON_UART3_BAUDRATE  115200
#define  ACON_UART4_BAUDRATE  115200

/*********************IRQ Priority******************************/

#define	 CAMERA_IRQ_PRIORITY   1        //相机中断的优先级高于陀螺仪
#define	 DEBUG_USART_IRQ_PRIORITY   1
#define	 TUOLUOYI_IRQ_PRIORITY   3
#define  START_BUTTON_IRQ_PRIORITY   14



#define  ACON_PID_CONTROL_TIME  20 //PID调节时间 单位 ms
#define  ACON_PID_CONTROL_RATE  (1000 / ACON_PID_CONTROL_TIME)    // PID调节频率
#define  ACON_PID_CONTROL_LEN_OFFSET   2 												// PID 调节控制航线偏差，单位cm

#define ACON_PID_KP				0.4
#define ACON_PID_TI				(1/100.0) 



//#define ACON_PID_KP				7.0
//#define ACON_PID_TI				(1/3.0)
//#define ACON_PID_KD				0




/**********************************************************/
#define dest_data_size_t  int


#endif  //__APP_CONFIG_H
