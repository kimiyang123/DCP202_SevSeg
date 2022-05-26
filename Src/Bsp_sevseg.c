#include "Bsp_sevseg.h"


typedef struct
{
	char Letter;			//字符字母
	uint8_t SevSEG_Code;	//字母的数码管编码
} LetterSev_def;

LetterSev_def LetterTab[] = {
	{'A',	0x88},
	{'S',	0x92 & 0x7F},
	{'H',	0x89},
	{'L',	0xC7},
	{'N',	0xc8},
	{'E',	0x86},
	{'R',	~0x50},
	{'U',	0xc1},
	{'P',	0x8c},
	{'O',	0xa3},
	{'-',	0xbf},
	{' ',	0xff},
	{'>',	~0x01},  // _
	{'F',	0x8E}
};

uint8_t Sev_Tab[] = {
			0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xf8,
			0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
			0x89,0xc7,0xc8,0xc1,0x8c,0xa3,0xbf,0xff
		//	 H	  L    N    U    P    o    -     
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


/**
 * @brief :数码管初始化方法，开机调用 
 * @description: 
 * @return {*}
 */
void SMG_BSP_Init(void)
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


/**
 * @brief : 数码管动态刷新方法，推荐在定时在中断中周期调用
 * @description: 每调用执行一次，数码管显示一个位。
 * @return {*}
 */
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

/**
 * @brief : 清除数码管所有显示内容
 * @description: 
 * @return {*}
 */
void SMG_CleanAll(void){
	uint8_t i=0;
	for(i=0;i<SMG_ITEMS;i++)
	{
		SMG_BUFFER[i] = 0xff;
	}
}

/**
 * @brief : 清除数码管某个显示管
 * @description: 
 * @param Spos:	StartPosition起始位置，右侧起
 * @param len: 清除数码管显示个数
 * @return {*}
 */
void SMG_CleanPos(uint8_t Spos,uint8_t len)
{
	uint8_t i=0;
	if(Spos > SMG_ITEMS -1) Spos = SMG_ITEMS -1;
	if((len+Spos) > SMG_ITEMS) len = SMG_ITEMS - Spos;

	for ( i = Spos; i < len + Spos; i++)
	{
		SMG_BUFFER[ SMG_ITEMS - i - 1] = 0xFF;
	}
	
}

uint8_t SMG_BuffRead(uint8_t id){
	if(id < SMG_ITEMS)
	{
		return SMG_BUFFER[id];
	}
	else{
		return 0xff;
	}
}


// ---------------以下是显示相关方法-------------------------
void SMG_BuffWrite(uint8_t id,uint8_t buffd)
{
	if(id < SMG_ITEMS )		//编号id必须小于数码管数量
	{
		SMG_BUFFER[id] = buffd;
	}
}




// 数码管显示整型数据函数
	/**
	 * @description: 数码管显示整数函数
	 * @param showNUM: {uint32_t} 需要显示的数据值
	 * @param startPos: {uint8_t} 个位数显示在右侧起哪个位置
	 * @param  showBit: {uint8_t} 显示几位有效数字
	 * @return {*}
	 */
void SMG_ShowInt(uint32_t showNUM, uint8_t startPos, uint8_t showBit)
{
	uint8_t bit;
	uint8_t endPos;
	endPos = startPos + showBit;
	if(endPos > 8 ) endPos = 8;				//限定结束位置

	for(bit = startPos; bit < endPos; bit++)
	{
		SMG_BUFFER[ bit ] = Sev_Tab[ showNUM % 10 ];
		showNUM /= 10;
	}
}




uint8_t LetterTab_find(char str)
{
	uint8_t i=0;
	uint16_t tabLength;
	tabLength = sizeof(LetterTab)/sizeof(LetterSev_def);
	for ( i = 0; i < tabLength; i++)
	{
		if(LetterTab[i].Letter == str)
		{
			return LetterTab[i].SevSEG_Code;
		}
	}
	// 未定义字符，显示'_'
	return ~0x08;
	
}

/**
 * @brief : 获取ASCII码字符的数码管8段译码数据
 * @description: 
 * @param ch: {char} 需要查询的字符 如：'A'
 * @return {*}
 */
uint8_t _getSEG_byChar(char ch)
{
	if(ch >= '0' && ch <= '9')
	{
		return Sev_Tab[ ch - '0' ];
	}
	else
	{
		ch = toupper(ch);	//将字符统一转换为大写
		return LetterTab_find(ch);
	}
	
}


void SMG_print(char *str , uint8_t startPos)
{
	uint8_t i=0;
	uint8_t strShowSize_MAX = 0;
	strShowSize_MAX = SMG_ITEMS - startPos;

	for ( i = 0; i < strShowSize_MAX; i++)
	{
		if(*str == '\0') break;
		SMG_BuffWrite( SMG_ITEMS - ( startPos+i ) - 1, _getSEG_byChar(*str));
		str++;
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








