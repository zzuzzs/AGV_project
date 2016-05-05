#include "camera.h"

camera_data_t cameradata = {0};

char tmp[PAKLEN] = {0};


static char data_st_flag = 0;
static char index = 0;

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


void camera_data_tan(char * data, u8 st, u8 len)
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
			if(PAKLEN  == index )
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
}

