#include "agv.h"

void AGV_V_set(float speed)
{
	AGV_status.V_Set = speed;
	motor_speed_set(RIGHT_MOTOR,speed);
	motor_speed_set(LEFT_MOTOR,speed);
}


void AGV_run(void)
{
	AGV_V_set(ACON_RUN_SPEED);
	motor_run(LEFT_MOTOR,CW);
	motor_run(RIGHT_MOTOR,CCW);
	AGV_status.runing_status = 1;
	AGV_status.rotating_status = 0;
	
}

void AGV_stop(void)
{
	motor_stop(LEFT_MOTOR);
	motor_stop(RIGHT_MOTOR);
	AGV_status.runing_status = 0;
	AGV_status.rotating_status = 0;
	AGV_status.updata_waitting_status = 0;
}

void AGV_rotate(void)
{
	int tmp = 0;
	AGV_V_set(ACON_ROTATION_SPEED);
//	motor_speed_set(ROTATION_MOTOR,ACON_ROTATION_SPEED);
	switch(AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
	{
		case LEFT:
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CCW);
			//motor_run(ROTATION_MOTOR,CCW);
			tmp = AGV_status.runing_towards - AGV_status.AGV_control_p->data.rotating_data.rotating_degree;
			break;
		case RIGHT:
			motor_run(LEFT_MOTOR,CW);
			motor_run(RIGHT_MOTOR,CW);
			//motor_run(ROTATION_MOTOR,CW);
			tmp = AGV_status.runing_towards + AGV_status.AGV_control_p->data.rotating_data.rotating_degree;
			break;
	}
	if(tmp >= 360)
	{
		tmp -= 360;
	}
	else if(tmp < 0)
	{
		tmp += 360;
	}
		
	AGV_status.updata_waitting_status = DRGREE_UPDATA_WRITING;   //由于陀螺仪默认状态下一直在更新角度状态，因此此赋值暂无意义
	AGV_status.rotating_status = 1;	
	AGV_status.runing_status = 0;
	AGV_status.runing_towards = tmp;
}

static void init_next_run_control(void)
{
	AGV_status.AGV_control_p->available_flag = 0;
	AGV_status.AGV_control_p = AGV_status.AGV_control_p->next;
	switch(AGV_status.AGV_control_p->data_type)
	{
		case RUNING_TYPE:
			AGV_run();
			break;
		case STOP_TYPE:
			AGV_stop();
			break;
		case ROTATION_TYPE:
			AGV_rotate();
			break;
	}
}

void AGV_run_control(float len_offset, float degree_offset,float len_dest)
{
		float alignment1 = 0,alignment2 = 0;
	
		if(len_dest < ACON_DEST_CONTROL_LEN)
		{
			
			AGV_status.updata_waitting_status = LEN_UPDATA_WRITING;
		}
	
		
		PID_data_V.err_now = AGV_status.V_Set - (AGV_status.V_left + AGV_status.V_right) / 2.0;
		PID_data_run.err_now = -(degree_offset + 180 * len_offset / ACON_LEN_QR / PI) / 10.0;
		
		alignment1 = PID_process(&PID_data_V);
		alignment2 = PID_process(&PID_data_run);
			
		motor_speed_set(LEFT_MOTOR,AGV_status.V_Set + alignment1 + alignment2);
		motor_speed_set(RIGHT_MOTOR,AGV_status.V_Set + alignment1 - alignment2);
		

		if(len_dest <  ACON_DEST_LEN_OFFSET)// && len_dest > -ACON_DEST_LEN_OFFSET)
		{
			AGV_stop();
		}
		
}

void AGV_rotating_control(void)
{
	/*
	//PID调节左轮速度，使之与右轮相同，若处于举升状态，托盘同步回转
	float tmp = 0;
	PID_data_rotate.err_now  = AGV_status.V_right - AGV_status.V_left;
	tmp = PID_process_tmp(&PID_data_rotate);
	
	tmp += AGV_status.V_left;
	motor_speed_set(LEFT_MOTOR,tmp);
	*/
	
}

