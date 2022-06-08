/*
 * @Date: 
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-08 21:22:52
 * @FilePath: \DCP202_SevSeg\Src\Motors\MotorClass.h
 * @Description: 此文件保存 motor对象共有的数据类型
 * 
 */
#ifndef __MOTOR_CLASS_H__
#define __MOTOR_CLASS_H__

    #include <stdint.h>
    #include <stm32f10x_conf.h>
	
    // 电机对象 数据信息结构体
    typedef struct
    {
        // 电机方向
        uint8_t Dir;               
        // 电机当前速度 
        uint8_t speed;              
        
        // 电机位置 无单位数值16bit(有符号数)
        __IO int32_t locRAW;        
        // 电机距离坐标值，单位mm(有符号，可表示负数)
        __IO int16_t locMM;
        // 电机滑台目标位置         
        int16_t locTargetMM;        
        //  绑定传感器的复位函数
        void (*pFunc_SensorReset)(void);    
    } motorInfo_def;


    // 电机方向控制类型
    enum
    {
        MOTOR_RUN_Dir_Forward = 0,
        MOTOR_RUN_Dir_Backward,
        MOTOR_RUN_Dir_STOP
    };




#endif
		
		
