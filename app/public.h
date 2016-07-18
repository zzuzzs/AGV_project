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
	u8 rotating_towards;     //回转方向
	int8_t rotating_degree;  //回转角度
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


typedef struct {
	u8 runbutton_status;    //标识行驶按钮状态
	u8 suspendbutton_status;    //标识暂停按钮状态
	u8 init_Directon_flag;      //标识小车姿态初始化是否调整完成
	u8 control_st_flag;    //标识是否开始对小车进行控制

	u8 rotating_status;				//标识是否处于回转状态
	u8 runing_status;				//标识是否处于直行状态
	enum{
		PALLET_UP,
		PALLET_DOWN,
	}		pallet_status;      //标识托盘状态
	u8 updata_waitting_status; //标识前台程序是否等待后台程序（中断）更新小车状态数据
	
	u8 control_req_cnt;
	u8 avoid_obj_warnning_cnt;  //避障警告计数
	u8 accident_stop_flag;     //避障意外停车标志
	
	u16 runing_towards;    //小车设定航向,即与X轴正向顺时针夹角 0,90,180,270.地面坐标系
	u16 encode_left_cnt;
	u16 encode_right_cnt;

	
	float V_right;			//小车右轮速度测量值 单位m/s
	float V_left;				//小车左轮速度测量值 单位m/s
	float V_Set;				//小车设定速度  单位m/s
	float X_location;   //小车X轴方向的实际位置，单位cm，地面坐标系
	float Y_location;		//小车Y轴方向的实际位置，单位cm，地面坐标系
	float X_offset;			//相机X轴方向相对于二维码的位置，左负右正.单位cm，小车坐标系 :小车正前方为Y轴正向，右侧为X轴正向
	float Y_offset;			//相机Y轴方向相对于二维码的位置，单位cm，小车坐标系
	float Direction_Gyro;  //由陀螺仪计算出的小车行进方向与X轴正向夹角，单位度。地面坐标系
	float Direction_Enco; //由编码器计算出的小车行进方向与X轴正向夹角，单位度。地面坐标系
	float Direction_kalman;			
	float Direction;			//小车行进方向与X轴正向实际夹角，单位度。地面坐标系
	float right_Voltage;  //单位 V
  float left_Voltage;		//单位 V
	AGV_control_t *AGV_control_p;
} AGV_status_t;

typedef struct{
	float Encode_len;
	float Degree_T_kalman;
	float Degree_T;
} Encode_data_t;



typedef struct {
	float Kp;
	float Ti;
	float Td;
	float err_now;
	float err_pre_1;
	float err_pre_2;
		
} PID_data_t;

typedef struct {
	u8 data_type;
	volatile float * X;
	struct {
		volatile float *Encode_measure;
		volatile float *Gyro_measure;
		}	EG;
	float P;
	struct{
		float Encode_Weight;
		float Gyro_Weight;
	}H;
	float Q;
	#if 0
	struct{
		float V_Encode;
		float V_Gyro;
	} V;
	#endif
	struct {
		float R_Encode;
		float R_Gyro;
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
	PALLET_UP_TYPE = 4,
	PALLET_DOWN_TYPE = 5,
};

enum kalman_type{
	GYRO_DATA = 1,
	ENCODE_DATA  = 2,
	DEGREE_DATA = 3,
};

/* Exported constants --------------------------------------------------------*/
#define  PI 3.1415926




/* Exported macro ------------------------------------------------------------*/
#define PID_RUN_KP  ACON_PID_RUN_KP
#define PID_RUN_TI  ACON_PID_RUN_TI
#define PID_RUN_TD  ACON_PID_RUN_TD

#define PID_V_KP 		ACON_PID_V_KP		
#define PID_V_TI		ACON_PID_V_TI
#define PID_V_TD		ACON_PID_V_TD

/* Exported variable --------------------------------------------------------*/


extern volatile AGV_status_t AGV_status;
extern AGV_control_t AGV_control_data_1;
extern AGV_control_t AGV_control_data_2;
extern AGV_control_t AGV_control_data_3;
extern AGV_control_t AGV_control_data_4;
extern AGV_control_t AGV_control_data_5;
extern AGV_control_t AGV_control_data_6;
extern AGV_control_t AGV_control_data_7;
extern AGV_control_t AGV_control_data_8;


extern PID_data_t PID_data_run;
extern PID_data_t PID_data_V;
extern kalman_data_t Encode_kalman_data;
extern Encode_data_t Encode_data;
extern kalman_data_t Degree_kalman_data;

extern u32 systick;

extern u8 command_buf[9];

/* Exported functions ------------------------------------------------------- */
void delay_ms(u32 cnt);
void START_BUTTON_IRQ_Set(FunctionalState status);
void GYRO_IRQ_Set(FunctionalState status);
void CAMERA_IRQ_Set(FunctionalState status);
void AGV_control_data_init(void);
void PID_run_data_init(void);
void PID_V_data_init(void);
float PID_process(PID_data_t * PID_data_p);
float  PID_process_tmp(PID_data_t * PID_data_p);
void command_process(void);
void status_printf(AGV_status_t *p);
void Encode_kalman_init(void);
void Degree_kalman_init(void);
void Kalman_process(kalman_data_t * kalman_data_p);
void usart_sent(const char *p);
#endif  /*__PUBLIC_H*/

