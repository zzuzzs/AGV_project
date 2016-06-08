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



typedef volatile struct {
	u8 init_flag;    //��־��ʼλ���Ƿ��ж�ά��
} camera_status_t;

/* Exported constants --------------------------------------------------------*/


#define CAMERA_BUF_LEN  			200	//�����������ֽ��� 200
#define CAMERA_PAKLEN  27
#define LEN_PER_PIC     0.2451  //����ÿһ���ش�����ʵ�ʾ���,��λ mm
#define PIC_CENTRE_X        240     //��ά����ͼ��������ʱ��X����
#define PIC_CENTRE_Y        376		  //��ά����ͼ��������ʱ��Y����

/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];
extern int Camera_rx_sta;
extern camera_status_t camera_status;

/* Exported functions ------------------------------------------------------- */
void camera_process(void);
#endif  //__CAMERA_H