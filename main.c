#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include "camera.h"
#include "tuoluoyi.h"
#include "motor.h"
#include "flash.h"

int main(void)
{

	u8 ResetTest,Reset_Flag = 0x55;
	__disable_irq();
	boardinit();
	PID_init();
	__enable_irq();
	
	sFLASH_ReadBuffer(&ResetTest,ResetTest_ADDR,1);
	 
	if(ResetTest!=0x55)
    {
			GPIO_SetBits(GPIOA,MCU_LED1);
		  sFLASH_Reset();
			sFLASH_WriteBuffer(&Reset_Flag,ResetTest_ADDR,1);
	  }
		
	while(1)
	{
		camera_process();
		
		if(camera_status.init_flag)
		{
			START_BUTTON_IRQ_Set(ENABLE);
			if(AGV_status.runbutton_status)
			{
				//开陀螺仪的中断
				TUOLUOYI_IRQ_Set(ENABLE);
				
				//运行陀螺仪数据处理程序
				tuoluoyi_process();
				
				//判断零飘采样
				if(tuoluoyi_status.zero_flag)
				{
					//开其他后台任务
					
					//开前台任务
					while(1)
					{
						if(!AGV_status.updata_waitting_status)
						{
							camera_process();
							tuoluoyi_process();
						}
						if(!(systick % ACON_PID_CONTROL_TIME))
						{
							AGV_control(&AGV_control_data);
						}
						
						if(!AGV_status.runbutton_status)
							break;
					}
				}
			}
			else
			{
				camera_status.init_flag = 0;
			}
		}
	
	}
}
