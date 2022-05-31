#include "Motor_HPWM.h"
#include "Motor_LocSensor.h"

motorInfo_def sMotor_X , sMotor_Y;

// 双路电机驱动需要用到TIM1 的4路PWM信号
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
    sTimBase_Init.TIM_Prescaler = 72 - 1; //时钟分频     // PWM = 80KHz
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


/**
 * @brief :  电机GPIO及涉及到的TIM定时器等初始化
 * @description: 
 * @return {void}
 */
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

    // 电机结构体对象初始化
    MotorX_Reset();
    MotorY_Reset();
    // 电机位置传感器初始化
    // locSensor_MorotX_Bind(&sMotor_X);
    // locSensor_MorotY_Bind(&sMotor_Y);
}


/**
 * @description:  通过电机方向和速度参数，转换为CCR的PWM值
 * @param dir: 电机方向 0 or 1
 * @param speed: 电机速度 数值范围[0:50]
 * @return {uint8_t}
 */
uint8_t _getPWMofCCR(uint8_t dir,uint8_t speed)
{
    uint8_t _ccr;
    if(speed > 50) speed = 50;
    if(dir == 0)
    {
        _ccr = 50 - speed;
    }
    else
    {
        _ccr = 50 + speed;
    }
    if(_ccr == 0) _ccr = 1;
    else if(_ccr >=100) _ccr = 99;

    return _ccr;
}

/**
 * @description: X轴电机运行方法
 * @param MotorRun_dir: 电机方向 0 or 1
 * @param speed: 电机速度 < 50
 * @return {void}
 */
void MotorX_Run(uint8_t MotorRun_dir,uint8_t speed)
{
    sMotor_X.Dir = MotorRun_dir;
    sMotor_X.speed = speed;
    // X轴电机 PWMA and PWMB 使用 TIM1-CH3N  +  CH1
    TIM1->CCR3 = _getPWMofCCR(MotorRun_dir,speed);
}

/**
 * @brief : X轴电机运行方法
 * @description: 
 * @param sigSpeed: int8_t 通过参数的正负值，控制电机正反转
 * @return {void}
 */
void MotorX_SigRun(int8_t sigSpeed)
{
    uint8_t Dirbit = MOTOR_RUN_Dir_STOP;
    uint8_t speed = 0;
    if(sigSpeed < 0)    Dirbit = MOTOR_RUN_Dir_Backward;
    else if( sigSpeed > 0) Dirbit = MOTOR_RUN_Dir_Forward;
    
    if(Dirbit != MOTOR_RUN_Dir_STOP)    //电机非停车
    {
        // 取速度绝对值
        sigSpeed = sigSpeed > 0 ? (sigSpeed):(-sigSpeed);
        MotorX_Run(Dirbit,sigSpeed);
    }
    else{
        MotorX_Stop();
    }
    
}


void MotorX_Stop(void)
{
    sMotor_X.speed = 0;
    //sMotor_X.Dir = MOTOR_RUN_Dir_STOP;
    TIM1->CCR3 = _getPWMofCCR(sMotor_X.Dir,sMotor_X.speed);
}

void MotorX_Reset(void)
{
    sMotor_X.Dir = MOTOR_RUN_Dir_STOP;
    sMotor_X.locRAW = 0;
    sMotor_X.locMM = 0;     //电机 距离值清零
    sMotor_X.speed = 0;
}


/**
 * @description: Y轴电机运行方法
 * @param MotorRun_dir: 电机方向 0 or 1
 * @param speed: 电机速度 < 50
 * @return {void}
 */
void MotorY_Run(uint8_t MotorRun_dir,uint8_t speed)
{
    sMotor_Y.Dir = MotorRun_dir;
    sMotor_Y.speed = speed;

    // Y轴电机 PWMC and PWMD 使用 TIM1-CH2 + CH1N
    TIM1->CCR2 = _getPWMofCCR(MotorRun_dir,speed);
}

/**
 * @brief : Y 轴电机符号运行方法
 * @description: 
 * @param sigSpeed: int8_t 通过参数的正负值，控制电机正反转
 * @return {void}
 */
void MotorY_SigRun(int8_t sigSpeed)
{
    uint8_t Dirbit = MOTOR_RUN_Dir_STOP;
    uint8_t speed = 0;
    if(sigSpeed < 0)    Dirbit = MOTOR_RUN_Dir_Backward;
    else if( sigSpeed > 0) Dirbit = MOTOR_RUN_Dir_Forward;
    
    if(Dirbit != MOTOR_RUN_Dir_STOP)    //电机非停车
    {
        // 取速度绝对值
        sigSpeed = sigSpeed > 0 ? (sigSpeed):(-sigSpeed);
        MotorY_Run(Dirbit,sigSpeed);
    }
    else{
        MotorY_Stop();
    }
}


