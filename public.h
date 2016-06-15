#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "app_config.h" 
#include "board_config.h"
#include "includes.h"

/* Exported types ------------------------------------------------------------*/

typedef volatile struct {
	u8 runbutton_status;    //��־��ʻ��ť״̬
	u8 suspendbutton_status;    //��־��ͣ��ť״̬
	u8 rotating_status;				//��־�Ƿ��ڻ�ת״̬
	u8 runing_status;				//��־�Ƿ���ֱ��״̬
	u8 up_down_status;      // ��־����״̬
	u8 updata_waitting_status; //��־ǰ̨�����Ƿ�ȴ���̨�����жϣ�����С��״̬����
	u8 init_Directon_flag;
	u16 runing_towards;    //С���趨����,����X������н� 0,90,180,270
	u16 encode_right_cnt;
	u16 encode_left_cnt;
	float V_right;			//С�������ٶȲ���ֵ
	float V_left;				//С�������ٶȲ���ֵ
	float X_location;   //X���������ԭ���ʵ��λ��
	float X_offset;			
	float Y_location;		//Y���������ԭ���ʵ��λ��
	float Y_offset;			
	float Directon;			//С���н�������X������ʵ�ʼн�	
	float right_Voltage;
  float left_Voltage;
	
	
} AGV_status_t;

typedef struct {
	dest_data_size_t dest_X;
	dest_data_size_t dest_Y;

} AGV_control_t;


typedef struct {
	float Kp;
	float Ti;
	//float Ki;
	//float Kd;
	float err_now;
	float err_pre_1;
//	float err_pre_2;
		
} PID_data_t;

enum  COMMAND_TYPE{
	PID_VALUE = 1,
	START_BUTTON = 2,
	
};


/* Exported constants --------------------------------------------------------*/
#define  PI 3.1415926




/* Exported macro ------------------------------------------------------------*/
#define PID_KP  ACON_PID_KP
#define PID_TI  ACON_PID_TI
//#define PID_KD  ACON_PID_KD

/* Exported variable --------------------------------------------------------*/


extern AGV_status_t AGV_status;
extern AGV_control_t AGV_control_data;
extern PID_data_t PID_data;

extern u32 systick;

extern u8 command_buf[9];

/* Exported functions ------------------------------------------------------- */
void delay_ms(u32 cnt);
void START_BUTTON_IRQ_Set(FunctionalState status);
void TUOLUOYI_IRQ_Set(FunctionalState status);
void CAMERA_IRQ_Set(FunctionalState status);
void PID_init(void);
float PID_process(PID_data_t * PID_data_p);
void command_process(void);
void status_printf(AGV_status_t *p);
#endif  /*__PUBLIC_H*/

