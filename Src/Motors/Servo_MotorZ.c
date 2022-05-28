#include "Servo_MotorZ.h"

/**
 * @brief : 舵机和Z轴电机初始化, 通过初始化TIM4 输出2路 PWM信号
 * @description:
 * @return {*}
 */
void ServoMotor_Init(void)
{
    GPIO_InitTypeDef sGPIO_InitStruct;
    TIM_TimeBaseInitTypeDef sTIM_BaseInitStruct;
    TIM_OCInitTypeDef sTIM_OCInit;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    sGPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    sGPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    sGPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &sGPIO_InitStruct);

    sTIM_BaseInitStruct.TIM_Period = 20000;
    sTIM_BaseInitStruct.TIM_Prescaler = 72 - 1;
    sTIM_BaseInitStruct.TIM_ClockDivision = 0;
    sTIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &sTIM_BaseInitStruct);

    TIM_OCStructInit(&sTIM_OCInit);
    sTIM_OCInit.TIM_OCMode = TIM_OCMode_PWM2; // PWM2模式，舵机脉冲信号有三极管电路反向
    sTIM_OCInit.TIM_OutputState = TIM_OutputState_Enable;
    sTIM_OCInit.TIM_Pulse = 0x0000; // CCR 值
    sTIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;
    sTIM_OCInit.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OC3Init(TIM4, &sTIM_OCInit);
    TIM_OC4Init(TIM4, &sTIM_OCInit);

    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    TIM_Cmd(TIM4, ENABLE);

    // 设置3，4 两个通道的占空比
    TIM_SetCompare3(TIM4, 10000); // Z轴电机PWM
    TIM_SetCompare4(TIM4, 1500);  // Servo电机PWM

    // 使能 TIMx  CH3 CH4通道脉冲输出
    TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);
    TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Enable);
}

// 舵机角度控制信号脉冲宽度在 0.5ms -- 2.5ms范围变化
uint8_t _SerVol = 0; //保存舵机当前设定位置

void ServoWrite(uint8_t SerVol)
{
    uint16_t serPWM;
    if (SerVol > 100)
        SerVol = 100;

    _SerVol = SerVol;
    serPWM = (2500 - 500) / 100 * SerVol + 500; //[0,500] => [500,2500]
    TIM4->CCR4 = serPWM;
}

motorZ_Info_def sMotorZ;

void MotorZ_Run(uint8_t dir, uint8_t zspeed)
{
    if (dir > 2)
        dir = 2; //限定2以上的数字
    if (zspeed > 100)
        zspeed = 0;

    if (dir == 2)
    {
        TIM4->CCR3 = 10000;
    }

    if (dir == 0) // [0,100] => [10000,20000]
    {
        TIM4->CCR3 = zspeed * 100 + 10000;
    }
    else
    { // [0,100] => [10000,0]
        TIM4->CCR3 = (zspeed * -100) + 10000;
    }

    sMotorZ.dir = dir;
    sMotorZ.speed = zspeed;
}

void MotorZ_RunSig(int8_t zspeed)
{
    if (zspeed > 100)
        zspeed = 100;
    else if (zspeed < -100)
        zspeed = -100;
    TIM4->CCR3 = 100 * zspeed + 10000;

    if (zspeed >= 0)
    {
        sMotorZ.dir = 0;
        sMotorZ.speed = zspeed;
    }
    else
    {
        sMotorZ.dir = 1;
        sMotorZ.speed = -zspeed;
    }
}

uint8_t MotorZ_MoveByTime(int8_t zspeed, uint16_t duration)
{
    static uint32_t entTime = 0;
    static uint8_t state = 0;

    if ((entTime + duration) > getSysTicks())
    { // 目标时间 大于 当前时间，说明前一此仍在执行
        return sMotorZ.dir;
    }

    switch (state)
    {
    case 0:

        entTime = getSysTicks();
        MotorZ_RunSig(zspeed);
        state = 1;

        break;

    case 1: // 判断超时
       // 执行到case 1 说明函数头上的if条件不成立了
        state = 0;
        return enMOTORz_TIMOUT;
    break;
    }

    return sMotorZ.dir;

    // entTime = getSysTicks();
    // MotorZ_RunSig(zspeed);

    // while (getSysTicks() - entTime < duration)
    //     ;
    // return sMotorZ.dir;
}

void MotorZ_Stop(void)
{
    TIM4->CCR3 = 10000; // Z轴电机PWM占空比 50%
    // Z轴电机信息刷新
    sMotorZ.dir = 2;
    sMotorZ.speed = 0;
}
