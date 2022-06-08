/*
 * @Date: 
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-08 21:49:16
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_BLDC.h
 * @Description: 带驱动功能的无刷电机驱动程序
 *              需要有2个IO口，1个PWM调速，1个GPIO方向
 * 
 */
#ifndef __MOTOR_BLDC_H__
#define __MOTOR_BLDC_H__

    #include <stm32f10x_conf.h>
    #include "MotorClass.h"
    #include "Motor_LocSensor.h"

    // BLDC 无刷电机控制IO定义,需根据实际情况修改
    // 无刷电机方向控制引脚，普通IO
    #define BLDC_DIR_GPIOx      GPIOE
    #define BLDC_DIR_PINx       GPIO_Pin_10
    // 无刷电机PWM调速引脚，TIM-PWM
    #define BLDC_PWM_GPIOx      GPIOE
    #define BLDC_PWM_PINx       GPIO_Pin_12     //TIM1_CH3n

    // 无刷电机PWM定时器
    #define BLDC_TIMx           TIM1


    enum {
        BLDC_DIR_CW = 0,       //步进电机正反转切换 0 / 1
        BLDC_DIR_CCW = !BLDC_DIR_CW
    };
    #define BLDC_DIR(_DIR)         GPIO_WriteBit(BLDC_DIR_GPIOx,BLDC_DIR_PINx,(BitAction)_DIR)





    /**********函数声明**********/

    void BLDC_Motor_Init(MotorSensor_typedef *_mSensor);

    void BLDC_Motor_Run(uint8_t MotorRun_dir,uint8_t speed);

    void BLDC_Motor_SigRun(int8_t sigSpeed);

    void BLDC_Motor_Stop(void);

    void BLDC_Motor_Reset(void);



#endif


