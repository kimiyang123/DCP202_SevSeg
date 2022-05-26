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



void delay_ticks(uint16_t dtime);
uint32_t getSysTicks(void);

void BSP_Configuration(void);

void Beep_On(uint16_t onTime);

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

