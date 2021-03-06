#include "camera.h"
#include "gyro.h"
#include "string.h"


u8  CAMERA_RX_BUF[CAMERA_BUF_LEN];   //串口缓存区
int Camera_rx_sta; 									//串口中断程序在缓存区将要填充的位置
camera_status_t camera_status = {0};
static camera_data_t cameradata = {0};
static char tmp[CAMERA_PAKLEN] = {0};
static char data_st_flag = 0;
static char index = 0;							
static int index_buf = 0;						//相机程序将要处理的缓存区位置

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
	
	if( *data != ' ' && *data - '0')
		tmp += (*data -'0') * 100;
	data++;

	if( *data != ' ' && *data - '0')
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

static float camera_data_transfer(float Direction,float dest)
{

	if(0 == AGV_status.runing_towards)
	{
		if(Direction > 180)
		{
			Direction -= 360;
		}
			
		if(dest > 180)
		{
			dest -= 360;
		}
	}
	 dest =  Direction * ACON_CAMERA_ANGLE_WEIGHT + dest * ( 1 - ACON_CAMERA_ANGLE_WEIGHT);
	
	return dest < 0 ? (dest + 360) : dest;
}


static void  camera_data_analysis(char * data)
{
	float Direction = 0;
	cameradata.number = numberdata_analysis(data);
	cameradata.x =  XYdata_analysis(data + 5);
	cameradata.y =  XYdata_analysis(data + 12);
	cameradata.O = Odata_analysis(data + 19);
	
//	__disable_irq();
	AGV_status.X_offset = (cameradata.x - PIC_CENTRE_X) * LEN_PER_PIC / 10;
	AGV_status.Y_offset = (PIC_CENTRE_Y - cameradata.y) * LEN_PER_PIC / 10;
	
	switch(AGV_status.runing_towards)
	{
		case 0:
			AGV_status.X_location  = (cameradata.number / 100) * ACON_LEN_QR + AGV_status.Y_offset - LEN_CAMERA_TO_CENTRE;
			AGV_status.Y_location  = (cameradata.number % 100) * ACON_LEN_QR + AGV_status.X_offset;
			Direction =  cameradata.O > 0 ? 180 - cameradata.O : -(cameradata.O - 180);
			break;
		case 90:
			AGV_status.X_location  = (cameradata.number / 100) * ACON_LEN_QR - AGV_status.X_offset;
			AGV_status.Y_location  = (cameradata.number % 100) * ACON_LEN_QR + AGV_status.Y_offset - LEN_CAMERA_TO_CENTRE;
			Direction = 180 - cameradata.O;
		break;
		case 180:
			AGV_status.X_location  = (cameradata.number / 100) * ACON_LEN_QR - AGV_status.Y_offset + LEN_CAMERA_TO_CENTRE;
			AGV_status.Y_location  = (cameradata.number % 100) * ACON_LEN_QR - AGV_status.X_offset;
			Direction =  180 - cameradata.O;
		break;
		case 270:
			AGV_status.X_location  = (cameradata.number / 100) * ACON_LEN_QR + AGV_status.X_offset;
			AGV_status.Y_location  = (cameradata.number % 100) * ACON_LEN_QR - AGV_status.Y_offset + LEN_CAMERA_TO_CENTRE;
			Direction =  180 - cameradata.O;
		break;
	}
	
	AGV_status.Direction = camera_data_transfer(Direction,AGV_status.Direction);
	AGV_status.Direction_Gyro = camera_data_transfer(Direction,AGV_status.Direction_Gyro);
	AGV_status.Direction_Enco = camera_data_transfer(Direction,AGV_status.Direction_Enco);
		
	PID_data_run.err_now = 0;
	PID_data_run.err_pre_1 = 0;
	PID_data_run.err_pre_2 = 0;
	PID_data_V.err_now = 0;
	PID_data_V.err_pre_1 = 0;
	PID_data_V.err_pre_2 = 0;
	
	Encode_data.Encode_len = 0;
//	__enable_irq();

	camera_status.init_flag = 1; 

}


static void camera_data_tan(u8* data, u8 st, u8 len)
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
				{
					camera_data_analysis(tmp);
				}
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



void camera_process(void)
{
	int len = 0;
	len = (Camera_rx_sta + CAMERA_BUF_LEN - index_buf)  % CAMERA_BUF_LEN ;  
																																			
	if(len)
	{
		camera_data_tan(CAMERA_RX_BUF, index_buf, len);
		index_buf += len;
		if(index_buf >= CAMERA_BUF_LEN)
		{
			index_buf -= CAMERA_BUF_LEN;
		}
	}
}

void Camera_DeInit(void)
{
	memset(CAMERA_RX_BUF,CAMERA_BUF_LEN,0);
	memset(tmp,CAMERA_PAKLEN,0);
	memset(&cameradata,sizeof(cameradata),0);
	Camera_rx_sta = 0;
	data_st_flag = 0;
	index = 0;
	index_buf = 0;
	
}

