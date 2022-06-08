#include "Motor_Step.h" //步进 电机驱动

motorInfo_def sMotor_Step; // 步进电机对象

void MotorStep_Init(MotorSensor_typedef *_mSensor)
{
    GPIO_InitTypeDef sGPIOx;
    TIM_TimeBaseInitTypeDef sTimBase;
    TIM_OCInitTypeDef sTimeOC;

    uint16_t Tim_ccer=0;
    // Tim和GPIO外设时钟初始化
    MSTEP_TIMx_RCC_EN();
    MSTEP_GPIOx_RCC_EN();
    // 步进电机 方向 和 使能 管脚初始化
    sGPIOx.GPIO_Pin = MSTEP_DIR_PINx;
    sGPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
    sGPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MSTEP_DIR_GPIOx, &sGPIOx);
    sGPIOx.GPIO_Pin = MSTEP_EN_PINx;
    GPIO_Init(MSTEP_EN_GPIOx, &sGPIOx);

    sGPIOx.GPIO_Pin = MSTEP_PUL_PINx;
    sGPIOx.GPIO_Mode = GPIO_Mode_AF_PP;
    sGPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MSTEP_PUL_GPIOx, &sGPIOx);

    // 通过判断定时器是否已启动，来判断TIM是否已经初始化过
    if (READ_BIT(MSTEP_TIMx->CR1, TIM_CR1_CEN) == 0)
    { //未初始化过
        // 根据选用的管脚，选择是否启用remap
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); // tim1端口重映射
        
        TIM_TimeBaseStructInit(&sTimBase);
        sTimBase.TIM_Period = 100 - 1;
        sTimBase.TIM_Prescaler = 72 - 1;
        TIM_TimeBaseInit(MSTEP_TIMx, &sTimBase);
    }
    // else
    {
        sTimeOC.TIM_Pulse = 50;
        sTimeOC.TIM_OCMode = TIM_OCMode_PWM1;
        sTimeOC.TIM_OutputNState = TIM_OutputNState_Disable;
        sTimeOC.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
        
        // 防止其它通道CCR被 tim_oc3init()函数覆盖
        Tim_ccer = MSTEP_TIMx->CCER;
        TIM_OC3Init(MSTEP_TIMx, &sTimeOC);
        TIM1->CCER |= Tim_ccer;
    }

    TIM_ARRPreloadConfig(MSTEP_TIMx,ENABLE);
    TIM_Cmd(MSTEP_TIMx, ENABLE);
    TIM_CtrlPWMOutputs(MSTEP_TIMx, ENABLE);

    // 4- 绑定传感器
    if(_mSensor != NULL)
    {
        _mSensor->psMotor_Obj = &sMotor_Step;   // 将传感器和电机绑定
        _mSensor->InitFunc();                   // 执行传感器初始化
        // 对sensor数据复位
        sMotor_Step.pFunc_SensorReset = _mSensor->senReset;
    }

}



void MotorStep_Stop(void)
{
    sMotor_Step.Dir = MOTOR_RUN_Dir_STOP;
    sMotor_Step.speed = 0;
    MSTEP_EN(MSTEP_ENABLE);
    TIM_CCxNCmd(MSTEP_TIMx, TIM_Channel_3, TIM_CCxN_Disable);
}


void MotorStep_Run(uint8_t MotorRun_dir, uint8_t speed)
{
    sMotor_Step.Dir = MotorRun_dir;
    sMotor_Step.speed = speed;
    if(MotorRun_dir == MOTOR_RUN_Dir_Forward && speed > 0)
    { // 后退
        MSTEP_DIR(MSTEP_DIR_CW);
        MSTEP_EN(MSTEP_DISABEL);
        TIM_CCxNCmd(MSTEP_TIMx,TIM_Channel_3,TIM_CCxN_Enable);
    }
    else if(MotorRun_dir == MOTOR_RUN_Dir_Backward && speed > 0)
    {
        MSTEP_DIR(MSTEP_DIR_CCW);
        MSTEP_EN(MSTEP_DISABEL);
        TIM_CCxNCmd(MSTEP_TIMx,TIM_Channel_3,TIM_CCxN_Enable);
    }
    else // 停车
    {
        MotorStep_Stop();
    }

}



void MotorStep_SigRun(int8_t sigSpeed)
{
    uint8_t dirbit = MOTOR_RUN_Dir_STOP;

    if(sigSpeed < 0) dirbit = MOTOR_RUN_Dir_Backward;
    else if(sigSpeed >0) dirbit = MOTOR_RUN_Dir_Forward;

    if(dirbit != MOTOR_RUN_Dir_STOP)
    {
        sigSpeed = sigSpeed > 0 ? (sigSpeed) : (-sigSpeed);
        MotorStep_Run(dirbit,sigSpeed);
    }
    else
    {
        MotorStep_Stop();
    }
}



void MotorStep_Reset(void)
{
    sMotor_Step.Dir = MOTOR_RUN_Dir_STOP;
    sMotor_Step.locRAW = 0;
    sMotor_Step.locMM = 0; //电机 距离值清零
    sMotor_Step.speed = 0;
    // 如果有传感器复位方法，调用执行
    if (sMotor_Step.pFunc_SensorReset != NULL)
    {
        sMotor_Step.pFunc_SensorReset();
    }
}


