#include "tuoluoyi.h"

tuoluoyi_data_t tuoluoyidata = {0};

tuoluoyi_data_t tuoluoyidata_zero = {0};

tuoluoyi_data_t tuoluoyidatatmp[20];
tuoluoyi_info_t  tuoluoyiinfo = {0};

char err = 0;
char zero_flag = 0;

static char tmp[PAKLEN] = {0};
static char index = 0;
static char data_no = 0;
static char FA_flag = 0;
static char data_st_flag = 0;

static int index_f = 0;
static int index_f_flag = 0;

static float chartofloat(char *data)
{
	char tmp[4];
	tmp[0] = *(data + 3);
	tmp[1] = *(data + 2);
	tmp[2] = *(data + 1);
	tmp[3] = *data;
	return *((float *)tmp);
}

static char data_analysis(char *data,tuoluoyi_data_t * tuoluoyidata_p)
{
	
	//jiaoyan
	char i;
	char sum = 0;
	for(i = 0; i < PAKLEN;i ++)
	{
		sum += data[i];
	}
	sum += 0xFF;
	
	if(sum)
	{ 
		err++;
		return 1;
	}
	
	
	tuoluoyidata_p->RollRate = chartofloat(data + 17);
	tuoluoyidata_p->PitchRate = chartofloat(data + 17 + 4);
	tuoluoyidata_p->YawRate = chartofloat(data + 17 + 8);

	tuoluoyidata_p->Xacc = chartofloat(data + 5);
	tuoluoyidata_p->Yacc = chartofloat(data + 5 + 4);
	tuoluoyidata_p->Zacc = chartofloat(data + 5 + 8);
	
	if(zero_flag)
	{
		tuoluoyidata_p->RollRate -= tuoluoyidata_zero.RollRate;
		tuoluoyidata_p->PitchRate -= tuoluoyidata_zero.PitchRate;
		tuoluoyidata_p->YawRate -= tuoluoyidata_zero.YawRate;

		tuoluoyidata_p->Xacc -= tuoluoyidata_zero.Xacc;
		tuoluoyidata_p->Yacc -= tuoluoyidata_zero.Yacc;
		tuoluoyidata_p->Zacc -= tuoluoyidata_zero.Zacc;
		
	}
	
	return 0;
	
	 
}

void Tuoluoyi_data_tan(u8 * data, u8 st, u8 len)
{
	int i,i2;
	

	for(i = st; i < len + st; i++)
	{
		
		i2 = i;
		if(i >= USART_REC_LEN)
				i2 -= USART_REC_LEN;
		if(data_st_flag)
		{
			tmp[index] = *(data + i2);
			index++;
			if(index > PAKLEN - 1)
			{
				data_st_flag = 0;
				
			
				if( 0 == zero_flag)
				{
					if(!data_analysis(tmp,tuoluoyidatatmp + data_no))
							data_no++;
					if(data_no > 19)
					{
						
						int m;
						for(m = 0; m < 20;m++) 
						{
							tuoluoyidata_zero.Xacc += tuoluoyidatatmp[m].Xacc;
							tuoluoyidata_zero.Yacc += tuoluoyidatatmp[m].Yacc;
							tuoluoyidata_zero.Zacc += tuoluoyidatatmp[m].Zacc;
							tuoluoyidata_zero.PitchRate += tuoluoyidatatmp[m].PitchRate;
							tuoluoyidata_zero.RollRate += tuoluoyidatatmp[m].RollRate;
							tuoluoyidata_zero.YawRate += tuoluoyidatatmp[m].YawRate;
						}

						tuoluoyidata_zero.Xacc /= 20;
						tuoluoyidata_zero.Yacc /= 20;
						tuoluoyidata_zero.Zacc /= 20;
						tuoluoyidata_zero.PitchRate /= 20;
						tuoluoyidata_zero.RollRate /= 20;
						tuoluoyidata_zero.YawRate /= 20;
						
						zero_flag = 1;
					}

				}
				else
				{
					data_analysis(tmp,&tuoluoyidata);
					
					tuoluoyiinfo.Xv += tuoluoyidata.Xacc * T;
					tuoluoyiinfo.Yv += tuoluoyidata.Yacc * T;
					tuoluoyiinfo.Zv += tuoluoyidata.Zacc * T;
					
					tuoluoyiinfo.Xl += tuoluoyiinfo.Xv * T + 0.5 * tuoluoyidata.Xacc * T * T;
					tuoluoyiinfo.Yl += tuoluoyiinfo.Yv * T + 0.5 * tuoluoyidata.Yacc * T * T;
					tuoluoyiinfo.Zl += tuoluoyiinfo.Zv * T + 0.5 * tuoluoyidata.Zacc * T * T;
					
					tuoluoyiinfo.roll += tuoluoyidata.RollRate * T * 180 / PI;
					tuoluoyiinfo.pitch += tuoluoyidata.PitchRate * T * 180 / PI;
					tuoluoyiinfo.yaw += tuoluoyidata.YawRate * T * 180 / PI;
				}
 
				index = 0;
			}
		}
		else{
			if(0xFA == *(data + i2))
			{
				FA_flag = 1;
			}
			else 
				if(0XFF == *(data + i2 ))
				{
					if(1 == FA_flag)
						data_st_flag = 1;
					
				}
				else
					FA_flag = 0;

		}
		
	}

}


void tuoluoyi_process(void)
{
	int len = 0;
		len = (USART_RX_STA + USART_REC_LEN - index_f)  % USART_REC_LEN;  // USART_RX_STA : 当前存放字符的位置
																																			// USART_REC_LEN :缓存大小(循环存放)
																																			// index_f ：上次处理到的字符位置
		if(len > PAKLEN + 2)
		{
			Tuoluoyi_data_tan(USART_RX_BUF, index_f, len);
			index_f += len;
			if(index_f >= USART_REC_LEN)
			{
				index_f -= USART_REC_LEN;
				index_f_flag++;
			}
		}
}



