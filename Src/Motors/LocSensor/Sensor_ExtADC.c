#include "Sensor_ExtADC.h"

#include "utils.h"

// will be obsolete
motorInfo_def* psMotorX_ADC;  // 传感器对应的电机对象指针


void senExtADC_Init(void);
 void senExtADC_Reset(void);

MotorSensor_typedef SenExtADC = {
   .psMotor_Obj = NULL,
   .InitFunc = senExtADC_Init,
   .senReset = senExtADC_Reset,
};


uint16_t ADC_Zero = 0;  // 机械零点ADC值

void senExtADC_Reset(void);


void senExtADC_Init(void)
{
   // 添加ADC传感器定时测量任务
   tim2_addTask(senExtADC_Task,10,NULL);
}

void senExtADC_Init_Obsolete(motorInfo_def* motX)
{

    if(motX != NULL) psMotorX_ADC = motX;

    // 给motor添加复位函数指针
    motX->pFunc_SensorReset = senExtADC_Reset;
    
    // 添加ADC传感器定时测量任务
    tim2_addTask(senExtADC_Task,10,NULL);

}

 void senExtADC_Reset(void)
 {
    ADC_Zero = SenADC_Get();
 }



 /**
  * @brief : 该函数加入到定时任务
  * @description: 
  * @return {*}
  */
 void senExtADC_Task(void)
 {
    if (psMotorX_ADC != NULL)
    {
       psMotorX_ADC->locRAW = SenADC_Get() - ADC_Zero;
       psMotorX_ADC->locMM = psMotorX_ADC->locRAW * EXT_ADCDIS_RATE;
    }
 }


///  rand ADC test
uint16_t SenADC_Rand(void)
{
   return random(0,1024);
}

