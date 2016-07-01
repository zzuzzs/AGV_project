#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "app_config.h" 
#include "board_config.h"
#include "includes.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
	dest_data_size_t dest_X;
	dest_data_size_t dest_Y;
	
} dest_data_t;

typedef struct {
	u8 rotating_towards;     //��ת����
	int8_t rotating_degree;  //��ת�Ƕ�
} rotating_data_t;

 typedef union  {
		dest_data_t dest_data;
		rotating_data_t rotating_data;
} AGV_data_t;

typedef struct _AGV_control_ {
	u16 data_type;
	u8  available_flag;
	AGV_data_t data;
	struct _AGV_control_ *next;
} AGV_control_t;


typedef volatile struct {
	u8 runbutton_status;    //��־��ʻ��ť״̬
	u8 suspendbutton_status;    //��־��ͣ��ť״̬
	u8 rotating_status;				//��־�Ƿ��ڻ�ת״̬
	
	u8 runing_status;				//��־�Ƿ���ֱ��״̬
	u8 up_down_status;      // ��־����״̬
	u8 updata_waitting_status; //��־ǰ̨�����Ƿ�ȴ���̨�����жϣ�����С��״̬����
	u8 init_Directon_flag;
	u16 runing_towards;    //С���趨����,����X������˳ʱ��н� 0,90,180,270.��������ϵ
	u16 encode_right_cnt;
	u16 encode_left_cnt;
	float V_right;			//С�������ٶȲ���ֵ ��λm/s
	float V_left;				//С�������ٶȲ���ֵ ��λm/s
	float X_location;   //С��X�᷽���ʵ��λ�ã���λcm����������ϵ
	float Y_location;		//С��Y�᷽���ʵ��λ�ã���λcm����������ϵ
	float X_offset;			//���X�᷽������ڶ�ά���λ�ã�������.��λcm��С������ϵ :С����ǰ��ΪY�������Ҳ�ΪX������
	float Y_offset;			//���Y�᷽������ڶ�ά���λ�ã���λcm��С������ϵ
	float Directon;			//С���н�������X������ʵ�ʼнǣ���λ�ȡ���������ϵ
	float right_Voltage;  //��λ V
  float left_Voltage;		//��λ V
	AGV_control_t *AGV_control_p;
} AGV_status_t;

typedef struct{
	float Degree;
	float Degree_kalman;
} Encode_status_t;



typedef struct {
	float Kp;
	float Ti;
	//float Ki;
	//float Kd;
	float err_now;
	float err_pre_1;
//	float err_pre_2;
		
} PID_data_t;

typedef struct {
	u8 data_type;
	float * X;
	struct {
		float *Encode_measure;
		float *Tuoluoyi_measure;
		}	EG;
	float P;
	struct{
		float Encode_Heght;
		float Tuoluoyi_Heght;
	}H;
	float Q;
	struct{
		float V_Encode;
		float V_Tuoluoyi;
	} V;
	struct {
		float R_Encode;
		float R_Tuoluoyi;
	} R;
} kalman_data_t;


enum  command_type{
	PID_VALUE = 1,
	START_BUTTON = 2,
};

enum control_type{
	RUNING_TYPE = 1,
	STOP_TYPE   = 2,
	ROTATION_TYPE = 3,
};

enum kalman_type{
	TUOLUOYI_DATA = 1,
	ENCODE_DATA  = 2,
	DEGREE_DATA = 3,
};

/* Exported constants --------------------------------------------------------*/
#define  PI 3.1415926




/* Exported macro ------------------------------------------------------------*/
#define PID_RUN_KP  ACON_PID_RUN_KP
#define PID_RUN_TI  ACON_PID_RUN_TI

#define PID_ROTATE_KP 	ACON_PID_ROTATE_KP		
#define PID_ROTATE_TI		ACON_PID_ROTATE_TI

/* Exported variable --------------------------------------------------------*/


extern AGV_status_t AGV_status;
extern AGV_control_t AGV_control_data_1;
extern AGV_control_t AGV_control_data_2;
extern AGV_control_t AGV_control_data_3;
extern AGV_control_t AGV_control_data_4;
extern AGV_control_t AGV_control_data_5;
extern AGV_control_t AGV_control_data_6;
extern AGV_control_t AGV_control_data_7;
extern AGV_control_t AGV_control_data_8;


extern PID_data_t PID_data_run;
extern PID_data_t PID_data_rotate;
extern kalman_data_t Encode_kalman_data;
extern Encode_status_t Encode_status;
extern kalman_data_t Degree_kalman_data;

extern u32 systick;

extern u8 command_buf[9];

/* Exported functions ------------------------------------------------------- */
void delay_ms(u32 cnt);
void START_BUTTON_IRQ_Set(FunctionalState status);
void TUOLUOYI_IRQ_Set(FunctionalState status);
void CAMERA_IRQ_Set(FunctionalState status);
void AGV_control_data_init(void);
void PID_run_init(void);
void PID_rotate_init(void);
float PID_process(PID_data_t * PID_data_p);
float PID_process_tmp(PID_data_t * PID_data_p);
void command_process(void);
void status_printf(AGV_status_t *p);
void Encode_kalman_init(void);
void Degree_kalman_init(void);
void Kalman_process(kalman_data_t * kalman_data_p);
#endif  /*__PUBLIC_H*/

