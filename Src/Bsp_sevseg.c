#include "Bsp_sevseg.h"


uint8_t Sev_Tab[] = {
			0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xf8,
			0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
			0x89,0xc7,0xc8,0xc1,0x8c,0xa3,0xbf,0xff
};

uint8_t  SMG_BUFFER[SMG_ITEMS];
#define  COM_ALLOFF			0xFF

// 设置数码管公共端:
// 根据comID 选择哪个数码显示
void SMG_SetCom(uint8_t comId)
{
	// GPIO 端口的低8位为数码管选择位，bit0有效
	if(comId == COM_ALLOFF)
	{
		SMG_GPIO->ODR |= 0xff;
	}
	else{
		SMG_GPIO->ODR |= 0xff;
		SMG_GPIO->ODR &= ~(0x80>>comId);
	}

}


// 设置显示段码，disSeg 七段译码值
void SMG_setData(uint8_t disSeg)
{
	// GPIO端口的 高8位为段码显示位 0为点亮
	SMG_GPIO->ODR &= ~(0xFF <<8);
	SMG_GPIO->ODR |= (disSeg << 8);
	
}


// 端口初始化函数
void BSP_SMG_Init(void)
{
	uint8_t i=0;
	GPIO_InitTypeDef GpioInit;
	
	// 开启GPIO的RCC时钟使能
	RCC->APB2ENR |= SMG_GPIO_RCC_ENRBIT;
	// 初始化GPIO管脚为输出
	GpioInit.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GpioInit.GPIO_Pin 	= GPIO_Pin_All;
	GpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SMG_GPIO, &GpioInit);
	// 初始化数码管显示缓存数据为 不亮
	for(i=0;i<SMG_ITEMS;i++)
	{
		SMG_BUFFER[i] = 0xff;
	}

}




// 数码管动态刷新方法
// 需要周期调用。
void SMG_Refresh(void)
{
	static uint8_t idx;
	
	// 公共端COM关闭(消除鬼影)
	SMG_SetCom(COM_ALLOFF);
	// 刷新 data数据
	SMG_setData( SMG_BUFFER[idx] );
	// 打开显示位数码管公共端
	SMG_SetCom(idx);
	
	idx ++;
	if(idx >= SMG_ITEMS) idx = 0;

}



// ---------------以下是相关显示方法-------------------------
void SMG_BuffWrite(uint8_t id,uint8_t buffd)
{
	if(id < SMG_ITEMS )		//编号id必须小于数码管数量
	{
		SMG_BUFFER[id] = buffd;
	}
}








// 数码管显示整型数据函数
/*
	showNum ：需要显示的整型数
	
*/
void SMG_ShowInt(uint32_t showNUM, uint8_t startPos, uint8_t showBit)
{
	uint8_t bit;

	for(bit = startPos; bit < showBit; bit++)
	{
		SMG_BUFFER[ bit ] = Sev_Tab[ showNUM % 10 ];
		showNUM /= 10;
	}
}







// ---------------以下是开机初始化显示方法-------------------------
// 数码管开关花样显示
#define SHIFT_Data		12345678
void SMG_RotateShift(uint8_t shiftDir)
{
	static uint32_t sdata = SHIFT_Data;
	SMG_ShowInt(sdata,0,8);
	
	if(shiftDir == 0) // 左移
	{
		sdata = (sdata * 10) % 100000000 + (sdata / 10000000); 
	}
	else{		//右移
		sdata = (sdata % 10) * 10000000 + (sdata / 10);
	}
}


// 数码管整体数据变化
#define DEFAULT_VALUE		99999999
void SMG_All_num(uint8_t UpOrDn)
{
	static uint32_t sdata = DEFAULT_VALUE;
	SMG_ShowInt(sdata,0,8);
	
	if(UpOrDn == 0)
	{
			if(sdata == 0)
			{
				sdata = 99999999;
			}
			else
			{
				sdata -= 11111111;
			}
	}
	else
	{
			if(sdata == 99999999)
			{
				sdata = 0;
			}
			else
			{
				sdata += 11111111;
			}
	}
	
	
}








