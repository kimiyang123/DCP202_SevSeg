#include "KeyPad4x4.h"


void  keyPad_delay( uint16_t dt)
{
	 uint16_t i=0;  
   while(dt--)
   {
      i=10000;  //�Լ�����
      while(i--) ;    
   }
}

void keyPad_Init(void)
{
	GPIO_InitTypeDef gpioInit;
	
	RCC->APB2ENR |= KEYPAD_IOEN;
	
	//ROW���߸�4λ��ʼ��Ϊ���
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = KEYPAD_ROW_BITMASK;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEYPAD_GPIO,&gpioInit);
	//COL���ߵ�4λ��ʼ��λ����
	gpioInit.GPIO_Mode = GPIO_Mode_IPU;
	gpioInit.GPIO_Pin = KEYPAD_COL_BITMASK;
	GPIO_Init(KEYPAD_GPIO,&gpioInit);
}


// ��ȡ��nλ ����Чλ����λ�� mask
uint16_t getBitMask(uint16_t mask, uint8_t id)
{
	uint8_t i=0;
	
	// ����1��λ��
	for(i=0; i<16; i++)
	{
		if( (mask & (1<<i)) != 0)
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

// 4x4����ɨ�����
uint8_t keyPad4x4_Scan(void)
{
	unsigned char KeyVal = 0;
	unsigned char rowID,colId;
	uint16_t tmp;
	// ROW��ɨ��
	for(rowID = 0; rowID < 4; rowID++)
	{
			GPIO_WriteBit(KEYPAD_GPIO,KEYPAD_ROW_BITMASK,(BitAction)1);
			GPIO_WriteBit(KEYPAD_GPIO,(0x0800 >> rowID),(BitAction)0);
			// ��ȡCOLλ���ж��Ƿ���0
			if( (GPIO_ReadInputData(KEYPAD_GPIO) & KEYPAD_COL_BITMASK) != KEYPAD_COL_BITMASK )
			{
				tmp = GPIO_ReadInputData(KEYPAD_GPIO)& KEYPAD_COL_BITMASK;
				for( colId = 0; colId < 4; colId++ )
				{
						if( (tmp & (0x1000 << colId)) == 0x00)
						{
								// ���ҵ�������������
								KeyVal = rowID * 4 + colId + 1;
								break;
						}
				}
				break;
			}
			else{
				KeyVal = 0;
			}
	}
	return KeyVal;
}


uint8_t  keyLast = 0; //����ǰһ��״ֵ̬

uint8_t keyPad_Event(void)
{
	uint8_t keyEventVal = 0;
	uint16_t temp;
	// ����Ƿ��а�������
	// ����ROW��λȫ������λ 0 ��ȡCOLλ
	GPIO_WriteBit(KEYPAD_GPIO,KEYPAD_ROW_BITMASK,0);
	temp = GPIO_ReadInputData(KEYPAD_GPIO) & KEYPAD_COL_BITMASK;
	
	if(temp != KEYPAD_COL_BITMASK)	// �а�������
	{
		// ������ʱ
		keyPad_delay(1);
		temp = keyPad4x4_Scan();
		if(temp != keyLast)
		{
			keyEventVal = temp;
			keyLast = temp;
		}
		else{  // ������ǰһ��ֵ���
			keyEventVal = 0;
		}
	}
	else if(keyLast != 0)
	{
			keyLast = 0;
	}
		
	return keyEventVal;

}






