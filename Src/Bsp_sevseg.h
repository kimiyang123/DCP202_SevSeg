

#ifndef __BSP_SEVSEG_H__
#define __BSP_SEVSEG_H__

	
	#include "stm32f10x.h"
	#include "ctype.h"
	
	#define  SMG_ITEMS					8
	
	#define  SMG_GPIO					GPIOG
	#define  SMG_GPIO_RCC_ENRBIT		RCC_APB2ENR_IOPGEN
	
	// 数码管初始化函数
	void BSP_SMG_Init(void);
	
	void SMG_Refresh(void);		// 中断里间隔调用
	void SMG_CleanAll(void);
	void SMG_CleanPos(uint8_t Spos,uint8_t len);

	void SMG_BuffWrite(uint8_t id,uint8_t buffd);
	void SMG_ShowInt(uint32_t showNUM, uint8_t startPos, uint8_t showBit);
	void SMG_print(char *str , uint8_t startPos);

	//开机画面，数码管数据移位方法
	void SMG_RotateShift(uint8_t shiftDir);
	void SMG_All_num(uint8_t UpOrDn);

#endif
