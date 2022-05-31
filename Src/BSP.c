
#include "BSP.h"

// 时钟节拍
__IO uint32_t _sysTicks_inc = 0;	

/****************************************************************************
* 名    称：void sysTick_increase(void)
* 功    能：时钟节拍增量
* 入口参数：无
* 出口参数：无
* 说    明：此方法由 systick定时器中断周期调用
* 调用方法：无 
****************************************************************************/  
void sysTick_increase(void)
{
	_sysTicks_inc ++;
}


/*获取系统时钟节拍数量*/
uint32_t getSysTicks(void)
{
	return _sysTicks_inc;	
}

/****************************************************************************
* 名    称：void delay_ticks
* 功    能：基于systick定时器的延时函数
* 入口参数：dtime 延时时间，单位mS
* 出口参数：无
* 说    明：此方法由 systick定时器中断周期调用
* 调用方法：无 
****************************************************************************/  

void delay_ticks(uint16_t dtime)
{
	uint32_t timestamp;
	timestamp = getSysTicks();
	while(getSysTicks() - timestamp < dtime);
}


extern void SMG_Refresh(void);


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
  sysTick_increase();

}




/************************************************/
/*								GPIO     
*/

/**
 * @brief : 开发板 板载资源配置，含GPIO systick初始化
 * @description: 
 * @return {*}
 */
void BSP_Configuration(void)
{
	GPIO_InitTypeDef GPIOInit;
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	// *******BEEP 蜂鸣器管脚初始化**********/
	GPIOInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Pin = GPIO_Pin_13;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_Port,&GPIOInit);

	
	// *******LED 管脚初始化**********/
	GPIOInit.GPIO_Mode	 	= GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Pin 		= GPIO_Pin_7|GPIO_Pin_9;
	GPIOInit.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIOInit);
	GPIO_WriteBit(GPIOC,GPIOInit.GPIO_Pin,(BitAction)1);
	
	// *******Button 按键管脚初始化**********/
	GPIOInit.GPIO_Mode		= GPIO_Mode_IPU;
	GPIOInit.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_Init(BTN_Port,&GPIOInit);
	
	// 中断优先级分组配置
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//需要用该方法修改中断优先级分组
//	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);

// ***********Systick 初始化******************************/
	SysTick_Config(SystemCoreClock / 1000);
	// SysTick_Config(72000);
	NVIC_SetPriority(SysTick_IRQn, 0);			// systick中断优先级设置为0，0 最高。
// *************systick 初始化结束**************************/

	
}


/**
 * @brief : 蜂鸣器发声
 * @description: 
 * @param onTime: (uint16_t)发声时间，单位ms
 * @return {null}
 */
void Beep_On(uint16_t onTime)
{
	uint32_t BeepOnstamp;
	BeepOnstamp = getSysTicks();
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,1);
	while(getSysTicks() - BeepOnstamp < onTime );
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
}


uint8_t bspKey_EventDn , perBspKey = 0xff;
uint8_t Bsp_BTNScan(void)
{
	uint8_t key = 0;
	uint8_t i=0;

	// 将2个按键 组合到一个字节数据的最低2位
	if(GPIO_ReadInputDataBit(BTN_Port,BTN1_Pin) == 1)
	{
		key |= 0x01;
	}
	key<<=1;
	if(GPIO_ReadInputDataBit(BTN_Port,BTN2_Pin) == 1)
	{
		key |= 0x01;
	}
	key = (~key) & 0x03;  // 取反以1表示按键有效

	if(key != 0x00)
	{	//有按键按下
		bspKey_EventDn = (key ^ perBspKey);
		
		if(key != perBspKey)
		{

		}

	}

	

}



/**
 * @brief : 数据范围映射方法
 * @description: 
 * @param {long} x
 * @param {long} in_min
 * @param {long} in_max
 * @param {long} out_min
 * @param {long} out_max
 * @return {*}
 */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



