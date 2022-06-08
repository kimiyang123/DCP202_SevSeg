#include "Sensor_Encoder.h"


// motorInfo_def* psMotorX;   //X轴电机对象指针
// motorInfo_def* psMotorY;   //Y轴电机对象指针


void senEncoderX_Init(void);
void senEncoderY_Init(void);



// encoder编码器脉冲传感器对象，可支持绑定1个电机
MotorSensor_typedef SenEncoder_X = {
    .psMotor_Obj = NULL,
    .InitFunc = senEncoderX_Init,
};

// encoder编码器脉冲传感器对象，可支持绑定1个电机
MotorSensor_typedef SenEncoder_Y = {
    .psMotor_Obj = NULL,
    .InitFunc = senEncoderY_Init,
};


#define psMotorX    SenEncoder_X.psMotor_Obj 
#define psMotorY    SenEncoder_Y.psMotor_Obj 


/**
 * @brief : 只初始化TIM3 基本定时器功能及使能
 *          IC输入捕获功能定时器公共部分设置
 * @description: 
 * @return {void}
 */
void TIM3_Cap_Init(void)
{
//    GPIO_InitTypeDef sGPIO_Init;
   TIM_TimeBaseInitTypeDef sTIMBase_Init;
   NVIC_InitTypeDef  sNVIC_Init;
   
   RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,   ENABLE);

   sTIMBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
   sTIMBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
   sTIMBase_Init.TIM_Period = 0xFFFF;        //ARR-自动重载值
   sTIMBase_Init.TIM_Prescaler = 72 -1;
   TIM_TimeBaseInit(TIM3,&sTIMBase_Init);

   // 中断设置
   sNVIC_Init.NVIC_IRQChannel = TIM3_IRQn;
   sNVIC_Init.NVIC_IRQChannelPreemptionPriority = 1;
   sNVIC_Init.NVIC_IRQChannelSubPriority = 0;
   sNVIC_Init.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&sNVIC_Init);

    // 使能定时器3
   TIM_Cmd(TIM3,ENABLE);

}

// 该函数即将废弃不用，改用senEncoderX_Init() 及 senEncoderY_Init()
void senEncoder_Init(motorInfo_def* motX,motorInfo_def* motY)
{
    if(psMotorX == NULL && psMotorY == NULL)
    {
        TIM3_Cap_Init();
    }
    if(motX != NULL) psMotorX = motX;
    if(motY != NULL) psMotorY = motY;

}

// X 轴编码器 传感器初始化，对应IO TIM3_CH3
void senEncoderX_Init(void)
{
    GPIO_InitTypeDef sGPIO_Init;
    TIM_ICInitTypeDef sTIM_ICInit;

    if (SenEncoder_Y.psMotor_Obj == NULL)
    { //第一次初始化TIM3
        TIM3_Cap_Init();
    }
    // 初始化IO  GPIOB_0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    sGPIO_Init.GPIO_Mode = GPIO_Mode_IPU;
    sGPIO_Init.GPIO_Pin = GPIO_Pin_0; // TIM3-CH3   //IC 管脚编号
    GPIO_Init(GPIOB, &sGPIO_Init);

    // 设置定时器捕获通道功能
    TIM_ICStructInit(&sTIM_ICInit);
    sTIM_ICInit.TIM_Channel = TIM_Channel_3;        // 通道配置ch3
    TIM_ICInit(TIM3, &sTIM_ICInit);                 // 定时器配置

    //启用对应通道的中断捕获
    TIM_ITConfig(TIM3, TIM_IT_CC3 , ENABLE);        // CC3中断使能
}


// Y 轴编码器 传感器初始化，对应IO TIM3_CH4
void senEncoderY_Init(void)
{
    GPIO_InitTypeDef sGPIO_Init;
    TIM_ICInitTypeDef sTIM_ICInit;

    if(SenEncoder_X.psMotor_Obj == NULL)
    {   //第一次初始化TIM3
        TIM3_Cap_Init();
    }

  // 初始化IO GPIO_B1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    sGPIO_Init.GPIO_Mode = GPIO_Mode_IPU;
    sGPIO_Init.GPIO_Pin = GPIO_Pin_1; // TIM3-CH4   //IC 管脚
    GPIO_Init(GPIOB, &sGPIO_Init);


    // 设置定时器捕获通道功能
    TIM_ICStructInit(&sTIM_ICInit);
    sTIM_ICInit.TIM_Channel = TIM_Channel_4;        // 通道配置ch4
    TIM_ICInit(TIM3, &sTIM_ICInit);                 // 定时器配置

    //启用对应通道的中断捕获
    TIM_ITConfig(TIM3, TIM_IT_CC4 , ENABLE);        // CC4中断使能
}


/*******************************************************************************
 * Function Name  : TIM3_IRQHandler
 * Description    : TIM3脉冲捕获中断服务程序;TIM3_CH3, TIM3_CH4;
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void TIM3_IRQHandler(void)
{
    // TIM3_CC3 有输入捕获
    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) // GPIOB0 有上升沿输入捕获信号
    {
        if(psMotorX != NULL)
        {
            if(psMotorX->Dir == MOTOR_RUN_Dir_Backward)
            {
                if(psMotorX->locRAW > INT32_MIN)
                {
                    psMotorX->locRAW --;
                }
            }
            else if (psMotorX->Dir == MOTOR_RUN_Dir_Forward)
            {
                if (psMotorX->locRAW < INT32_MAX)
                    psMotorX->locRAW++;
            }
            psMotorX->locMM = psMotorX->locRAW * RAW_DIS_RATE;
        }
    }
    // TIM3_CC4有输入捕获
    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) // GPIOB1 有上升沿输入捕获信号
    {
        if(psMotorY != NULL)
        {
            if(psMotorY->Dir == MOTOR_RUN_Dir_Backward)
            {
                if(psMotorY->locRAW > INT32_MIN)
                {
                    psMotorY->locRAW --;
                }
            }
            else if (psMotorY->Dir == MOTOR_RUN_Dir_Forward)
            {
                if (psMotorY->locRAW < INT32_MAX)
                    psMotorY->locRAW++;
            }
            psMotorY->locMM = psMotorY->locRAW * RAW_DIS_RATE;
            
        }
    }

    // 清除中断标志位TIM_IT_Update
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3 | TIM_IT_CC4);
}
