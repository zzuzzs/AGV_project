#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "app_config.h" 
#include "board_config.h"
#include "includes.h"

/* Exported types ------------------------------------------------------------*/

typedef volatile struct {
	u8 runbutton_status;    //标志行驶按钮状态
	u8 suspendbutton_status;    //标志暂停按钮状态
	u8 rotating_status;				//标志是否处于回转状态
	u8 rotating_towards;     //回转方向
	int8_t rotating_degree;  //回转角度
	u8 runing_status;				//标志是否处于直行状态
	u8 up_down_status;      // 标志托盘状态
	u8 updata_waitting_status; //标志前台程序是否等待后台程序（中断）更新小车状态数据
	u8 init_Directon_flag;
	 
	u16 runing_towards;    //小车设定航向,即与X轴正向顺时针夹角 0,90,180,270.地面坐标系
	u16 encode_right_cnt;
	u16 encode_left_cnt;
	float V_right;			//小车右轮速度测量值 单位m/s
	float V_left;				//小车左轮速度测量值 单位m/s
	float X_location;   //小车X轴方向的实际位置，单位cm，地面坐标系
	float Y_location;		//小车Y轴方向的实际位置，单位cm，地面坐标系
	float X_offset;			//相机X轴方向相对于二维码的位置，左负右正.单位cm，小车坐标系 :小车正前方为X轴正向，右侧为Y轴正向
	float Y_offset;			//相机Y轴方向相对于二维码的位置，单位cm，小车坐标系
	float Directon;			//小车行进方向与X轴正向实际夹角，单位度。地面坐标系
	float right_Voltage;  //单位 V
  float left_Voltage;		//单位 V
	
	
} AGV_status_t;

typedef struct _AGV_control_ {
	dest_data_size_t dest_X;
	dest_data_size_t dest_Y;
	struct _AGV_control_ *next;

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
extern AGV_control_t AGV_control_data_1;
extern AGV_control_t AGV_control_data_2;
extern AGV_control_t AGV_control_data_3;
extern AGV_control_t AGV_control_data_4;
extern AGV_control_t *AGV_control_data_now;
extern PID_data_t PID_data;

extern u32 systick;

extern u8 command_buf[9];

/* Exported functions ------------------------------------------------------- */
void delay_ms(u32 cnt);
void START_BUTTON_IRQ_Set(FunctionalState status);
void TUOLUOYI_IRQ_Set(FunctionalState status);
void CAMERA_IRQ_Set(FunctionalState status);
void AGV_control_data_init(void);
void PID_init(void);
float PID_process(PID_data_t * PID_data_p);
void command_process(void);
void status_printf(AGV_status_t *p);
#endif  /*__PUBLIC_H*/

