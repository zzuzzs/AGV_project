#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include "camera.h"
#include "gyro.h"
#include "agv.h"
#include "flash.h"

int main(void)
{


	u8 ResetTest,Reset_Flag = 0x55;



	__disable_irq();
	boardinit();
	PID_run_data_init();
	PID_V_data_init();
	AGV_control_data_init();
	Gyro_kalman_init();
	Encode_kalman_init();
	Degree_kalman_init();

	

	sFLASH_ReadBuffer(&ResetTest,ResetTest_ADDR,1);

	if(ResetTest!=0x55)
	{
		sFLASH_Reset();
		sFLASH_WriteBuffer(&Reset_Flag,ResetTest_ADDR,1);
	}
	__enable_irq();
	AGV_status.runbutton_status = 1;
	while(1)
	{
		camera_process();
		//delay_ms(5);   //给数据包传输留出时间，防止相机子程序死锁在camera_status.init_flag的置为上。
		
		if(camera_status.init_flag)
		{
			AGV_pre_set();
			START_BUTTON_IRQ_Set(ENABLE);
			if(AGV_status.init_Directon_flag){
				if(AGV_status.runbutton_status)
				{
					delay_ms(1000);
				
					//开其他后台任务
					GYRO_IRQ_Set(ENABLE);
					
					#ifdef DEBUG
					AGV_status.AGV_control_p = &AGV_control_data_8;
					#endif
					AGV_status.control_st_flag = 1;
		
					//开前台任务
					while(1)
					{
						camera_process();
						Gyro_process();

						if(AGV_status.control_req > 0)
						{
							 AGV_control();
							// status_printf(&AGV_status);	
						}
						
				//	if(!AGV_status.runbutton_status)
					//	break;
					
					}
				
				}
		//	else
			//{
				//camera_status.init_flag = 0;
			//}
			}
		}
	}

}
