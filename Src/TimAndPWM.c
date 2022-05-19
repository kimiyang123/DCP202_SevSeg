

#include "TimAndPWM.h"

void TIM2_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	// 设置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	// 使能中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

uint8_t tim2Fun_cnt = 0;
timFunList_def timfunList[4];

uint8_t tim2_addFun(tim_Period_Fun _fun, uint16_t period_t, void *parm)
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		if (timfunList[i].tFun == 0)
		{
			timfunList[i].tFun = _fun;
			timfunList[i].PeriodTick = period_t;
			timfunList[i].param = parm;
			break;
		}
	}

	return i;
}

void tim2_delFun(tim_Period_Fun _fun)
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		if (timfunList[i].tFun == _fun)
		{
			timfunList[i].tFun = 0;
			timfunList[i].PeriodTick = 0;
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
			if (timfunList[i].tFun != 0)
			{
				if ((tim2Cnt % timfunList[i].PeriodTick) == 0)
				{
					timfunList[i].tFun(timfunList[i].param);
				}
			}
		}
	}

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断待处理位;
}
