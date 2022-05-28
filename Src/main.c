#include "main.h"
#include "Joystick_Driver.h"
#include "Servo_MotorZ.h"


int main()
{
    int speed = 100;
    BSP_Configuration();

    LED1_ON();

    ServoMotor_Init();

    ServoWrite(0);
    MotorZ_Stop();

    Beep_On(20);
    LED1_OFF();

    delay_ticks(2000);

    Beep_On(10);
    LED1_Toggle();
    while (1)
    {
       if(MotorZ_MoveByTime(speed,2000) == enMOTORz_TIMOUT)
       {
           speed = -speed;
            LED1_Toggle();
            Beep_On(10);

       }
    }
    
    

    while (1)
    {        
        delay_ticks(2000);
        ServoWrite(50);
            MotorZ_RunSig(-50);
        delay_ticks(3000);

            MotorZ_Stop();
        ServoWrite(30);
            MotorZ_RunSig(30);
        delay_ticks(4000);

        ServoWrite(70);
        delay_ticks(1000);
            MotorZ_Stop();

    }
    

}

