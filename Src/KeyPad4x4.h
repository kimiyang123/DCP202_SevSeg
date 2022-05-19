
#ifndef __KEYPAD_H__
#define __KEYPAD_H__

	#include "stm32f10x.h"
    
    #define KEYPAD_GPIO         GPIOD
    #define KEYPAD_IOEN         RCC_APB2ENR_IOPDEN
		
		#define KEYPAD_GPIO_BITMASK 0xFF00	// GPIO屏蔽位 0xFF00 or 0x00FF
		#define KEYPAD_ROW_BITMASK	0x0F00	// 按键行线 所在bit位
		#define KEYPAD_COL_BITMASK	0xF000	// 按键列线 作为bit位
		
		
		
		// 键盘初始化
		void keyPad_Init(void);		

    // 4x4按键扫描程序
    uint8_t keyPad4x4_Scan(void);
		uint8_t keyPad_Event(void);


#endif
