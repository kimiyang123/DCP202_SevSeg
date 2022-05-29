/*
 * @Date: 2022-05-27 16:55:50
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-28 10:38:48
 * @FilePath: \DCP202_SevSeg\Src\Motors\ServO_MotorZ.h
 * @Description: 舵机及Z轴电机控制
 *             使用TIM4 定时器的2个PWM脉冲，分别控制Z轴电机和Servo舵机
 */

#ifndef __SERVO_MOTORZ_H__
#define __SERVO_MOTORZ_H__

    #include <stm32f10x_conf.h>

    enum {
        enMOTORz_FORW = 0,
        enMOTORz_BACK = 1,
        enMOTORz_TIMOUT     //运行时间到
    };

    typedef struct 
    {
        uint8_t dir;        //电机方向 2表示位停机
        uint8_t speed;      //电机速度 
    }motorZ_Info_def ;


    extern uint32_t getSysTicks(void);
    

    // // JG: PB8,PB9, Z轴电机,舵机,;

    /**
     * @brief : 舵机及Z轴电机控制初始化方法
     *          <p>初始化TIM4 </p> 以及对应CH3 CH4 通道的GPIOB8,GPIOB9
     * @description: 
     * @return {*}
     */    
    void ServoMotorZ_Init(void);

    /**
     * @brief : 舵机开合度设置
     * @description: 
     * @param SerVol : {uint8_t} 设置范围 0-100
     * @return {*}
     */    
    void ServoWrite(uint8_t SerVol);

    /**
     * @brief : Z 轴电机带方向参数控制方法
     * @description: 
     * @param dir : {uint8_t} 电机方向 0 or 1
     * @param zspeed : {uint8_t} 电机速度值参数 输入范围[0-100]
     * @return {*}
     */
    void MotorZ_Run(uint8_t dir,uint8_t zspeed);

    /**
     * @brief : Z 轴电机速度和方向控制
     * @description: 
     * @param  zspeed : {int8_t}有符号数,输入范围[-100 , 100]
     * @return {*}
     */    
    void MotorZ_RunSig(int8_t zspeed);

    /**
     * @brief : Z 轴电机停车方法，会设置PWM占空比为50%
     * @description: 
     * @return {*}
     */    
    void MotorZ_Stop(void);

    /**
     * @brief : Z 轴电机持续运行指定时间
     * @description: 
     * @param zspeed :{int8_t} Z轴电机运行速度，有符号表示方向,输入范围[-100 , 100]
     * @param duration: {uint16_t} 电机运行的持续时间，ms单位
     * @return {返回当前电机方向}
     */    
    uint8_t MotorZ_MoveByTime(int8_t zspeed, uint16_t duration);

#endif




 /***********MotorZ Servo使用Demo********************************
  * 
  * #include "main.h"
  * #include "Servo_MotorZ.h"
  * 
    int main()
    {
        int speed = 100;
        BSP_Configuration();

        ServoMotorZ_Init();      // 舵机和 Z轴 定时器GPIO初始化

        ServoWrite(0);           // 舵机走到0角度位置
        MotorZ_Stop();           // Z轴电机停车


        delay_ticks(2000);

        while (1)
        {
            // 一种简单的 Z轴电机到位测试 移动2S时间
            if(MotorZ_MoveByTime(speed,2000) == enMOTORz_TIMOUT)
            {
                speed = -speed;
                ServoWrite(speed); 
            }
        }

    }
  * 
  * 
  * 
  *************************************************************/ 
