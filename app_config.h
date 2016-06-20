#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/**********************************************************/
#define DEBUG

#ifdef DEBUG

#endif  /*DEBUG*/
/**********************************************************/
#define ACON_TIM_CONT  0xffff

#define ACON_SYSIRQ_TIME  1  //ϵͳ�ж�ʱ�� ��λms

#define ACON_V_INIT      0.10   //С����ʼ�ٶ� ��λm/s
#define ACON_ROTATION_SPEED 0.05  //ԭ��ת����ٶ�  ��λm/s
#define ACON_DEGREE_OFFSET 2  //С��ת��ʱ�Ƕ��ݴ�
#define ACON_DEST_CONTROL_LEN    70  //��ʻʱ�Ŀ��Ƶ�
#define ACON_DEST_LEN_OFFSET    1  //��ʻʱ�Ŀ����ݴ�

#define ACON_LEN_QR         80.0  //����������ά��֮��ļ�� ��λcm


#define ACON_NUM_EXP  20  //��������Ʈ���������

/***UART***/
#define  ACON_UART1_BAUDRATE  115200
#define  ACON_UART2_BAUDRATE  115200
#define  ACON_UART3_BAUDRATE  115200
#define  ACON_UART4_BAUDRATE  115200

/*********************IRQ Priority******************************/

#define	 CAMERA_IRQ_PRIORITY   1        //����жϵ����ȼ�����������
#define	 DEBUG_USART_IRQ_PRIORITY   1
#define	 TUOLUOYI_IRQ_PRIORITY   3
#define  START_BUTTON_IRQ_PRIORITY   14



#define  ACON_PID_CONTROL_TIME  20 //PID����ʱ�� ��λ ms
#define  ACON_PID_CONTROL_RATE  (1000 / ACON_PID_CONTROL_TIME)    // PID����Ƶ��
#define  ACON_PID_CONTROL_LEN_OFFSET   2 												// PID ���ڿ��ƺ���ƫ���λcm

#define ACON_PID_KP				0.4
#define ACON_PID_TI				(1/100.0) 



//#define ACON_PID_KP				7.0
//#define ACON_PID_TI				(1/3.0)
//#define ACON_PID_KD				0




/**********************************************************/
#define dest_data_size_t  int


#endif  //__APP_CONFIG_H
