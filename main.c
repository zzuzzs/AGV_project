#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include "camera.h"
#include "tuoluoyi.h"
#include "motor.h"

int main(void)
{

	__disable_irq();
	boardinit();
	PID_init();
	__enable_irq();
	
	while(1)
	{
		camera_process();
		
		if(camera_status.init_flag)
		{
			START_BUTTON_IRQ_Set(ENABLE);
			if(AGV_status.runbutton_status)
			{
				//�������ǵ��ж�
				TUOLUOYI_IRQ_Set(ENABLE);
				
				//�������������ݴ������
				tuoluoyi_process();
				
				//�ж���Ʈ����
				if(tuoluoyi_status.zero_flag)
				{
					//��������̨����
					
					//��ǰ̨����
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
