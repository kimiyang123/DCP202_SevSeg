/*
 * @Date: 2022-05-18 19:57:20
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-24 22:45:06
 * @FilePath: \DCP202_SevSeg\Src\TimTask.c
 * @Description: 定时器和PMW功能初始化
 * 
 */
#include "TimTask.h"


uint8_t taskReadyCnt = 0;	//定时任务就绪数量
timFunList_def timTaskList[TIM_TASK_MAX];


 uint8_t timTask_avaiable(void)
 {
	return TIM_TASK_MAX - taskReadyCnt;
 }


/**
 * @brief TimTask_Init() 定时器任务调度初始化函数，主要用于初始化定时器
 * @return {NULL}
 */
void TimTask_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	// 设置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	// 使能中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}


/**
 * @brief 定时器周期调度 任务添加
 * @param _fun: 需要调度的任务函数,函数指针
 * @param period_t:  任务函数的调度周期 100表示每100ms执行一次
 * @param parm : 任务函数的传递参数，没有则输入 NULL 或 0
 * @return (uint8_t)taskId: 返回任务ID 
 */
uint8_t tim2_addTask(tim_Period_Fun _fun, uint16_t period_t, void *parm)
{
	uint8_t i = 0;
	// TODO >>>> 判断有可用列表空间后再运行
	_fun(parm);		  // 加入列队前先执行一次

	for (i = 0; i < 4; i++)
	{
		// 检测任务ID位置是否已被占用
		if (timTaskList[i].tFun == 0)
		{
			timTaskList[i].tFun = _fun;
			timTaskList[i].PeriodTick = period_t;
			timTaskList[i].param = parm;
			taskReadyCnt ++;
			break; 
		}
	}

	return i;
}

void tim2_delTask(tim_Period_Fun _fun)
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		if (timTaskList[i].tFun == _fun)
		{
			timTaskList[i].tFun = 0;
			timTaskList[i].PeriodTick = 0;
			taskReadyCnt --;
			break;
		}
	}
}

/*******************************************************************************
 * Function Name  : TIM2_IRQHandler
 * Description    : TIM2中断函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
uint32_t tim2Cnt = 0;
void TIM2_IRQHandler(void)
{
	uint8_t i = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 检测中断是否发生;
	{
		tim2Cnt++; // 1ms

		for (i = 0; i < 4; i++)
		{
			if (timTaskList[i].tFun != 0)
			{
				if ((tim2Cnt % timTaskList[i].PeriodTick) == 0)
				{
					timTaskList[i].tFun(timTaskList[i].param);
				}
			}
		}
	}

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断待处理位;
}
