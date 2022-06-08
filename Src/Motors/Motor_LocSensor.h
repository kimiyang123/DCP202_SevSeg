/*
 * @Date: 2022-05-22 14:29:03
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-08 14:48:59
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_LocSensor.h
 * @Description: motor Location sensor
 *        电机位置获取函数封装
 *     - 电机传感器类型: 1.编码器 
 *                      2.电容式传感器 
 *                      3.MEMS传感器 
 *                      4.步进电机数
 */
#ifndef __MOTOR_LOCATION_SENSOR_H_
#define __MOTOR_LOCATION_SENSOR_H_

    #include <stm32f10x_conf.h>
	
	#include "MotorClass.h"
    

		// 电机传感器对象结构体
		// 该结构体定义必须放在 #include "Sensor_Encoder.h" 前
    typedef struct 
    {
        motorInfo_def* psMotor_Obj;
        void (*InitFunc)(void);       // !不可赋值 传感器初始化函数
        void (*senReset)(void);      // 传感器复位函数
    }MotorSensor_typedef;
		
		
    
    #include "Sensor_Encoder.h"     // 编码器传感器
    #include "Sensor_byTicks.h"
    // #include "Sensor_ExtADC.h"



    
    #define NULL 0

    #define MOTOR_POSLOC_MAX        0xFFFF


    

#endif
