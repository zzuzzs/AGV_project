#ifndef  __TUOLUOIYI_H
#define __TUOLUOIYI_H
#include "public.h"

/* Exported types ------------------------------------------------------------*/
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
	float Xl_pre;
	float Yl_pre;
	float yaw_pre;
} tuoluoyi_info_t;


typedef volatile struct {
	u8 zero_flag;    //��־��Ʈ�Ƿ��Ѽ���
	u8 clear_flag;   //��־�����������Ƿ���������ӳ�������
} tuoluoyi_status_t;

/* Exported constants --------------------------------------------------------*/

#define TUOLUOYI_BUF_LEN  			200	//�����������ֽ��� 200
#define T  0.05             //�����ǲ�������
#define TUOLUOYI_PAKLEN  33


/* Exported macro ------------------------------------------------------------*/


/* Exported variable --------------------------------------------------------*/
extern u8  TUOLUOYI_RX_BUF[TUOLUOYI_BUF_LEN];
extern int Tuoluoyi_rx_sta;
extern tuoluoyi_status_t tuoluoyi_status;
extern tuoluoyi_info_t  tuoluoyiinfo;

/* Exported functions ------------------------------------------------------- */
void tuoluoyi_process(void);
#endif //__TUOLUOIYI_H

