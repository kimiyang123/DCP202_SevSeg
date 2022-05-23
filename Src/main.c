

#include "stm32f10x.h" // Device header
#include "stm32f10x_conf.h"

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimTask.h"
#include "Motor_HPWM.h"

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
	BSP_Configuration();
	SMG_BSP_Init();

	keyPad_Init();
	keyIndepend_Init();

	TimTask_Init();
	Beep_On(200);

	Motor_PortInit();

	int pwmv = 0;
	uint8_t dir=0;

	SMG_ShowInt(123,2,3);
	while (1)
	{
		MotorX_Run(dir,50);
		delay_ticks(2000);
		MotorY_Run(1,30);
		dir = !dir;

		MotorX_Run(MOTOR_RUN_Dir_Forward,50);

		delay_ticks(5000);

		MotorX_Stop();
		delay_ticks(3000);
		MotorY_Stop();
		delay_ticks(6000);
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
