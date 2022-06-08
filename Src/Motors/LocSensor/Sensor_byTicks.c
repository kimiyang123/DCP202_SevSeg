#include "Sensor_byTicks.h"

void SenTicks_Init(void);
void SenTicks_Task(void);

// 定时器模拟传感器对象，依靠定时器任务计时
MotorSensor_typedef SenTicks = {
    .psMotor_Obj = NULL,
    .InitFunc = SenTicks_Init,
    .senReset = NULL,
};

// 如有更多电机需要时间，可再定义一个sensor对象
// MotorSensor_typedef SenTicks_2 = {
//     .psMotor_Obj = NULL,
//     .InitFunc = SenTicks_Init_2,
//     .senReset = NULL,
// };


#define psMotor     SenTicks.psMotor_Obj


/**
 * @brief : Ticks传感器初始化
 * @description: 
 * @return {*}
 */
void SenTicks_Init(void)
{
    // 依靠定时器1ms中断任务，对电机对象raw数据进行加减操作
    // 将定时计算函数，加入到TIM2 周期任务列表中。
    tim2_addTask(SenTicks_Task,1,NULL);

}




/**
 * @brief : SensorTicks传感器定时调用任务，至少1ms调用一次
 * @description: 
 * @return {*}
 */
void SenTicks_Task(void)
{
    if(psMotor != NULL)
    {
        if(psMotor->Dir == MOTOR_RUN_Dir_Backward)
        {
            if(psMotor->locRAW > INT32_MIN)
            {
                psMotor->locRAW --;
            }
        }
        else if(psMotor->Dir == MOTOR_RUN_Dir_Forward)
        {
            if(psMotor->locRAW < INT32_MAX)
            {
                psMotor->locRAW ++;
            }
        }
        // 计算距离mm
        psMotor->locMM = psMotor->locRAW * TICKS_DIS_RATE;
    }

    // 如有2个电机需要使用本传感器，需要再定义一段代码
}


