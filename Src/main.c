

#include "stm32f10x.h" // Device header

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimTask.h"
#include "Motor_HPWM.h"

int num;

void fundemo(void)
{
	static uint16_t i = 0;
	SMG_ShowInt(i, 0, 4);
	i++;
}

void LED1_flash()
{
	LED1_Toggle();
}

void LED2_flash()
{
	LED2_Toggle();
}

unsigned char State = 0, perState = 0xff;
int main(void)
{
	SysTick_Config(72000);

	BSP_Configuration();
	BSP_SMG_Init();
	keyPad_Init();
	keyIndepend_Init();

	//TIM2_Init();

	Motor_PortInit();

	int pwmv = 0;
	TIM1->CCR3 = 1;
	TIM1->CCR2 = 100;
	while (1);
	{
		if(pwmv ++ >= 100)
		{
			pwmv = 0;

			// TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Disable);
			// TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Disable);

		}
		TIM1->CCR2 = pwmv;
		delay_ticks(100);

		

		/* code */
	}
	

	uint8_t flag = 0;
	uint16_t thisKey = 0;

	tim2_addTask(SMG_Refresh, 3, NULL);
	// tim2_addTask((tim_Period_Fun)SMG_RotateShift,1000,0);
	Beep_On(100);
	char *numStr;

	uint8_t setflas = 0,perSetflas;
	while (1)
	{
		keyPad_Event();
		switch (setflas)
		{
		case 0:

			SMG_print("S",0);
			if(0 == app_setArea() && getKeyLast() == 1) setflas = 1;
			break;
		case 1:
			SMG_print("hello",0);

			if(getKeyLast() == 1){
				app_setArea_reset();
				setflas = 2;
				SMG_CleanAll();
			}
			break;

		case 2:
			
			SMG_print("F",0);
			app_viewParam();

			if(getKeyLast() == 1)
			{
				setflas = 0;
				SMG_CleanAll();
			}
		break;
		
		default:
			break;
		}
	}
	
	Beep_On(3000);


	while (1)
	{
		thisKey = keyPad_Event();
		if (thisKey != 0)
		{
			Beep_On(3);
			if (thisKey == 16)
			{
				tim2_delTask((tim_Period_Fun)SMG_RotateShift);
				break;
			}
		}
	}



	return 0;
}
