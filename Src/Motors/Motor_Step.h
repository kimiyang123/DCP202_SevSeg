/*
 * @Date:
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-07 22:34:17
 * @FilePath: \DCP202_SevSeg\Src\Motors\Motor_Step.h
 * @Description: 步进电机驱动代码
 *                  
 */
#ifndef     __MOTOR_STEP_H__
#define     __MOTOR_STEP_H__



    #include <stm32f10x.h>
    #include <stm32f10x_conf.h>

    #include "MotorClass.h"
    #include "Motor_LocSensor.h"

    // **** 定义 步进顶级控制器的 EN、DIR、PULSE 三个信号管脚
    // 步进电机方向引脚 - DIR
    #define MSTEP_DIR_GPIOx     GPIOE
    #define MSTEP_DIR_PINx      GPIO_Pin_10
    // 步进电机使能控制引脚 - EN （可选使用）
    #define MSTEP_EN_GPIOx      GPIOE
    #define MSTEP_EN_PINx       GPIO_Pin_14      //

    // 步进电机速度控制脉冲引脚 - PULSE
    #define MSTEP_PUL_GPIOx     GPIOE
    #define MSTEP_PUL_PINx      GPIO_Pin_12     //TIM1_CH3n
    // 脉冲发生定时器（采用定时器方法输出PULSE脉冲）
    #define MSTEP_TIMx          TIM1        
    
    
    #define MSTEP_TIMx_RCC_EN()     RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE)
    #define MSTEP_GPIOx_RCC_EN()    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);

    enum {
        MSTEP_DIR_CW = 0,       //步进电机正反转切换 0 / 1
        MSTEP_DIR_CCW = !MSTEP_DIR_CW
    };
    #define MSTEP_DIR(_DIR)         GPIO_WriteBit(MSTEP_DIR_GPIOx,MSTEP_DIR_PINx,(BitAction)_DIR)

    enum {
        MSTEP_ENABLE = 0,
        MSTEP_DISABEL = !MSTEP_ENABLE,
    };
    #define MSTEP_EN(_ENSTATE)      GPIO_WriteBit(MSTEP_EN_GPIOx,MSTEP_EN_PINx,(BitAction)_ENSTATE) 




    /**
     * @brief :  步进电机初始化
     * @description: 步进电机需要用到一个定时器发脉冲
     *               1个方向IO 和 1个enable使能IO
     * @return {*}
     */
    void MotorStep_Init(MotorSensor_typedef *_mSensor);

    /**
     * @brief : 步进电机运行
     * @description: 
     * @param  MotorRun_dir : {uint8_t}运行方向
     * @param  speed : {uint8_t} 运行速度，目前无效，只要大于0即可
     * @return {*}
     */
    void MotorStep_Run(uint8_t MotorRun_dir,uint8_t speed);

    /**
     * @brief : 根据正负符号控制电机方向
     * @description: 
     * @param  sigSpeed : int8_t，有符号数值，正负表示正反转，大小无所谓，只为兼容motorX、motorY电机的运行方法
     * @return {*}
     */    
    void MotorStep_SigRun(int8_t sigSpeed);

    /**
     * @brief : 步进电机停车方法
     * @description: 
     * @return {*}
     */    
    void MotorStep_Stop(void);

    /**
     * @brief : 步进电机复位方法，回将绑定传感器的距离值复位清零
     * @description: 
     * @return {*}
     */    
    void MotorStep_Reset(void);
    


#endif

