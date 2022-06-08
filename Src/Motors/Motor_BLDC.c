#include "Motor_BLDC.h"

// 无刷电机结构体对象
motorInfo_def sBLDC_Motor;

void BLDC_Motor_Init(MotorSensor_typedef *_mSensor)
{
    GPIO_InitTypeDef sGPIOx;
    TIM_OCInitTypeDef sTimeOC;

    // 1- 使能GPIO设备的RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    sGPIOx.GPIO_Pin = BLDC_DIR_PINx;
    sGPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
    sGPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLDC_DIR_GPIOx, &sGPIOx);

    // 2- 初始化PWM 管脚 及 定时器
    // 如果PWM引脚和上面DIR引脚不在同一个组上，需要重新打开对应的RCC
    sGPIOx.GPIO_Pin = BLDC_PWM_PINx;
    sGPIOx.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BLDC_PWM_GPIOx, &sGPIOx);

    // 定时器初始化，一定要主要该定时器是否和其它地方复用
    if (READ_BIT(BLDC_TIMx->CR1, TIM_CR1_CEN) == 0)
    { //未初始化过
        TIM_TimeBaseInitTypeDef sTimBase;
        // 根据选用的管脚，选择是否启用remap
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); // tim1端口重映射

        TIM_TimeBaseStructInit(&sTimBase);
        sTimBase.TIM_Period = 100 - 1;
        sTimBase.TIM_Prescaler = 72 - 1;
        TIM_TimeBaseInit(BLDC_TIMx, &sTimBase);
    }
    // 这边使用的GPIOE12 是tim1-ch3n的反向通道，其它通道类似修改
    sTimeOC.TIM_Pulse = 0;
    sTimeOC.TIM_OCMode = TIM_OCMode_PWM1;
    sTimeOC.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    sTimeOC.TIM_OutputNState = TIM_OutputNState_Disable;
    sTimeOC.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    // 防止其它通道CCR被 tim_oc3init()函数覆盖
    uint16_t Tim_ccer = 0;
    Tim_ccer = BLDC_TIMx->CCER;
    TIM_OC3Init(BLDC_TIMx, &sTimeOC);   //初始化 tim第三通道
    TIM1->CCER |= Tim_ccer;

    TIM_ARRPreloadConfig(BLDC_TIMx, ENABLE);
    TIM_Cmd(BLDC_TIMx, ENABLE);
    TIM_CtrlPWMOutputs(BLDC_TIMx, ENABLE);

    if (_mSensor != NULL)
    {
        _mSensor->psMotor_Obj = &sBLDC_Motor; // 将传感器和电机绑定
        _mSensor->InitFunc();                 // 执行传感器初始化
        sBLDC_Motor.pFunc_SensorReset = _mSensor->senReset;
    }
}

void BLDC_Motor_Stop(void)
{
    BLDC_TIMx->CCR3 = 0;    // 如果CCR=0不为停车，请修改PWM模式[TIM_OCMode] 或 极性[TIM_OCNPolarity]

    sBLDC_Motor.Dir = MOTOR_RUN_Dir_STOP;
    sBLDC_Motor.speed = 0;
}



void BLDC_Motor_Run(uint8_t MotorRun_dir, uint8_t speed)
{
    sBLDC_Motor.Dir = MotorRun_dir;     //电机方向
    sBLDC_Motor.speed =   speed > 50 ? 50:speed;    //速度限制[0-50]
    
    if(MotorRun_dir == MOTOR_RUN_Dir_Forward && speed > 0)
    { // 后退
        BLDC_DIR(BLDC_DIR_CW);

        BLDC_TIMx->CCR3 =  speed << 2;  //放大2倍
    }
    else if(MotorRun_dir == MOTOR_RUN_Dir_Backward && speed > 0)
    {
        BLDC_DIR(BLDC_DIR_CW);
        BLDC_TIMx->CCR3 =  speed << 2;  //放大2倍
    }
    else // 停车
    {
        MotorStep_Stop();
    }

    

}

void BLDC_Motor_SigRun(int8_t sigSpeed)
{
    uint8_t dirbit = MOTOR_RUN_Dir_STOP;

    if (sigSpeed < 0)
        dirbit = MOTOR_RUN_Dir_Backward;
    else if (sigSpeed > 0)
        dirbit = MOTOR_RUN_Dir_Forward;

    if (dirbit != MOTOR_RUN_Dir_STOP)
    {
        sigSpeed = sigSpeed > 0 ? (sigSpeed) : (-sigSpeed);
        BLDC_Motor_Run(dirbit, sigSpeed);
    }
    else
    {
        BLDC_Motor_Stop();
    }
}

void BLDC_Motor_Reset(void)
{
    sBLDC_Motor.Dir = MOTOR_RUN_Dir_STOP;
    sBLDC_Motor.locRAW = 0;
    sBLDC_Motor.locMM = 0; //电机 距离值清零
    sBLDC_Motor.speed = 0;
    // 如果有传感器复位方法，调用执行
    if (sBLDC_Motor.pFunc_SensorReset != NULL)
    {
        sBLDC_Motor.pFunc_SensorReset();
    }
}


