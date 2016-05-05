#ifndef __PUBLIC_H
#define __PUBLIC_H

typedef unsigned char u8;


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
}  tuoluoyi_info_t;




typedef struct  {
		int number;
		float x;
		float y;
		float O;
} camera_data_t;


#endif  /*__PUBLIC_H*/

