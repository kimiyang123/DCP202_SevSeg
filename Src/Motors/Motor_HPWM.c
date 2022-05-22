#include "Motor_HPWM.h"

void TIM1_PWM_Init(void)
{
    GPIO_InitTypeDef sGPIO_Init;
    TIM_TimeBaseInitTypeDef sTimBase_Init;
    TIM_OCInitTypeDef sTimOCInit;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_TIM1, ENABLE);
  
    sGPIO_Init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12 |
                          GPIO_Pin_8 | GPIO_Pin_11;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    sGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &sGPIO_Init);

    /** !注意：PinRemap时，必须打开AFIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); // tim1端口重映射

    sTimBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
    sTimBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
    sTimBase_Init.TIM_Period = 100 - 1;  // ARR
    sTimBase_Init.TIM_Prescaler = 72 - 1; //时钟分频
    sTimBase_Init.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &sTimBase_Init);

    sTimOCInit.TIM_Pulse = 50; //设置占空比，CCR/TIM_Period
    sTimOCInit.TIM_OCMode = TIM_OCMode_PWM1;
    sTimOCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
    sTimOCInit.TIM_OutputState = TIM_OutputState_Disable; //TIM_OutputState_Enable;
    sTimOCInit.TIM_OCIdleState = TIM_OCIdleState_Reset;
    // 高级定时器设置参数
    sTimOCInit.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    sTimOCInit.TIM_OutputNState = TIM_OutputNState_Disable; //TIM_OutputNState_Enable;
    sTimOCInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &sTimOCInit);
    TIM_OC2Init(TIM1, &sTimOCInit);
    TIM_OC3Init(TIM1, &sTimOCInit);

    // 在没有
    // TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;            //运行模式下输出
    // TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;            //空闲模式下输出选择
    // TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;                //锁定设置
    // TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                              //死区时间设置
    // TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                    //刹车功能使能
    // TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;       //刹车输入极性
    // TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //自动输出使能
    // TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    // 单独修改某个OCx输出的极性
    // TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_High);

    TIM_ARRPreloadConfig(TIM1,ENABLE);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Motor_PortInit(void)
{
    TIM1_PWM_Init();

    // 开启PWM 通道
    TIM_CCxCmd( TIM1,TIM_Channel_1,TIM_CCx_Enable);     // - PE9
    TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);    // - PE12

    TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);    // - PE8
    TIM_CCxCmd( TIM1,TIM_Channel_2,TIM_CCx_Enable);     // - PE11


    // 单独开启或关闭某个定时器通道的指令
    // TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);
}
