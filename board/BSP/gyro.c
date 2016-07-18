#include "gyro.h"

gyro_data_t gyrodata = {0};
u8  GYRO_RX_BUF[GYRO_BUF_LEN] = {0};
int Gyro_rx_sta = 0;

static char err = 0;
static char zero_flag = 0;

static char tmp[GYRO_PAKLEN] = {0};
static char index = 0;
static char data_no = 0;
static char FA_flag = 0;
static char data_st_flag = 0;

static int index_f = 0;
static int index_f_flag = 0;

static gyro_data_t gyrodata_zero = {0};
static gyro_data_t gyrodatatmp[20];
static gyro_info_t  gyroinfo = {0};

static float chartofloat(char *data)
{
	char tmp[4];
	tmp[0] = *(data + 3);
	tmp[1] = *(data + 2);
	tmp[2] = *(data + 1);
	tmp[3] = *data;
	return *((float *)tmp);
}

static char data_analysis(char *data,gyro_data_t * gyrodata_p)
{
	
	//jiaoyan
	char i;
	char sum = 0;
	for(i = 0; i < GYRO_PAKLEN;i ++)
	{
		sum += data[i];
	}
	sum += 0xFF;
	
	if(sum)
	{ 
		err++;
		return 1;
	}
	
	
	gyrodata_p->RollRate = chartofloat(data + 17);
	gyrodata_p->PitchRate = chartofloat(data + 17 + 4);
	gyrodata_p->YawRate = chartofloat(data + 17 + 8);

	gyrodata_p->Xacc = chartofloat(data + 5);
	gyrodata_p->Yacc = chartofloat(data + 5 + 4);
	gyrodata_p->Zacc = chartofloat(data + 5 + 8);
	
	if(zero_flag)
	{
		gyrodata_p->RollRate -= gyrodata_zero.RollRate;
		gyrodata_p->PitchRate -= gyrodata_zero.PitchRate;
		gyrodata_p->YawRate -= gyrodata_zero.YawRate;

		gyrodata_p->Xacc -= gyrodata_zero.Xacc;
		gyrodata_p->Yacc -= gyrodata_zero.Yacc;
		gyrodata_p->Zacc -= gyrodata_zero.Zacc;
		
	}
	
	return 0;
	 
}

void Gyro_data_tan(u8 * data, u8 st, u8 len)
{
	int i,i2;
	

	for(i = st; i < len + st; i++)
	{
		
		i2 = i;
		if(i >= GYRO_BUF_LEN)
				i2 -= GYRO_BUF_LEN;
		if(data_st_flag)
		{
			tmp[index] = *(data + i2);
			index++;
			if(index > GYRO_PAKLEN - 1)
			{
				data_st_flag = 0;
				
			
				if( 0 == zero_flag)
				{
					if(!data_analysis(tmp,gyrodatatmp + data_no))
							data_no++;
					if(data_no > 19)
					{
						
						int m;
						for(m = 0; m < 20;m++) 
						{
							gyrodata_zero.Xacc += gyrodatatmp[m].Xacc;
							gyrodata_zero.Yacc += gyrodatatmp[m].Yacc;
							gyrodata_zero.Zacc += gyrodatatmp[m].Zacc;
							gyrodata_zero.PitchRate += gyrodatatmp[m].PitchRate;
							gyrodata_zero.RollRate += gyrodatatmp[m].RollRate;
							gyrodata_zero.YawRate += gyrodatatmp[m].YawRate;
						}

						gyrodata_zero.Xacc /= 20;
						gyrodata_zero.Yacc /= 20;
						gyrodata_zero.Zacc /= 20;
						gyrodata_zero.PitchRate /= 20;
						gyrodata_zero.RollRate /= 20;
						gyrodata_zero.YawRate /= 20;
						
						zero_flag = 1;
					}

				}
				else
				{
					data_analysis(tmp,&gyrodata);
					
					gyroinfo.Xv += gyrodata.Xacc * T;
					gyroinfo.Yv += gyrodata.Yacc * T;
					gyroinfo.Zv += gyrodata.Zacc * T;
					
					gyroinfo.Xl += gyroinfo.Xv * T + 0.5 * gyrodata.Xacc * T * T;
					gyroinfo.Yl += gyroinfo.Yv * T + 0.5 * gyrodata.Yacc * T * T;
					gyroinfo.Zl += gyroinfo.Zv * T + 0.5 * gyrodata.Zacc * T * T;
					
					gyroinfo.roll += gyrodata.RollRate * T * 180 / PI;
					gyroinfo.pitch += gyrodata.PitchRate * T * 180 / PI;
					gyroinfo.yaw += gyrodata.YawRate * T * 180 / PI;
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


void gyro_process(void)
{
	int len = 0;
	len = ( Gyro_rx_sta + GYRO_BUF_LEN - index_f)  % GYRO_BUF_LEN;  //  Gyro_rx_sta : 当前存放字符的位置
																																			// GYRO_BUF_LEN :缓存大小(循环存放)
																																			// index_f ：上次处理到的字符位置
		if(len > GYRO_PAKLEN + 2)
		{
			Gyro_data_tan(GYRO_RX_BUF, index_f, len);
			index_f += len;
			if(index_f >= GYRO_BUF_LEN)
			{
				index_f -= GYRO_BUF_LEN;
				index_f_flag++;
			}
		}
}



