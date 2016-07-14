#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/**********************************************************/
#define DEBUG

#ifdef DEBUG

#endif  /*DEBUG*/
/**********************************************************/
#define ACON_TIM_CONT  0xffff

#define ACON_SYSIRQ_TIME  1  //系统中断时间 单位ms

#define ACON_RUN_SPEED_INIT      0.1  //小车初始速度 单位m/s
#define ACON_RUN_SPEED      0.1      //小车设定速度
#define ACON_ROTATION_SPEED 0.1  //原地转弯的速度  单位m/s
#define ACON_RUN_STOP_INIT_SPEED      0.1   //小车定点停车速度控制
#define ACON_MAX_SPEED             0.8    //小车许可的最大速度
#define ACON_MIN_SPEED             0 //小车许可的最小速度
#define ACON_RUN_OR_STOP_ACC       0.05    //小车变速时的加速度
#define ACON_DEGREE_OFFSET 	1  //小车转弯时角度容错
#define ACON_DEST_CONTROL_LEN    220  //行驶时的控制点

#define ACON_DEST_LEN_OFFSET    1  //行驶时的控制容错

#define ACON_LEN_QR         80.0  //相邻两个二维码之间的间距 单位cm


#define ACON_NUM_EXP  20  //陀螺仪零飘计算采样数

#define ACON_CAMERA_ANGLE_WEIGHT 0.8   //计算角度时相机数据权重

/***UART***/
#define  ACON_UART1_BAUDRATE  115200
#define  ACON_UART2_BAUDRATE  115200
#define  ACON_UART3_BAUDRATE  115200
#define  ACON_UART4_BAUDRATE  115200

/*********************IRQ Priority******************************/

#define	 CAMERA_IRQ_PRIORITY   1        //相机中断的优先级高于陀螺仪
#define	 DEBUG_USART_IRQ_PRIORITY   1
#define	 GYRO_IRQ_PRIORITY   3
#define  START_BUTTON_IRQ_PRIORITY   14
#define  EXIT_IRQ15_10_PRIORITY       2



#define  ACON_PID_CONTROL_TIME  20 //PID调节时间 单位 ms
#define  ACON_PID_CONTROL_RATE  (1000.0 / ACON_PID_CONTROL_TIME)    // PID调节频率
#define  ACON_PID_CONTROL_LEN_OFFSET   2 												// PID 调节控制航线偏差，单位cm

#define ACON_PID_RUN_KP				1
#define ACON_PID_RUN_TI				0.5
#define ACON_PID_RUN_TD				0.005


#define ACON_PID_V_KP   12
#define ACON_PID_V_TI  	0.5
#define ACON_PID_V_TD		0.08


#define PNSTD  	 0.001   //模型标准差
#define EMNSTD 	 0.003   //编码器测量噪声标准差
#define GMNSTD 	 0.001  //陀螺仪测量噪声标准差
#define RA_ENCO  0.0007    //编码器小车姿态测量误差
#define RA_GYRO  0.007     //陀螺仪小车姿态测量误差

/**********************************************************/
#define dest_data_size_t  int


#endif  //__APP_CONFIG_H
