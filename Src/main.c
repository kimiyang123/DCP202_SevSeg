#include "main.h"
#include "Joystick_Driver.h"
#include "Servo_MotorZ.h"
#include "LCD12864_Driver.h"
#include "TimTask.h"
#include "KeyPad4x4.h"
#include "Uart_Debug.h"
// #include "ADS1118.h"
#include "ADS7950.h"


uint16_t i = 29;
uint16_t d = 234;
void shotI(void)
{
    LCD_Printf(3,1,"%0.4d",i++);
}

void showd(void)
{
    LCD_ShowInt(d,2,4,3);
    d++;
}


int main(void)
{

    uint8_t chID ;
    uint16_t adcV[4];
    BSP_Configuration();
    DBG_Uart_Init(115200);
    DBG_Uart_printf("system run...\n");

    ADS79xx_Init();
    Beep_On(10);

    uint16_t i=0,j=0;
    // ADS79xx_setMode(ADS79xx_MANUAL,RANGE_1,0);
    ADS79xx_setMode(ADS79xx_MANUAL,RANGE_1,3);
    // while (1)
    // {
    //     adcV[0] = ADS79xx_ReadADC_Continue(&chID);
    //     DBG_Uart_printf("ADC-%0.2d:%X\n",chID,adcV[0]);
    //     delay_ticks(0);
    //     if(j>=4) j = 0;
    // }

    while (1)
    {
        adcV[0] = ADS79xx_ReadADC(3);
        DBG_Uart_printf("ADC:%X\n",adcV[0]);
        delay_ticks(100);
    }
    
    

    while (1)
    {

        adcV[0] = ADS79xx_ReadADC_Continue(&chID);
        DBG_Uart_printf("ADC-%0.2d:%X \tj=%d\n",chID,adcV[0],j);
        delay_ticks(100);

        if(i ++ >15)
        {
            i=0;
            j++;
            if(j>=4) j = 0;

            ADS79xx_setMode(ADS79xx_AUTO_2,RANGE_1,j);
        }
    }
    

}



/*    ADS1118 Test
int main(void)
{
    int motorx;
    int i = 0;

    BSP_Configuration();
    LCD_CMD_Init();
	Beep_On(15);

    DBG_Uart_Init(9600);
    DBG_Uart_printf("system run...\n");
    LCD_Printf(0,2,"HELLO");


    // ADS1118_StructInitDef(&sADSInit);
    // ADS1118_Init(&sADSInit);

    ADS1118_Init(&ADS1118_REG);

    TimTask_Init();

    tim2_addTask(showd,100,NULL);
    tim2_addTask(shotI,10,NULL);

    
    while (1)
    {
        uint16_t adc1,adc2;
        float val=0;
        ADS1118_setChannel(ADS_CH_A1GND);
        adc1 = ADS1118_ReadADC();
        delay_ticks(5);
        ADS1118_setChannel(ADS_CH_A0GND);
        adc2 = ADS1118_ReadADC();
        DBG_Uart_printf("ADS1118 : 0X%0.4X \t 0x%0.4X\t",adc1,adc2);
        delay_ticks(10);
        val = (int16_t)adc1 * 2.048/32767.0;
        DBG_Uart_printf("%.3f\t",val);
        val = (int16_t)adc2 * 2.048/32767.0;
        DBG_Uart_printf("%.3f\n",val);

        // Beep_On(10);
    }
}
*/


