#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "app_config.h" 
#include "board_config.h"
#include "includes.h"


#define  PI 3.1415926

typedef struct tuoluoyi_data{
	
	float Xacc;
	float Yacc;
	float Zacc;
	float RollRate;
	float PitchRate;
	float YawRate;
} tuoluoyi_data_t;

typedef struct {
	float Xv;
	float Yv;
	float Zv;
	float Xl;
	float Yl;
	float Zl;
	float roll;
	float pitch;
	float yaw;
} tuoluoyi_info_t;


typedef struct  {
		int number;
		float x;
		float y;
		float O;
} camera_data_t;


typedef volatile struct {
	u8 rotating;
	u8 runing;
	u8 X_location;
	u8 X_offset;
	u8 Y_location;
	u8 Y_offset;
	u16 Directon;
} AGV_status_t;



extern AGV_status_t AGV_status;

#endif  /*__PUBLIC_H*/

