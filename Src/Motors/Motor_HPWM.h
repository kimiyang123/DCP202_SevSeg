/*
 * @Date: 2022-05-21 23:00:04
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-22 23:42:50
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_HPWM.h
 * @Description: 本代码通过STM32的TIM1定时器的PWM输出，
 *               驱动2路直流有刷电机
 *  *
 *      DRV8412      GPIO
 *      PWM-A       PE12-(CH3N)
 *      PWM-B       PE9 -(CH1)
 *      PWM-C       PE8 -(CH1N)
 *      PWM-D       PE11-(CH2)
 */

#ifndef __MOTOR_HPWM_H__
#define __MOTOR_HPWM_H__

#include <stm32f10x.h>
#include <stm32f10x_conf.h>


enum
{
    MOTOR_RUN_Dir_Forward = 0,
    MOTOR_RUN_Dir_Backward,
    MOTOR_RUN_Dir_STOP
};


// 电机对象 数据信息结构体
typedef struct
{
    uint8_t Dir;      //电机方向
    uint8_t speed;    //电机当前速度
    uint16_t location; // 电机位置 无单位数值16bit
} motorInfo_def;

void Motor_PortInit(void);

void MotorX_Run(uint8_t MotorRun_dir, uint8_t speed);
void MotorX_Stop(void);
void MotorX_Reset(void);

void MotorY_Run(uint8_t MotorRun_dir, uint8_t speed);
void MotorY_Stop(void);
void MotorY_Reset(void);

#endif
