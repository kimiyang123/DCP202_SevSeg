

#include "stm32f10x.h" // Device header
#include "stm32f10x_conf.h"

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimTask.h"
#include "Motor_HPWM.h"

unsigned char State = 0, perState = 0xff;

int main(void)
{
	BSP_Configuration();
	SMG_BSP_Init();

	keyPad_Init();
	keyIndepend_Init();

	TimTask_Init();
	Beep_On(20);

	Motor_PortInit();

	uint8_t dir=0,xdir = 0;
	SMG_ShowInt(123,2,3);

	int16_t dis = 10;
	uint8_t xStopFlag,yStopFlag;
	while (1)
	{	//双电机运作测试

		xStopFlag = motorX_moveTo_disMM(dis*5);
		yStopFlag = motorY_moveTo_disMM(dis);

		if(xStopFlag == MOTOR_RUN_Dir_STOP &&
			yStopFlag == MOTOR_RUN_Dir_STOP)
		{
				delay_ticks(2000);
				dis= 0 - dis;
		}
	}

	//MotorX_Run(0,50);
	uint8_t state = 0;
	#define moRunSpeed 35
	MotorY_Run(0,moRunSpeed);
	MotorX_Run(1,50);
	while (1)
	{
		switch (state)
		{
		case 0:
			if (sMotor_Y.locRAW == 3000)
			{
				MotorY_Stop();
				delay_ticks(2000);
				MotorY_Run(1,moRunSpeed);
				state = 1;
			}else if(sMotor_Y.locRAW >= (3000-500))
			{
				MotorY_Run(0, \
					constrain(((1500 - sMotor_Y.locRAW) / 12),15,20) );
			}
			
			

			break;
		case 1:
			if (sMotor_Y.locRAW == 0)
			{
				MotorY_Stop();
				delay_ticks(2000);
				MotorY_Run(0,moRunSpeed);
				state = 0;
			}
			else if(sMotor_Y.locRAW < 500)
			{
				MotorY_Run(1, \
					constrain((sMotor_Y.locRAW /25 ),15, 20) );
			}
			break;
		
		default:
			break;
		}
	}
	

	while (1)
	{
		

		MotorY_Run(dir,35);
		delay_ticks(2000);
		MotorY_Stop();
		delay_ticks(3000);
		
		dir = !dir;
		MotorY_Run(dir,35);
		delay_ticks(2000);
		MotorY_Stop(); 
		delay_ticks(3000);
		dir = !dir;

		xdir = !xdir;

	}
	

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
