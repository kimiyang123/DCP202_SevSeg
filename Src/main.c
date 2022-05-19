#include "stm32f10x.h"                  // Device header

#include "main.h"
#include "Bsp_sevseg.h"
#include "bsp.h"
#include "KeyPad4x4.h"

#include "TimAndPwm.h"

int num;



// 
void fundemo(void)
{
	static uint16_t i=0;
	SMG_ShowInt(i,0,4);
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



unsigned char State = 0,perState = 0xff;
int main(void)
{
	SysTick_Config(72000);
	
	BSP_Configuration();
	BSP_SMG_Init();
	keyPad_Init();
	
	TIM2_Init();
	


	uint8_t flag = 0;
	uint8_t thisKey = 0;
		
	tim2_addFun(fundemo,100,0);
	delay_ticks(3000);
	tim2_addFun(LED1_flash,250,0);
	
	delay_ticks(4001);
	tim2_delFun(fundemo);
	tim2_addFun(LED2_flash,70,0);
	
	while(1)
	{
		
		thisKey = keyPad_Event();
		if(thisKey != 0 )
		{
			SMG_ShowInt(thisKey,0,4);
			if(thisKey == 16) break;
		}
	}
	
	while(1)
	{
	
		thisKey = keyPad_Event();
		
		
			switch(State)
			{
				case 0:
					if(perState != State)
					{
						perState = State;
						tim2_addFun(SMG_RotateShift,400,0);
					}
					if(thisKey == 15) tim2_delFun(SMG_RotateShift);
					if(thisKey == 16) tim2_addFun(SMG_RotateShift,600,0);
					
				break;
				
				
				case 1:
					
				break;
			}
		
	}
	
	return 0;
}

