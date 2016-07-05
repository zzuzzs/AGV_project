#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/**********************************************************/
#define DEBUG

#ifdef DEBUG

#endif  /*DEBUG*/
/**********************************************************/
#define ACON_TIM_CONT  0xffff

#define ACON_SYSIRQ_TIME  1  //ϵͳ�ж�ʱ�� ��λms

#define ACON_RUN_SPEED      0.3   //С����ʼ�ٶ� ��λm/s
#define ACON_ROTATION_SPEED 0.1  //ԭ��ת����ٶ�  ��λm/s
#define ACON_STOP_SPEED      0.1   //С������ͣ���ٶȿ���
#define ACON_RUN_A          0.1   //С������ʱ�ļ��ٶ�
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
#define  ACON_PID_CONTROL_RATE  (1000.0 / ACON_PID_CONTROL_TIME)    // PID����Ƶ��
#define  ACON_PID_CONTROL_LEN_OFFSET   2 												// PID ���ڿ��ƺ���ƫ���λcm

#define ACON_PID_RUN_KP				0.4826
#define ACON_PID_RUN_TI				0.5
#define ACON_PID_RUN_TD				0.005


#define ACON_PID_V_KP   1.9304
#define ACON_PID_V_TI  0.5
#define ACON_PID_V_TD	0.08


#define PNSTD  0.001   //ģ�ͱ�׼��
#define EMNSTD 0.003   //����������������׼��
#define GMNSTD 0.0016  //�����ǲ���������׼��
#define RA_ENCO  0.0007    //������С����̬�������
#define RA_GYRO  0.007     //������С����̬�������

/**********************************************************/
#define dest_data_size_t  int


#endif  //__APP_CONFIG_H