static void AGV_control_pre(void)
{
	int cnt = 0,tmp = 0;
	float LEN_left = 0, LEN_right = 0;
	if(AGV_status.runing_status || AGV_status.rotating_status)
	{
		
		#ifdef DEBUG
		if(AGV_status.runing_status && AGV_status.rotating_status)   //状态冲突检测
		{
			while(1)
			{
				usart_sent("error#2\n");
			}
		}
		#endif  //DEBUG
		tmp = CON_ENCODE_RIGHT->CNT;
		cnt = (tmp - AGV_status.encode_right_cnt + ACON_TIM_CONT) % ACON_TIM_CONT;
		if(AGV_status.rotating_status && RIGHT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			cnt = ACON_TIM_CONT - cnt;
		}
		if(cnt > ACON_TIM_CONT / 2)
			{
				cnt = 0;
			}
		AGV_status.V_right  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000;    //无方向 
		AGV_status.encode_right_cnt = tmp;
		
		tmp = CON_ENCODE_LEFT->CNT;
		cnt = (tmp - AGV_status.encode_left_cnt + ACON_TIM_CONT) % ACON_TIM_CONT;
		if(AGV_status.rotating_status && LEFT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			cnt = ACON_TIM_CONT - cnt;
		}
		if(cnt > ACON_TIM_CONT / 2)
		{
			cnt = 0;
		}		
		AGV_status.V_left  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000; //无方向
		AGV_status.encode_left_cnt = tmp;
		
		LEN_right = AGV_status.V_right * ACON_PID_CONTROL_TIME / 1000 * 100;
		LEN_left = AGV_status.V_left * ACON_PID_CONTROL_TIME / 1000 * 100;
		if(AGV_status.runing_status)
		{
			Encode_status.Degree += (LEN_left - LEN_right) / (PI * 2 * LEN_WHELLS) * 360;
		}
		else if(AGV_status.rotating_status && RIGHT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			Encode_status.Degree += (LEN_left + LEN_right) / (PI * 2 * LEN_WHELLS) * 360;
		}
		else if(AGV_status.rotating_status && LEFT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			Encode_status.Degree += (-LEN_left - LEN_right) / (PI * 2 * LEN_WHELLS) * 360;
		}
		
		if(Encode_status.Degree < 0)
		{
			Encode_status.Degree += 360;
		}
		else if(Encode_status.Degree > 360)
		{
			Encode_status.Degree -= 360;
		}
		Kalman_process(&Encode_kalman_data);
		
		if(LEN_UPDATA_WRITING == AGV_status.updata_waitting_status)
		{
			/*
			if(AGV_status.V_Set > ACON_STOP_SPEED)
			{
				AGV_V_set(AGV_status.V_Set - ACON_RUN_A *  ACON_PID_CONTROL_TIME / 1000.0);
				
			}*/
			LEN_right = AGV_status.V_right * ACON_PID_CONTROL_TIME / 1000 * 100;
			LEN_left = AGV_status.V_left * ACON_PID_CONTROL_TIME / 1000 * 100;
			
			switch(AGV_status.runing_towards)
			{
				case 0:
					AGV_status.X_location += (LEN_right + LEN_left) / 2;
				break;
				case 90:
					AGV_status.Y_location += (LEN_right + LEN_left) / 2;
				break;
				case 180:
					AGV_status.X_location -= (LEN_right + LEN_left) / 2;
				break;
				case 270:
					AGV_status.Y_location -= (LEN_right + LEN_left) / 2;
				break;
			}
		}
	}
}

void AGV_control(void)
{
	AGV_control_pre();
	Kalman_process(&Degree_kalman_data);
	if(AGV_status.runing_status)
	{
		float degree_offset = 0;
		float len_dest = 0;
		float len_offset = 0;
		switch(AGV_status.runing_towards)
		{
			case 0:
				len_dest = AGV_status.AGV_control_p->data.dest_data.dest_X - AGV_status.X_location;  //始终为正
				degree_offset =  AGV_status.Direction > 180 ? AGV_status.Direction - 360 : AGV_status.Direction;   //航向角偏差：向左偏位负，向右偏为正
				break;
			case 90:
				len_dest = AGV_status.AGV_control_p->data.dest_data.dest_Y - AGV_status.Y_location;
				degree_offset = AGV_status.Direction - 90;
				break;
			case 180:
				len_dest = AGV_status.X_location - AGV_status.AGV_control_p->data.dest_data.dest_X;
				degree_offset = AGV_status.Direction - 180;
				break;
			case 270:
				len_dest = AGV_status.Y_location - AGV_status.AGV_control_p->data.dest_data.dest_Y;
				degree_offset = AGV_status.Direction - 270;
				break;
		}
		len_offset = AGV_status.X_offset;      //左偏为负，右偏为正 
		AGV_run_control(len_offset,degree_offset,len_dest);
		
	}

	if(AGV_status.rotating_status)
	{
		float degree_offset = 0;
		
		AGV_rotating_control();
		
		
		switch(AGV_status.runing_towards)
		{
			case 0:
				degree_offset = AGV_status.Direction > 180 ? AGV_status.Direction - 360 : AGV_status.Direction;
				break;
			default:
				degree_offset = AGV_status.Direction - AGV_status.runing_towards;
		}
		if(degree_offset < 1 && degree_offset > -1)
		{
			AGV_stop();
		}

	}
	
	if(!(AGV_status.rotating_status || AGV_status.runing_status))
	{
		
		//停止状态下，下步行动的部署
		if(AGV_status.AGV_control_p->next && AGV_status.AGV_control_p->next->available_flag)
			{
				init_next_run_control();
			}
		
	}
	__disable_irq();
	AGV_status.control_req = 0;
	__enable_irq();
}




void AGV_pre_set(void)
{
	float angle_offset = AGV_status.Direction;
	if(AGV_status.runing_towards == 0 && angle_offset > 180)
	{
		angle_offset -= 360;
	}
	angle_offset -= AGV_status.runing_towards;
	AGV_V_set(0.01);
	if( angle_offset > 0)
	{
		if(angle_offset > 0.5)
		{
			motor_run(LEFT_MOTOR,CCW);
			motor_run(RIGHT_MOTOR,CCW);
		}
		else
		{
			AGV_status.init_Directon_flag = 1;
		}
	}
	else
	{
		if(angle_offset < -0.5)
		{
			motor_run(LEFT_MOTOR,CW);
			motor_run(RIGHT_MOTOR,CW);
			
		}
		else
		{
			AGV_status.init_Directon_flag = 1;
		}
	}
	if(AGV_status.init_Directon_flag)
	{
		motor_stop(LEFT_MOTOR);
		motor_stop(RIGHT_MOTOR);
		CON_ENCODE_LEFT->CNT  = 0;
		CON_ENCODE_RIGHT->CNT  = 0;
		
	}

}


