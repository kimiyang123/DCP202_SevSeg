/*
 * @Date: 2022-05-26 10:20:12
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-06 09:43:35
 * @FilePath: \DCP202_SevSeg\Src\Joystick_Driver.h
 * @Description: Joystick 摇杆手柄驱动
 *          本驱动需要依赖于 STM32的ADC 及 GPIO 模块
 *      5/30 添加压力检测通道
 */
#ifndef __JOYSTICK_DRIVER_H__
#define __JOYSTICK_DRIVER_H__


    #include <stm32f10x_conf.h>

    #define ADC_APBxClock_FUN       RCC_APB2PeriphClockCmd
    #define ADC_CLK                 RCC_APB2Periph_ADC1


    #define ADC_PIN0        GPIO_Pin_0   //摇杆 X轴
    #define ADC_PIN1        GPIO_Pin_1   //摇杆 Y轴
    #define ADC_PIN3        GPIO_Pin_3   //压力传感器
    #define ADC_PORT1       GPIOC

    // 摇杆通道 枚举类型
    enum{
        AXIS_X = 0,
        AXIS_Y,
        AXIS_PUSH   // 机械手抓压力检测
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

    extern uint16_t JoyADC_DMABuff[3];
    extern Joystick_def sJoyStick;

    /**
     * @brief : 摇杆设备初始化，会调用ADC 和 GPIO
     *          采样ADC DMA方法获取通道值
     * @description: 
     * @return {*}
     */    
    void Joystick_Init(void);

    /**
     * @brief : 获取 摇杆(Joystick) X 或 Y轴通道的ADC值 
     * @description: 
     * @param axisID : [uint8_t] 摇杆通道ID, 0\1 or can be a enum typed 
     *                  like as  AXIS_X、AXIS_Y
     * @return {*}
     */
    uint16_t Joystick_getAxisADC(uint8_t axisID);

    /**
     * @brief : 获取机械手抓压力值
     * @description: 
     * @param type : {uint8_t} type获取数据类型，0：ADC，1：电压值mV
     * @return {*}
     */
    uint16_t getHandGrasp_Push(uint8_t type);

    uint8_t Joystick_KeyScan(void);
    uint8_t Joystick_getKeyEvent(void);


    
    


#endif
