#include "KeyPad4x4.h"

void keyPad_delay(uint16_t dt)
{
	uint16_t i = 0;
	while (dt--)
	{
		i = 10000; //自己定义
		while (i--)
			;
	}
}

void keyPad_Init(void)
{
	GPIO_InitTypeDef gpioInit;

	RCC->APB2ENR |= KEYPAD_IOEN;

	// ROW行线高4位初始化为输出
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = KEYPAD_ROW_BITMASK;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEYPAD_GPIO, &gpioInit);
	// COL列线低4位初始化位输入
	gpioInit.GPIO_Mode = GPIO_Mode_IPU;
	gpioInit.GPIO_Pin = KEYPAD_COL_BITMASK;
	GPIO_Init(KEYPAD_GPIO, &gpioInit);
}

// 获取第n位 的有效位所在位的 mask
uint16_t getBitMask(uint16_t mask, uint8_t id)
{
	uint8_t i = 0;

	// 遍历1的位置
	for (i = 0; i < 16; i++)
	{
		if ((mask & (1 << i)) != 0)
		{
		}
	}

	return 0;
}

/****************************************
	KeyPad 4x4

	ROW0 --+-----+-----+-----+--
O	ROW1 --+-----+-----+-----+--
U	ROW2 --+-----+-----+-----+--
T	ROW3 --+-----+-----+-----+--
		   |     |     |     |
		COL0   COL1  COL2  COL3
			 [InputPU]
	ROW : 0x0f00
	COL : 0xf000
*****************************************/

// 4x4按键扫描程序
uint8_t keyPad4x4_Scan(void)
{
	unsigned char KeyVal = 0;
	unsigned char rowID, colId;
	uint16_t tmp;
	// ROW行扫描
	for (rowID = 0; rowID < 4; rowID++)
	{
		GPIO_WriteBit(KEYPAD_GPIO, KEYPAD_ROW_BITMASK, (BitAction)1);
		GPIO_WriteBit(KEYPAD_GPIO, (0x0800 >> rowID), (BitAction)0);
		// 读取COL位，判断是否有0
		if ((GPIO_ReadInputData(KEYPAD_GPIO) & KEYPAD_COL_BITMASK) != KEYPAD_COL_BITMASK)
		{
			tmp = GPIO_ReadInputData(KEYPAD_GPIO) & KEYPAD_COL_BITMASK;
			for (colId = 0; colId < 4; colId++)
			{
				if ((tmp & (0x1000 << colId)) == 0x00)
				{
					// 查找到按键所在行列
					KeyVal = rowID * 4 + colId + 1;
					break;
				}
			}
			break;
		}
		else
		{
			KeyVal = 0;
		}
	}
	return KeyVal;
}


uint8_t _keyEvent = 0;
uint8_t getKeyLast(void)
{
	return _keyEvent;
}

uint8_t keyLast = 0; //保留前一次状态值
uint8_t keyPad_Event(void)
{
	uint8_t keyEventVal = 0;
	uint16_t temp;
	// 检测是否有按键按下
	// 将行ROW四位全部设置位 0 读取COL位
	GPIO_WriteBit(KEYPAD_GPIO, KEYPAD_ROW_BITMASK, 0);
	temp = GPIO_ReadInputData(KEYPAD_GPIO) & KEYPAD_COL_BITMASK;

	if (temp != KEYPAD_COL_BITMASK) // 有按键按下
	{
		// 消抖延时
		keyPad_delay(1);
		temp = keyPad4x4_Scan();
		if (temp != keyLast)
		{
			keyEventVal = temp;
			keyLast = temp;
		}
		else
		{ // 按键和前一次值相等
			keyEventVal = 0;
		}
	}
	else if (keyLast != 0)
	{
		keyLast = 0;
	}
	_keyEvent = keyEventVal;	//保存到全局变量
	return keyEventVal;
}

/*******************独立按键*****************************/

/**
 * @description: 独立按键管脚初始化
 * @return {*}
 */
void keyIndepend_Init(void)
{
	GPIO_InitTypeDef gpioInit_def;

	// 使能KEYINDEP管脚时钟
	RCC_APB2PeriphClockCmd(KEY_INDEP_IOEN, ENABLE);

	gpioInit_def.GPIO_Mode = GPIO_Mode_IPU;
	gpioInit_def.GPIO_Pin = KEY_INDEP_GPIO_BITMASK;
	GPIO_Init(KEY_INDEP_GPIO, &gpioInit_def);
}

/**
 * @description: 独立按键接口,最多支持8个按键
 * @return {返回按键有效状态位}
 */
uint8_t key_Independ(void)
{
	static uint8_t perKey = 0xFF;
	uint8_t KeyEvent;
	uint8_t i = 0;
	uint16_t Ptmp, keyTemp=0;
	Ptmp = KEY_INDEP_GPIO->IDR & KEY_INDEP_GPIO_BITMASK;
	if (Ptmp != KEY_INDEP_GPIO_BITMASK)
	{
		keyPad_delay(1);
		if( KEY_INDEP_GPIO_BITMASK == \
			(KEY_INDEP_GPIO->IDR & KEY_INDEP_GPIO_BITMASK))
		{
			return KEY_NULL;
		}
		for (i = 0; i < 16; i++)
		{
			if ((KEY_INDEP_GPIO_BITMASK & (1 << i)) != 0)
			{
				keyTemp <<= 1;
				if ((Ptmp & (1 << i)) == 0)
				{
					keyTemp += 1;
				}
			}
		}
	}
	else
	{
		keyTemp = 0;
	}

	KeyEvent = keyTemp ^ perKey;
	perKey = keyTemp;

	return KeyEvent & keyTemp; //按键按下有效
	// return KeyEvent & ~keyTemp;	//按键松开有效

}
