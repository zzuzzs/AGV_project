#ifndef  __TUOLUOIYI__H
#define __TUOLUOIYI__H
#include "public.h"

#define USART_REC_LEN  			200	//�����������ֽ��� 200
#define T  0.05             //�����ǲ�������
#define  PI 3.1415926
#define PAKLEN  33

 u8  USART_RX_BUF[USART_REC_LEN];
 
 int USART_RX_STA; 


char err;
char zero_flag;
tuoluoyi_data_t tuoluoyidatatmp[20];

tuoluoyi_info_t  tuoluoyiinfo; 

tuoluoyi_data_t tuoluoyidata;

tuoluoyi_data_t tuoluoyidata_zero;

void Tuoluoyi_data_tan(u8 * rxdata,  u8 st, u8 len);

#endif //__TUOLUOIYI__H

