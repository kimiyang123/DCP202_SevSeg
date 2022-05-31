
#include "Joystick_Driver.h"


uint16_t JoyADC_DMABuff[3];    /*>!  只可读取，不可修改*/

uint8_t Joystick_KeyScan(void);


void Joystick_Init(void)
{
    // 1- 初始化使用到的ADC通道
    GPIO_InitTypeDef sGPIOInit;
    ADC_InitTypeDef sADCInit;
    DMA_InitTypeDef sDMAInit;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    sGPIOInit.GPIO_Pin = ADC_PIN0 | ADC_PIN1 | ADC_PIN3;
    sGPIOInit.GPIO_Mode = GPIO_Mode_AIN;        // 模拟输入信号
    GPIO_Init(ADC_PORT1,&sGPIOInit);

    // 配置ADC模式
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    ADC_APBxClock_FUN(RCC_APB2Periph_ADC1,ENABLE);
    sADCInit.ADC_Mode = ADC_Mode_Independent;
    sADCInit.ADC_ScanConvMode = ENABLE;
    sADCInit.ADC_DataAlign = ADC_DataAlign_Right;
    sADCInit.ADC_ContinuousConvMode = ENABLE;
    sADCInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    sADCInit.ADC_NbrOfChannel = 3;
    ADC_Init(ADC1,&sADCInit);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 配置ADC规则通道转换顺序
    ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_13,3,ADC_SampleTime_55Cycles5);   //PC3 压力检测

    // ADC的DMA初始化
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel1);

    sDMAInit.DMA_DIR = DMA_DIR_PeripheralSRC;
    sDMAInit.DMA_BufferSize = 3;
    sDMAInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    sDMAInit.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
    sDMAInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    sDMAInit.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    sDMAInit.DMA_MemoryBaseAddr = (uint32_t)&JoyADC_DMABuff;
    sDMAInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
    sDMAInit.DMA_Mode = DMA_Mode_Circular;
    sDMAInit.DMA_Priority = DMA_Priority_High;
    sDMAInit.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1,&sDMAInit);
    DMA_Cmd(DMA1_Channel1,ENABLE);      //DMA传输启动 CCR1_EN


    ADC_DMACmd(ADC1,ENABLE);        // ADC模块开启和DMA对接使能
    ADC_Cmd(ADC1,ENABLE);

    ADC_ResetCalibration(ADC1);
    while( ADC_GetResetCalibrationStatus(ADC1) );

    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
    // 软件触发，启动ADC连续转换
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    
    // 2- 初始化摇杆向下按键的管脚
    sGPIOInit.GPIO_Pin = GPIO_Pin_2;
    sGPIOInit.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC,&sGPIOInit);

}


/**
 * @brief : 获取 摇杆(Joystick) X 或 Y轴通道的ADC值 
 * @description: 
 * @param axisID : [uint8_t] 摇杆通道ID, 0\1 or can be a enum typed 
 *                  like as  AXIS_X、AXIS_Y
 * @return {*}
 */
uint16_t Joystick_getAxisADC(uint8_t axisID)
{
    if(axisID < 2)
    {
        return JoyADC_DMABuff[axisID];      // 返回XY轴通道的ADC值
    }
    return 0;  // 通道号无效则返回0
}


/**
 * @brief : 获取机械手抓压力值
 * @description: 
 * @param type : {uint8_t} type获取数据类型，0：ADC，1：电压值mV
 * @return {*}
 */
uint16_t getHandGrasp_Push(uint8_t type)
{
    if(type == 0)
    {
        return JoyADC_DMABuff[AXIS_PUSH];
    }
    else{
        return (3300 * JoyADC_DMABuff[AXIS_PUSH]) / 4095;
    }

}



uint8_t perJoyKey = 0xff;
uint8_t joykey_EventDN = 0;     // 按键按下事件
/**
 * @brief : 采集摇杆上按键值，并保存到event按键事件变量
 * @description: 
 * @return {uint8_t} 返回GPIO当前管脚状态值
 */
uint8_t Joystick_KeyScan(void)
{
    uint8_t key;
    key = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
    if(perJoyKey != key)
    {
        if(key == 0)
        {
            joykey_EventDN = 1;
        }
        perJoyKey = key;
    }
    else{
        joykey_EventDN = 0;
    }
    return key;
}

/**
 * @brief : 用于获取按键的按下变化 瞬间
 * @description: 
 * @return {*}
 */
uint8_t Joystick_getKeyEvent(void)
{
    return joykey_EventDN;
}




// ** 摇杆数据结构体 定义及初始化
Joystick_def sJoyStick = {
    .pJoyRAW_x = &JoyADC_DMABuff[0],
    .pJoyRAW_y = &JoyADC_DMABuff[1],
    .keyscanFun = Joystick_KeyScan,
    .pKeyEventDN = &joykey_EventDN
};

