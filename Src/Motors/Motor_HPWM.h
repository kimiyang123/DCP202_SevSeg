/*
 * @Date: 2022-05-21 23:00:04
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-21 23:10:44
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_HPWM.h
 * @Description:  本文文件为 DCP-218  H桥驱动程序
 *                DRV8412  双路有刷DC电机驱动
 *          使用STM32 的TIM1 - PWM 通道作为输出
 * 
 *     DRV8412      GPIO
 *      PWM-A       PE12-(CH3N)       
 *      PWM-B       PE9 -(CH1)
 *      PWM-C       PE8 -(CH1N)
 *      PWM-D       PE11-(CH2)
 */
#ifndef __MOTOR_HPWM_H__
#define __MOTOR_HPWM_H__

#include <stm32f10x.h>
#include <stm32f10x_conf.h>


 void Motor_PortInit(void);



#endif
