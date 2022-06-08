#include "ADS7950.h"

uint8_t manualChannel = 0;  // Manual模式下当前通道ID

ADS79xx_MCR_typedef ads79xx_MCR;


uint16_t ADS_Buff[ADC_CHANNEL_NUM]= {0};


void ads_delay(uint16_t dtime)
{
    uint16_t i=0;
    for (; dtime > 0 ; dtime --)
    {
        for ( i = 0; i < 1000; i++);
    }
    
}

void ADS79xx_setMode(enum ADS79xx_Mode modeId,enum ADS79xx_Range rangId,uint16_t chid);

void ADS79xx_Init(void)
{
    // 初始化CS引脚
    GPIO_InitTypeDef    sGPIO;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    sGPIO.GPIO_Pin = SPI_CS_Pin;
    sGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    sGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_CS_GPIOx,&sGPIO);
    GPIO_WriteBit(SPI_CS_GPIOx,SPI_CS_Pin,1);

    // SPI 硬件初始化
    HW_SPI_Init();

    ADS79xx_setMode(ADS79xx_MANUAL,RANGE_2,0);


}

 
void ADS79xx_setMode(enum ADS79xx_Mode modeId,enum ADS79xx_Range rangId,uint16_t chid)
{
    uint16_t ProRegSet=0;
    ads79xx_MCR.REG_bits.DI15_12_MODE = modeId;
    ads79xx_MCR.REG_bits.DI06_RANGE = rangId;
    ads79xx_MCR.REG_bits.DI11_PRO_EN = 1;
    // ads79xx_MCR.REG_bits.DI10_07_CH
    if (modeId == ADS79xx_MANUAL)
        ads79xx_MCR.REG_bits.DI10_07_CH = (chid & 0x000F);
    else
        ads79xx_MCR.REG_bits.DI10_07_CH = 0x00;

    ProRegSet = (0x01<<12) | (1<<11) | (chid << 7) | (1<<11);
    DBG_Uart_printf("ADS79xx:0x%0.4X : 0x%0.4X \n",ads79xx_MCR.REG_Int16 ,ProRegSet );
    
    ADS_CSbit(0);

        SPI_Send_16Bit(ADS_SPI,ads79xx_MCR.REG_Int16); 
    // SPI_Send_16Bit(ADS_SPI,(0x01<<12) | (1<<11) | (chid << 7) | (1<<6) ); 

    ADS_CSbit(1);

    // ads79xx_MCR.REG_bits.DI11_PRO_EN  = 0;    
    
    ADS_CSbit(0);
    SPI_Send_16Bit(ADS_SPI, ads79xx_MCR.REG_Int16);  
    ADS_CSbit(1);


    
    ADS_CSbit(0);
    SPI_Send_16Bit(ADS_SPI, ads79xx_MCR.REG_Int16);  
    ADS_CSbit(1);




    if(modeId == ADS79xx_AUTO_2)
    { // 从0-chaaaa自动扫描
        ProRegSet = (9 << 12) | ((chid & 0x000f) << 6);
        ADS_CSbit(0);
        SPI_Send_16Bit(ADS_SPI,ProRegSet);  
        ADS_CSbit(1);
    }
    else if(modeId == ADS79xx_AUTO_1)
    {
        // 从chid中为1的bit为开始扫描
        ADS_CSbit(0);
        SPI_Send_16Bit(ADS_SPI,0x8000);  
        ADS_CSbit(1);

        ADS_CSbit(0);
        SPI_Send_16Bit(ADS_SPI,chid);  
        ADS_CSbit(1);
    }


} 




uint16_t ADS79xx_ReadADC_Continue(uint8_t *rd_chid)
{
    uint16_t reval;
    ADS_CSbit(0);

    reval = SPI_Send_16Bit(ADS_SPI,0x00);

    if(rd_chid != NULL)
    {
        *rd_chid = (reval >> 12) & 0x0f;
    }
    ADS_CSbit(1);

    return (reval & 0x0FFF);
}


uint16_t ADS79xx_ReadADC(uint16_t chId)
{
    uint8_t id = 0;
    uint16_t ADC_T = 0;
    // 手动模式
    if(ads79xx_MCR.REG_bits.DI15_12_MODE == ADS79xx_MANUAL)
    {
        if(chId != ads79xx_MCR.REG_bits.DI10_07_CH)
        { //重新配置 manual模式下的通道
            ADS79xx_setMode(ADS79xx_MANUAL,ads79xx_MCR.REG_bits.DI06_RANGE,chId);
        }
        do
        {
            ADC_T = ADS79xx_ReadADC_Continue(&id);
            if(id < 4)
            {
                ADS_Buff[id] = ADC_T; 
            }
        } while (id != chId);

        return ADS_Buff[chId];
    }
    else  // auto-1 or auto-2
    {
        do
        {
            ADC_T = ADS79xx_ReadADC_Continue(&id);
            if(id < 4)
            {
                ADS_Buff[id] = ADC_T; 
            }
        } while (id != chId);
        return ADS_Buff[chId];
    }   
}
