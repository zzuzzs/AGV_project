#include "Gyro.h"
#include "string.h"

int Gyro_rx_sta = 0;
u8  GYRO_RX_BUF[GYRO_BUF_LEN] = {0};
Gyro_info_t  Gyroinfo = {0};
kalman_data_t Gyro_kalman_data = {0};

static char err = 0;

static char Gyro_pag[GYRO_PAKLEN] = {0};
static char index = 0;
static char FA_flag = 0;
static char data_st_flag = 0;

static int index_f = 0;
static int index_f_flag = 0;
static Gyro_data_t Gyrodata = {0};

static float chartofloat(char *data)
{
	char tmp[4];
	tmp[0] = *(data + 3);
	tmp[1] = *(data + 2);
	tmp[2] = *(data + 1);
	tmp[3] = *data;
	return *((float *)tmp);
}

static char data_analysis(char *data,Gyro_data_t * Gyrodata_p)
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
	
	
//	Gyrodata_p->RollRate = chartofloat(data + 20);
	//Gyrodata_p->PitchRate = chartofloat(data + 20 + 4);
	Gyrodata_p->YawRate = chartofloat(data + 20 + 8);

//	Gyrodata_p->Xacc = chartofloat(data + 5);
//	Gyrodata_p->Yacc = chartofloat(data + 5 + 4);
//	Gyrodata_p->Zacc = chartofloat(data + 5 + 8);
	
	return 0;
	 
}

static void Gyro_data_tan(u8 * data, u8 st, u8 len)
{
	int i,i2;

	for(i = st; i < len + st; i++)
	{
		
		i2 = i;
		if(i >= GYRO_BUF_LEN)
				i2 -= GYRO_BUF_LEN;
		if(data_st_flag)
		{
			Gyro_pag[index] = *(data + i2);
			index++;
			if(index > GYRO_PAKLEN - 1)
			{
				data_st_flag = 0;
				data_analysis(Gyro_pag,&Gyrodata);
				Kalman_process(&Gyro_kalman_data);
		//		Gyroinfo.yaw += Gyrodata.YawRate_kalman * T * 180 / PI;
				AGV_status.Direction_Gyro -=  Gyrodata.YawRate_kalman * T * 180 / PI;
				if(AGV_status.Direction_Gyro < 0)
				{
					AGV_status.Direction_Gyro += 360;
				}
				else if(AGV_status.Direction_Gyro > 360)
				{
					AGV_status.Direction_Gyro -= 360;
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


void Gyro_process(void)
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


void Gyro_kalman_init(void)
{
	Gyro_kalman_data.data_type = GYRO_DATA;
	Gyro_kalman_data.P = 0.001;
	Gyro_kalman_data.H.Gyro_Weight = 1;
	Gyro_kalman_data.Q = GMNSTD * GMNSTD;
	Gyro_kalman_data.R.R_Gyro = GMNSTD * GMNSTD;
	Gyro_kalman_data.V.V_Gyro = GMNSTD;
	Gyro_kalman_data.X = &Gyrodata.YawRate_kalman;
	Gyro_kalman_data.EG.Gyro_measure = &Gyrodata.YawRate;
	
}

