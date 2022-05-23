#include "Motor_LocSensor.h"


// void initExit()
// {
//    EXTI_InitTypeDef sEXTI_init;
//    sEXTI_init.EXTI_Mode = EXTI_Mode_Interrupt;
//    sEXTI_init.EXTI_Line = EXTI_Line0 | EXTI_Line1;
//    sEXTI_init.EXTI_Trigger = EXTI_Trigger_Rising;
//    sEXTI_init.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&sEXTI_init);
// }


void locSensor_Init(void)
{
   // 使用编码器位置传感器
   //senEncoder_Init();
}


/**
 * @brief : 传感器绑定 X轴电机对象
 * @description: 
 * @param _pMotorx:{motorInfo_def*} 电机对象指针
 * @return {*}
 */
void locSensor_MorotX_Bind(motorInfo_def* pMotx)
{
   senEncoder_Init(pMotx,NULL);
}

void locSensor_MorotY_Bind(motorInfo_def* pMoty)
{
   senEncoder_Init(NULL,pMoty);
}


