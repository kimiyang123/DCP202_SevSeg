#ifndef __SYS_BSP_H__
#define __SYS_BSP_H__

#include <stdio.h>
#include "stm32f10x.h"   
#include "stm32f10x_conf.h" 

#define LED_Port		GPIOC
#define LED1_Pin		GPIO_Pin_7
#define LED2_Pin		GPIO_Pin_9

#define LED1_ON()			GPIO_WriteBit(LED_Port,LED1_Pin,0)
#define LED1_OFF()		GPIO_WriteBit(LED_Port,LED1_Pin,1)
#define LED1_Toggle()	LED_Port->ODR ^= LED1_Pin
#define LED2_ON()			GPIO_WriteBit(LED_Port,LED2_Pin,0)
#define LED2_OFF()		GPIO_WriteBit(LED_Port,LED2_Pin,1)
#define LED2_Toggle()	LED_Port->ODR ^= LED2_Pin


#define BTN_Port        GPIOC
#define BTN1_Pin        GPIO_Pin_6
#define BTN2_Pin        GPIO_Pin_8

#define BEEP_Port       GPIOC
#define BEEP_Pin        


/**
 * @brief : 系统延时方法，需要预先初始化 systick 定时器
 * @description: 
 * @param  dtime: {uint16_t}延时时间，以mS为单位
 * @return {*}
 */
void delay_ticks(uint16_t dtime);

/**
 * @brief : 获取系统滴答定时器 Systick 的系统节拍数
 * @description: 
 * @return {uint32_t} 系统节拍数，单位mS
 */
uint32_t getSysTicks(void);

/**
 * @brief : STM32核心板，及其板载资源初始化，开机必须调用
 * @description: 
 * @return {*}
 */
void BSP_Configuration(void);

/**
 * @brief : 蜂鸣器驱动函数
 * @description: 
 * @param onTime : {uint16_t}发声持续时间，单位 ms
 * @return {*}
 */
void Beep_On(uint16_t onTime);

// todo list .....
// 添加STM32核心板 LED 及 按键驱动



/**
 * @brief : 数值比例映像方法
 * @description: 
 * @param {long} x:
 * @param {long} in_min
 * @param {long} in_max
 * @param {long} out_min
 * @param {long} out_max
 * @return {*}
 */
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif

