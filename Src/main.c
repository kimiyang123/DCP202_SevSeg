

#include "stm32f10x.h" // Device header

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimTask.h"

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
	char *numStr;

	uint8_t setflas = 0;
	while (1)
	{
		keyPad_Event();
		switch (setflas)
		{
		case 0:
			
			if(0 == app_setArea() && getKeyLast() == 1) setflas = 1;
			break;
		case 1:
			SMG_print("hello",0);

			if(getKeyLast() == 1){
				app_setArea_reset();
				setflas = 0;
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
