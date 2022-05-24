

#ifndef __BSP_SEVSEG_H__
#define __BSP_SEVSEG_H__

	
	#include "stm32f10x.h"
	#include "ctype.h"
	
	#define  SMG_ITEMS					8
	
	#define  SMG_GPIO					GPIOG
	#define  SMG_GPIO_RCC_ENRBIT		RCC_APB2ENR_IOPGEN
	
	/**
	 * @brief :数码管初始化方法，开机调用 
	 * @description: 
	 * @return {void}
	 */
	void SMG_BSP_Init(void);
	
	void SMG_Refresh(void);		// 中断里间隔调用
	void SMG_CleanAll(void);
	void SMG_CleanPos(uint8_t Spos,uint8_t len);

	/**
	 * @brief : 获取数码管指定Buff内的当前显示段码数据
	 * @description: 
	 * @param  id:{uint8_t} 指定数码管位置[0-7]
	 * @return {uint8_t} 返回数码管段码
	 */	
	uint8_t SMG_BuffRead(uint8_t id);

	void SMG_BuffWrite(uint8_t id,uint8_t buffd);

	/**
	 * @description: 数码管显示整数函数
	 * @param showNUM: {uint32_t} 需要显示的数据值
	 * @param startPos: {uint8_t} 个位数显示在右侧起哪个位置
	 * @param  showBit: {uint8_t} 显示几位有效数字
	 * @return {void}
	 */
	void SMG_ShowInt(uint32_t showNUM, uint8_t startPos, uint8_t showBit);
	
	/**
	 * @brief : 以字符串形式显示数码管符号
	 * @param str :{char*}  字符串指针 如:"hello"
	 * @param startPos: {uint8_t}左侧开始的起始位置
	 * @return {void}
	 */
	void SMG_print(char *str , uint8_t startPos);



	//开机画面，数码管数据移位方法
	void SMG_RotateShift(uint8_t shiftDir);
	void SMG_All_num(uint8_t UpOrDn);

#endif



/*******************************************
 * 使用演示
 * 
 * #include "Bsp_sevseg.h"
 * int main()
 * {
 * 		SMG_BSP_Init();	//调用初始化方法
 * 
 * 		SMG_ShowInt(123,0,3);	//在数码管最右侧显示123
 * 		SMG_print("Hello",0);	//在数码管左侧第一位开始显示"hello"字符
 * 
 * 		while(1)
 * 		{
 * 			SMG_Refresh();		//周期刷新显示数码管，推荐在定时器中断中调用
 * 		}
 * 
 * }
 * 
 * **********************************************/

