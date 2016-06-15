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
	u8 runing_status;				//标志是否处于直行状态
	u8 up_down_status;      // 标志托盘状态
	u8 updata_waitting_status; //标志前台程序是否等待后台程序（中断）更新小车状态数据
	u8 init_Directon_flag;
	u16 runing_towards;    //小车设定航向,即与X轴正向夹角 0,90,180,270
	u16 encode_right_cnt;
	u16 encode_left_cnt;
	float V_right;			//小车右轮速度测量值
	float V_left;				//小车左轮速度测量值
	float X_location;   //X轴向相对于原点的实际位置
	float X_offset;			
	float Y_location;		//Y轴向相对于原点的实际位置
	float Y_offset;			
	float Directon;			//小车行进方向与X轴正向实际夹角	
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

