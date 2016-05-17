#include "app_config.h"
#include "board.h"
#include "includes.h"
#include "public.h"

#include "stm32f10x_tim.h"


static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  OS_TCB   TaskTCB_1;
static  CPU_STK  TaskStk_1[APP_TASK_START_STK_SIZE];

static  OS_TCB   TaskTCB_2;
static  CPU_STK  TaskStk_2[APP_TASK_START_STK_SIZE];


static  void  AppTaskStart  (void *p_arg);

static void AppTask_1(void *p_arg);
static void AppTask_2(void *p_arg);

int i_1 = 0;
int i_2 = 3;

int main(void)
{
	 OS_ERR  err;  

		boardinit();
	
    CPU_IntDis();                                            /* Disable all interrupts.                              */

    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		BspInit();
		OSSchedRoundRobinCfg(1,1,&err);
    OSStart(&err);                                              
	//	CPU_IntEn();
						 
}



static  void  AppTaskStart (void *p_arg)
{
	OS_ERR err;

 	OSTaskCreate((OS_TCB     *)&TaskTCB_1,                
                 (CPU_CHAR   *)"App Task_1",
                 (OS_TASK_PTR ) AppTask_1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO ,
                 (CPU_STK    *)&TaskStk_1[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
	OSTaskCreate((OS_TCB     *)&TaskTCB_2,                
                 (CPU_CHAR   *)"App Task_2",
                 (OS_TASK_PTR ) AppTask_2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO ,
                 (CPU_STK    *)&TaskStk_2[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

}

static void AppTask_1(void *p_arg)
{
	
	while(1)
	{
		i_1++;
	}

}




static void AppTask_2(void *p_arg)
{
	while(1)
	{
		i_2++;
	}



}

