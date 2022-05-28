/*
 * @Date: 2022-05-26 10:20:12
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-27 16:52:27
 * @FilePath: \DCP202_SevSeg\Src\Joystick_Driver.h
 * @Description: Joystick 摇杆手柄驱动
 *          本驱动需要依赖于 STM32的ADC 及 GPIO 模块
 */
#ifndef __JOYSTICK_DRIVER_H__
#define __JOYSTICK_DRIVER_H__


    #include <stm32f10x_conf.h>

    #define ADC_APBxClock_FUN       RCC_APB2PeriphClockCmd
    #define ADC_CLK                 RCC_APB2Periph_ADC1


    #define ADC_PIN0        GPIO_Pin_0
    #define ADC_PIN1        GPIO_Pin_1
    #define ADC_PORT1       GPIOC

    enum{
        AXIS_X = 0,
        AXIS_Y,
        AXIS_Z
    };

// ******准备使用结构体封装的方法，构建Joystick类  *****/
    typedef uint16_t (*joyFun)(void);    //按键扫描函数类型指针

    typedef struct 
    {
        uint16_t* pJoyRAW_x;
        uint16_t* pJoyRAW_y;
        uint8_t*  pKeyEventDN;
        joyFun    keyscanFun;
    }Joystick_def;
    
// *******ENDOF Joystick ******************** /

    extern uint16_t JoyADC_DMABuff[2];
    extern Joystick_def sJoyStick;

    /**
     * @brief : 摇杆设备初始化，会调用ADC 和 GPIO
     *          采样ADC DMA方法获取通道值
     * @description: 
     * @return {*}
     */    
    void Joystick_Init(void);

    uint16_t Joystick_getAxis(uint8_t axisID);
    uint8_t Joystick_KeyScan(void);
    uint8_t Joystick_getKeyEvent(void);
    


#endif
