#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/**********************************************************/
#define DEBUG

#ifdef DEBUG

#endif  /*DEBUG*/
/**********************************************************/
#define ACON_TIM_CONT  0xffff

#define ACON_SYSIRQ_TIME  1  //ϵͳ�ж�ʱ�� ��λms

#define ACON_RUN_SPEED_INIT      0.1  //С����ʼ�ٶ� ��λm/s
#define ACON_RUN_SPEED      0.1      //С���趨�ٶ�
#define ACON_ROTATION_SPEED 0.1  //ԭ��ת����ٶ�  ��λm/s
#define ACON_RUN_STOP_INIT_SPEED      0.1   //С������ͣ���ٶȿ���
#define ACON_MAX_SPEED             0.8    //С����ɵ�����ٶ�
#define ACON_MIN_SPEED             0 //С����ɵ���С�ٶ�
#define ACON_RUN_OR_STOP_ACC       0.05    //С������ʱ�ļ��ٶ�
#define ACON_DEGREE_OFFSET 	1  //С��ת��ʱ�Ƕ��ݴ�
#define ACON_DEST_CONTROL_LEN    220  //��ʻʱ�Ŀ��Ƶ�

#define ACON_DEST_LEN_OFFSET    1  //��ʻʱ�Ŀ����ݴ�

#define ACON_LEN_QR         80.0  //����������ά��֮��ļ�� ��λcm


#define ACON_NUM_EXP  20  //��������Ʈ���������

#define ACON_CAMERA_ANGLE_WEIGHT 0.8   //����Ƕ�ʱ�������Ȩ��

/***UART***/
#define  ACON_UART1_BAUDRATE  115200
#define  ACON_UART2_BAUDRATE  115200
#define  ACON_UART3_BAUDRATE  115200
#define  ACON_UART4_BAUDRATE  115200

/*********************IRQ Priority******************************/

#define	 CAMERA_IRQ_PRIORITY   1        //����жϵ����ȼ�����������
#define	 DEBUG_USART_IRQ_PRIORITY   1
#define	 GYRO_IRQ_PRIORITY   3
#define  START_BUTTON_IRQ_PRIORITY   14
#define  EXIT_IRQ15_10_PRIORITY       2



#define  ACON_PID_CONTROL_TIME  20 //PID����ʱ�� ��λ ms
#define  ACON_PID_CONTROL_RATE  (1000.0 / ACON_PID_CONTROL_TIME)    // PID����Ƶ��
#define  ACON_PID_CONTROL_LEN_OFFSET   2 												// PID ���ڿ��ƺ���ƫ���λcm

#define ACON_PID_RUN_KP				1
#define ACON_PID_RUN_TI				0.5
#define ACON_PID_RUN_TD				0.005


#define ACON_PID_V_KP   12
#define ACON_PID_V_TI  	0.5
#define ACON_PID_V_TD		0.08


#define PNSTD  	 0.001   //ģ�ͱ�׼��
#define EMNSTD 	 0.003   //����������������׼��
#define GMNSTD 	 0.001  //�����ǲ���������׼��
#define RA_ENCO  0.0007    //������С����̬�������
#define RA_GYRO  0.007     //������С����̬�������

/**********************************************************/
#define dest_data_size_t  int


#endif  //__APP_CONFIG_H
