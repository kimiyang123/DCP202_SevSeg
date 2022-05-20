#ifndef __TIM_AND_PWM_H__
#define __TIM_AND_PWM_H__

    #include "stm32f10x.h"



		typedef void (*tim_Period_Fun)();
		
		typedef struct timFun_List{
			tim_Period_Fun 	tFun;
			uint16_t 				PeriodTick;
			void 						*param;
		}timFunList_def;
		
		
		void TIM2_Init(void);
		uint8_t tim2_addTask( tim_Period_Fun _fun, uint16_t period_t, void* parm );
		void  	tim2_delTask(tim_Period_Fun _fun);
		

#endif

