#include "camera.h"


u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];
int Camera_rx_sta;


static camera_data_t cameradata = {0};
static char tmp[CAMERA_PAKLEN] = {0};
static char data_st_flag = 0;
static char index = 0;
static int index_f = 0;

static float numberdata_analysis(char *data)
{
	float tmp = 0;
	if(*data - '0')
		tmp += (*data -'0') * 1000;
	data++;

	if(*data - '0')
		tmp += (*data -'0') * 100;
	data++;

	if(*data - '0')
		tmp += (*data -'0') * 10;
	data++;
	
	if(*data - '0')
		tmp += *data -'0';
	
	return tmp;

}


static float XYdata_analysis(char *data)
{
	float tmp = 0;
	
	if(*data - '0')
		tmp += (*data -'0') * 100;
	data++;

	if(*data - '0')
		tmp += (*data -'0') * 10;
	data++;
	
	if(*data - '0')
		tmp += *data -'0';
	
	data += 2;
	
	if(*data - '0')
		tmp += (*data -'0') / 10.0;
	data++;
	
	if(*data - '0')
		tmp += (*data -'0') / 100.0;
	
	return tmp;
	
}

static float Odata_analysis(char *data)
{
	float tmp = 0;
	data = data + 6;
	
	if(*data - '0')
		tmp += (*data -'0') / 100.0;

	data--;
	
	if(*data - '0')
		tmp += (*data -'0') / 10.0;

	data -= 2;
	
	if(*data - '0')
		tmp += *data -'0';
	
	data--;
	
	if(*data != '-' && *data != ' ')
		tmp += (*data -'0') * 10;
	else
		goto END;
	
	data--;
	
	
	if(*data != '-' && *data != ' ')
		tmp += (*data -'0') * 100;
	else
		goto END;

	data--;
	
END:	
	if(*data == '-')
		return -tmp;
	else return tmp;
			
}


static void  data_analysis(char * data)
{
	cameradata.number = numberdata_analysis(data);
	cameradata.x =  XYdata_analysis(data + 5);
	cameradata.y =  XYdata_analysis(data + 12);
	cameradata.O = Odata_analysis(data + 19);
}


camera_data_t* camera_data_tan(u8* data, u8 st, u8 len)
{
	
	int i,i2;

	for(i = st; i < len + st; i++)
	{
		
		i2 = i;
		if(i >= CAMERA_BUF_LEN)
				i2 -= CAMERA_BUF_LEN;
		
		if(data_st_flag)
		{
			tmp[index] = *(data + i2);
			index++;
			if(CAMERA_PAKLEN  == index )
			{
				if(*(data + i2) == 0x61)
					data_analysis(tmp);
				data_st_flag = 0;
				index = 0;
			}
				
		}
		else
		{
			if(0x24 == *(data + i2))
			{
				data_st_flag = 1;
			}
		}
		
	}	
	return &cameradata;
}



void camera_process(void)
{
	int len = 0;
	len = (Camera_rx_sta + CAMERA_BUF_LEN - index_f)  % CAMERA_BUF_LEN ;  
																																			
	if(len > CAMERA_PAKLEN + 1)
	{
		camera_data_tan(CAMERA_RX_BUF, index_f, len);
		index_f += len;
		if(index_f >= CAMERA_BUF_LEN)
		{
			index_f -= CAMERA_BUF_LEN;
		}
	}
}

