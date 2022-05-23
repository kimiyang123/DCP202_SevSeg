/*
 * @Date: 2022-05-22 14:29:03
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-22 23:48:59
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
		
    #include "Motor_HPWM.h"
    
    #include "Sensor_Encoder.h"     // 编码器传感器
    
    #define NULL 0

    #define MOTOR_POSLOC_MAX        0xFFFF

    
    /**
     * @brief : 绑定X轴传感器对应的电机
     * @description: 
     * @param  pMotx: {motorInfo_def*}X 轴电机对象
     * @return {*}
     */
//		 
    void locSensor_MorotX_Bind(motorInfo_def* pMotx);
		
    void locSensor_MorotY_Bind(motorInfo_def* pMoty);
    

#endif
