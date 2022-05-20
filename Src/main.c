

#include "stm32f10x.h" // Device header

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimAndPwm.h"

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

	TIM2_Init();

	uint8_t flag = 0;
	uint16_t thisKey = 0;

	tim2_addTask(SMG_Refresh, 3, NULL);
	// tim2_addTask((tim_Period_Fun)SMG_RotateShift,1000,0);
	Beep_On(100);
	// SMG_print("h-l>o F",1);
	// uiTest
	char *numStr;
	while (1)
	{
		int i = 0;
		numStr = ui_WaitEnter(0, 100);
		if (numStr != NULL)
		{
			i = atoi(numStr);
			SMG_ShowInt(i, 0, 8);
			delay_ticks(3000);
		}
		else{
			 Beep_On(1300);
		}
	}

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

	while (1)
	{

		thisKey = keyPad_Event();
		switch (State)
		{
		case 0:
			if (perState != State)
			{
				SMG_Clear();
				SMG_ShowInt(501, 5, 3);
				perState = State;
			}

			break;

		case 1:

			break;
		}
	}

	return 0;
}
