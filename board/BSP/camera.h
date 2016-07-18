#ifndef __CAMERA_H
#define __CAMERA_H
#include "public.h"
/* Exported types ------------------------------------------------------------*/

typedef struct  {
		int number;
		float x;
		float y;
		float O;
} camera_data_t;



typedef struct {
	u8 init_flag;    //标志初始位置是否有二维码
	int number;
} camera_status_t;

/* Exported constants --------------------------------------------------------*/


#define CAMERA_BUF_LEN  			200	//定义最大接收字节数 200
#define CAMERA_PAKLEN  27
#define LEN_PER_PIC     0.2573  //画面每一像素代表的实际距离,单位 mm
#define PIC_CENTRE_X        372     //二维码在图像正中心时的X坐标
#define PIC_CENTRE_Y       	232	  //二维码在图像正中心时的Y坐标

/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];
extern int Camera_rx_sta;
extern camera_status_t camera_status;

/* Exported functions ------------------------------------------------------- */
void camera_process(void);
void Camera_DeInit(void);
#endif  //__CAMERA_H
