#include "main.h"
// #include "Joystick_Driver.h"
// #include "Servo_MotorZ.h"
// #include "LCD12864_Driver.h"
#include "TimTask.h"
// #include "KeyPad4x4.h"
#include "Uart_Debug.h"
// #include "ADS1118.h"
// #include "ADS7950.h"
#include "Motor_HPWM.h"
#include "Motor_Step.h"


#include "ADS1118.h"
#include "math.h"

int main()
{

    BSP_Configuration();
    TimTask_Init();

    DBG_Uart_Init(115200);
    //DBG_Uart_printf("system run...\n");

    //Motor_PortInit();
    Beep_On(10);

    //MotorStep_Init(NULL);       //初始化电机并绑定传感器，NULL为不绑定
    
    Motor_X_Init(&SenEncoder_X);
    Motor_Y_Init(&SenEncoder_Y);
		

	uint8_t speed = 50;
    while (1)
    {
        MotorY_SigRun(speed);

        MotorX_Run(1,20);
        delay_ticks(3000);
        MotorX_Stop();
        delay_ticks(1000);

        MotorX_Run(0,20);
        delay_ticks(3000);
        MotorX_Stop();
        delay_ticks(1000);

        speed = -speed;
				
    }
    

    while (1)
    {
        // ADC_val = sin((float)i/255)*100;
        // DBG_Uart_printf("$%d %d;",ADC_val,i);
        // i++;
        // delay_ticks(100);
    }
    
    while (1)
    {

        MotorStep_Run(0, 30);
        delay_ticks(3000);
        MotorStep_Stop();

        delay_ticks(1000);

        MotorStep_Run(1, 30);
        delay_ticks(3000);
        MotorStep_Stop();

        delay_ticks(1000);
    }

    while (1)
    {
        //        DBG_Info("motorX:%d - %d\n",sMotor_X.locRAW,sMotor_X.locMM);
        delay_ticks(400);
    }
}

