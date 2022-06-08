/*
 * @Date: 
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-07 22:23:50
 * @FilePath: \DCP202_SevSeg\Src\Motors\LocSensor\Sensor_ExtADC.h
 * @Description: ADC 距离读取传感器
 * 
 */
#ifndef __SENSOR_EXT_ADC_H__
#define __SENSOR_EXT_ADC_H__

    #include "Motor_HPWM.h"
    #include "Motor_LocSensor.h"

    #include "TimTask.h"


    extern MotorSensor_typedef SenExtADC;



    // 以下三个define 选择一个使用
    #define xUSE_ADS1118
    #define xUSE_ADS7950
    #define USE_RADNTEST        //随机数测试

    #define EXT_ADCDIS_RATE     0.34    // [0.34mm / ADC]

    #if defined(USE_ADS1118)
        #include "ADS1118.h"
    #elif defined(USE_ADS7950)
        #include "ADS7950.h"
    #endif

    // 定义Sensor使用的ADC通道号
    #define ADC_Channel     1
    
    #if defined(USE_ADS1118)
        #define SenADC_Get()           ADS1118_ReadADC()
    #elif defined(USE_ADS7950)
        #define SenADC_Get()           ADS79xx_ReadADC(ADC_Channel)
    #elif defined(USE_RADNTEST)
        uint16_t SenADC_Rand(void);
        #define SenADC_Get()            SenADC_Rand()
    #endif


    void senExtADC_Task(void);

#endif
