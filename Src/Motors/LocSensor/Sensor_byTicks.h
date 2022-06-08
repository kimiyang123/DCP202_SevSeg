/*
 * @Date: 
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-08 12:41:25
 * @FilePath: \DCP202_SevSeg\Src\Motors\LocSensor\Sensor_byTicks.h
 * @Description: 通过ticks计时来作为电机距离传感器
 *              注意！最后法宝，不到万不得已不得使用。
 * 
 */
#ifndef __SENSOR_BY_TICKS_H__
#define __SENSOR_BY_TICKS_H__

    #include "MotorClass.h"
    #include "Motor_LocSensor.h"
    #include "BSP.h"
    #include "TimTask.h"    //定时器任务

    #define NULL 0

    #define TICKS_DIS_RATE      0.1  // ticks 与距离关系 0.1mm / ticks


    extern MotorSensor_typedef SenTicks;


    

#endif