void MotorY_Stop(void)
{
    //方向刹车
    // TIM1->CCR2 = _getPWMofCCR(!sMotor_Y.Dir,sMotor_Y.speed);
    
    sMotor_Y.speed = 0;
    sMotor_Y.Dir = MOTOR_RUN_Dir_STOP;
    TIM1->CCR2 = _getPWMofCCR(sMotor_Y.Dir,sMotor_Y.speed);
}

void MotorY_Reset(void)
{
    sMotor_Y.Dir = MOTOR_RUN_Dir_STOP;
    sMotor_Y.locRAW = 0;
    sMotor_Y.locMM = 0;
    sMotor_Y.speed = 0;
}

/**
 * @brief : 获取电机方向
 * @description: 
 * @param {uint8_t} motorID
 * @return {*}
 */
uint8_t get_MotorDir(uint8_t motorID)
{
    switch (motorID)
    {
    case 0:
        return sMotor_X.Dir;
        break;
    case 1:
        return sMotor_Y.Dir;
        break;
    default:
        return 0xff;
        break;
    }

}

/******************电机绝对定位移动 控制函数**************************************/

/**
 * @brief : X轴电机绝对位置移动
 * @description: 
 * @param disMM :{int16_t} 目标位置mm为单位
 * @return {uint8_t} 返回电机当前状态，
 *  MOTOR_RUN_Dir_STOP  : 电机已停机，目标位置已到
 *  MOTOR_RUN_Dir_Backward : 电机仍在后退
 *  MOTOR_RUN_Dir_Forward  :
 */
uint8_t motorX_moveTo_disMM(int16_t disMM)
{   
    static uint8_t mState = 0;

    if(disMM != sMotor_X.locTargetMM)
    {
        sMotor_X.locTargetMM = disMM;
        mState = 0;
        MotorX_Run(MOTOR_RUN_Dir_STOP,0);
    }
    
    switch (mState)
    {
    case 0: //判断电机行进方向
        if(disMM == sMotor_X.locMM)
        { // 已经到达位置
          return MOTOR_RUN_Dir_STOP;
        }
        // 目标距离 > 电机当前距离
        if(sMotor_X.locTargetMM > sMotor_X.locMM)
        {
            MotorX_Run(MOTOR_RUN_Dir_Forward,DEFAULT_SPEED);
            mState = 1;     //进入目标距离检测状态
        }else if(sMotor_X.locTargetMM < sMotor_X.locMM)
        {
            MotorX_Run(MOTOR_RUN_Dir_Backward,DEFAULT_SPEED);
            mState = 1;
        }
        break;
    case 1:
        if(sMotor_X.Dir == MOTOR_RUN_Dir_Forward && sMotor_X.locMM == sMotor_X.locTargetMM ||\
            sMotor_X.Dir == MOTOR_RUN_Dir_Backward && sMotor_X.locMM < sMotor_X.locTargetMM)
        {//到达目标位置
            MotorX_Stop();
            mState = 0;
            return sMotor_X.Dir;
        }

    break;
    default:
        break;
    }
    return sMotor_X.Dir;
}

/**
 * @brief : Y轴电机绝对位置移动
 * @description: 
 * @param disMM :{int16_t} 目标位置mm为单位
 * @return {uint8_t} 返回电机当前状态，
 *  MOTOR_RUN_Dir_STOP  : 电机已停机，目标位置已到
 *  MOTOR_RUN_Dir_Backward : 电机仍在后退
 *  MOTOR_RUN_Dir_Forward  :
 */
uint8_t motorY_moveTo_disMM(int16_t disMM)
{   
    static uint8_t mState = 0;

    if(disMM != sMotor_Y.locTargetMM)
    {
        sMotor_Y.locTargetMM = disMM;
        mState = 0;
        MotorY_Run(MOTOR_RUN_Dir_STOP,0);
    }
    
    switch (mState)
    {
    case 0: //判断电机行进方向
        if(disMM == sMotor_Y.locMM)
        { // 已经到达位置
          return MOTOR_RUN_Dir_STOP;
        }
        // 目标距离 > 电机当前距离
        if(sMotor_Y.locTargetMM > sMotor_Y.locMM)
        {
            MotorY_Run(MOTOR_RUN_Dir_Forward,DEFAULT_SPEED);
            mState = 1;     //进入目标距离检测状态
        }else if(sMotor_Y.locTargetMM < sMotor_Y.locMM)
        {
            MotorY_Run(MOTOR_RUN_Dir_Backward,DEFAULT_SPEED);
            mState = 1;
        }
        break;
    case 1:
        if(sMotor_Y.Dir == MOTOR_RUN_Dir_Forward && sMotor_Y.locMM == sMotor_Y.locTargetMM ||\
            sMotor_Y.Dir == MOTOR_RUN_Dir_Backward && sMotor_Y.locMM < sMotor_Y.locTargetMM)
        {//到达目标位置
            MotorY_Stop();
            mState = 0;
            return sMotor_Y.Dir;
        }

    break;
    default:
        break;
    }
    return sMotor_Y.Dir;
}

// 2022-05-24 
// todo.. 尝试修改在 传感器检测中断，定时器中判断目标距离是否到达。
// 

