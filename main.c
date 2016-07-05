#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"
#include "camera.h"
#include "tuoluoyi.h"
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
	tuoluoyi_kalman_init();
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
		//delay_ms(5);   //�����ݰ���������ʱ�䣬��ֹ����ӳ���������camera_status.init_flag����Ϊ�ϡ�
		
		if(camera_status.init_flag)
		{
			AGV_pre_set();
			START_BUTTON_IRQ_Set(ENABLE);
			if(AGV_status.init_Directon_flag){
				if(AGV_status.runbutton_status)
				{
					//������ж�
					CAMERA_IRQ_Set(DISABLE);
					Camera_DeInit();
					
					delay_ms(10000);
				
					//�������ǵ��ж�
					TUOLUOYI_IRQ_Set(ENABLE);
					
TUOLUOYI_PROCESS:
				//�������������ݴ������
					tuoluoyi_process();
					
					//�ж���Ʈ����
					if(tuoluoyi_status.zero_flag)
					{
						//��������̨����
						CAMERA_IRQ_Set(ENABLE);
						//��ǰ̨����
						AGV_V_set(ACON_RUN_SPEED);	
						AGV_status.AGV_control_p = &AGV_control_data_8;
						AGV_status.control_st_flag = 1;
						while(1)
						{
						
								camera_process();
								tuoluoyi_process();
	
								if(AGV_status.control_req)
								{
									 AGV_control();
									
								}
								status_printf(&AGV_status);
						//	if(!AGV_status.runbutton_status)
							//	break;
							
						}
					}
					else{
					goto TUOLUOYI_PROCESS;
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
