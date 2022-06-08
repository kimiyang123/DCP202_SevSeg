/*
 * @Date: 2022-05-21 23:00:04
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-06 22:12:33
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_HPWM.h
 * @Description: 本代码通过STM32的TIM1定时器的PWM输出，
 *               驱动2路直流有刷电机
 *  *
 *      DRV8412      GPIO
 *      PWM-A       PE12-(CH3N)  \
 *      PWM-B       PE9 -(CH1)   / X轴电机
 *      PWM-C       PE8 -(CH1N)  \
 *      PWM-D       PE11-(CH2)   / Y轴电机
 *  **************************************************
 *      控制方案： CH1通道PWM占空比固定为50%， 
 *                 分别改变CH3N及CH2通道的占空比偏离50%距离，以分别实现电机的正反转
 */

#ifndef __MOTOR_HPWM_H__
#define __MOTOR_HPWM_H__

#include <stm32f10x.h>
#include <stm32f10x_conf.h>

#include "MotorClass.h"
#include "Motor_LocSensor.h"

// XY轴直流电机自动运行默认速度  MAX=50
#define DEFAULT_SPEED   45




extern motorInfo_def sMotor_X , sMotor_Y;

void Motor_PortInit(void);
void Motor_X_Init(MotorSensor_typedef *_mSensor);
void Motor_Y_Init(MotorSensor_typedef *_mSensor);

void MotorX_Run(uint8_t MotorRun_dir, uint8_t speed);
void MotorX_SigRun(int8_t sigSpeed);            //通过带符号数控制方向
void MotorX_Stop(void);
void MotorX_Reset(void);

void MotorY_Run(uint8_t MotorRun_dir, uint8_t speed);
void MotorY_SigRun(int8_t sigSpeed);            //通过带符号数控制方向
void MotorY_Stop(void);
void MotorY_Reset(void);



/**
 * @brief : X轴电机移动到具体的 传感器 RAW值位置,精度比以毫米为单位移动更高
 * @description: 
 * @param  senRaw :{int32_t}传感器脉冲数值
 * @return {*}
 */
// uint8_t motorX_moveTo_senRAW(int32_t senRaw);
// uint8_t motorY_moveTo_senRAW(int32_t senRaw);



uint8_t motorX_moveTo_disMM(int16_t disMM);
uint8_t motorY_moveTo_disMM(int16_t disMM);

#endif
