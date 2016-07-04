#include "agv.h"

void AGV_run(void)
{
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
	motor_speed_set(LEFT_MOTOR,ACON_ROTATION_SPEED);
	motor_speed_set(RIGHT_MOTOR,ACON_ROTATION_SPEED);
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

static void V_left_set(float degree_alignment)
{
	float V_Left_targer = 0; 
	V_Left_targer = degree_alignment * PI / 180 * (LEN_WHELLS / 100) * ACON_PID_CONTROL_RATE + AGV_status.V_right;
	motor_speed_set(LEFT_MOTOR,V_Left_targer);

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
		float alignment;
	
		if(len_dest < ACON_DEST_CONTROL_LEN)
		{
			AGV_status.updata_waitting_status = LEN_UPDATA_WRITING;
		}
		//PID 调整
		if(len_offset < ACON_PID_CONTROL_LEN_OFFSET && len_offset > -ACON_PID_CONTROL_LEN_OFFSET)
		{
			//航线偏差在许可范围内，调整航向角，使之为零
			PID_data_run.err_now = -degree_offset / 10.0;
			
		}
		else
		{
			//航线偏差过大，调整航向角，使之在下一个二维码的位置回到航线
			PID_data_run.err_now = -(degree_offset + 180 * len_offset / ACON_LEN_QR / PI) / 10.0;
		}
		alignment = PID_process(&PID_data_run);
		V_left_set(alignment);

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
	int cnt = 0;
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
		cnt = (AGV_status.encode_right_cnt_now - AGV_status.encode_right_cnt_pre + ACON_TIM_CONT) % ACON_TIM_CONT;
		if(AGV_status.rotating_status && RIGHT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			cnt = ACON_TIM_CONT - cnt;
		}
		if(cnt > ACON_TIM_CONT / 2)
			{
				cnt = 0;
			}
		AGV_status.V_right  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000;    //无方向 
		
		cnt = (AGV_status.encode_left_cnt_now - AGV_status.encode_left_cnt_pre + ACON_TIM_CONT) % ACON_TIM_CONT;
		if(AGV_status.rotating_status && LEFT  == AGV_status.AGV_control_p->data.rotating_data.rotating_towards)
		{
			cnt = ACON_TIM_CONT - cnt;
		}
		if(cnt > ACON_TIM_CONT / 2)
		{
			cnt = 0;
		}		
		AGV_status.V_left  = cnt * PI * (D_MOTOR / 100.0) / CON_ENCODE_CNT / ACON_PID_CONTROL_TIME * 1000; //无方向
		
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
				degree_offset =  AGV_status.Directon > 180 ? AGV_status.Directon - 360 : AGV_status.Directon;   //航向角偏差：向左偏位负，向右偏为正
				break;
			case 90:
				len_dest = AGV_status.AGV_control_p->data.dest_data.dest_Y - AGV_status.Y_location;
				degree_offset = AGV_status.Directon - 90;
				break;
			case 180:
				len_dest = AGV_status.X_location - AGV_status.AGV_control_p->data.dest_data.dest_X;
				degree_offset = AGV_status.Directon - 180;
				break;
			case 270:
				len_dest = AGV_status.Y_location - AGV_status.AGV_control_p->data.dest_data.dest_Y;
				degree_offset = AGV_status.Directon - 270;
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
				degree_offset = AGV_status.Directon > 180 ? AGV_status.Directon - 360 : AGV_status.Directon;
				break;
			default:
				degree_offset = AGV_status.Directon - AGV_status.runing_towards;
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
	
	AGV_status.control_req = 0;
	AGV_status.encode_data_available = 0;
}

void AGV_V_set(float speed)
{
	motor_speed_set(RIGHT_MOTOR,speed);
	motor_speed_set(LEFT_MOTOR,speed);
}



void AGV_pre_set(void)
{
	AGV_V_set(0.01);
	if(AGV_status.Directon > 0)
	{
		if(AGV_status.Directon > 0.5)
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
		if(AGV_status.Directon  < -0.5)
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


