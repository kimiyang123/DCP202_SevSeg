#ifndef __TIM_AND_PWM_H__
#define __TIM_AND_PWM_H__

#include "stm32f10x.h"


#define TIM_TASK_MAX	4		//定义定时任务最大数量

typedef void (*tim_Period_Fun)();

typedef struct timFun_List
{
	tim_Period_Fun tFun;
	uint16_t PeriodTick;
	void *param;
} timFunList_def;


/**
 * @brief : 任务初始方法
 * @description: 
 * @return {void}
 */
void TimTask_Init(void);

/**
 * @brief 定时器周期调度 任务添加
 * @param _fun: 需要调度的任务函数,函数指针
 * @param period_t:  任务函数的调度周期 100表示每100ms执行一次
 * @param parm : 任务函数的传递参数，没有则输入 NULL 或 0
 * @return (uint8_t)taskId: 返回任务ID 
 */
uint8_t tim2_addTask(tim_Period_Fun _fun, uint16_t period_t, void *parm);

/**
 * @brief : 定时任务删除方法
 * @description: 
 * @param _fun :需要删除的任务函数指针
 * @return {void}
 */
void tim2_delTask(tim_Period_Fun _fun);




#endif
