
#include "ADS1118.h"


/**
 * @brief : SPI 硬件端口初始化
 * @description:
 * @return {*}
 */
/*
void SPI_Configuration(void)
{
    GPIO_InitTypeDef sGPIO_Init;
    SPI_InitTypeDef sSPI_Init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    sGPIO_Init.GPIO_Pin = SPI_SCK | SPI_MOSI;
    sGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_GPIO, &sGPIO_Init);

    sGPIO_Init.GPIO_Pin = SPI_MISO;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(SPI_GPIO, &sGPIO_Init);

    sGPIO_Init.GPIO_Pin = SPI_ADSCS;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_GPIO, &sGPIO_Init);

    GPIO_WriteBit(SPI_GPIO, SPI_ADSCS, Bit_SET);
    // GPIO_SetBits(SPI_GPIO,SPI_ADSCS);               // 片选信号设置为1

    // ADC CONFIGURATION
    SPI_StructInit(&sSPI_Init);
    sSPI_Init.SPI_Mode = SPI_Mode_Master;
    sSPI_Init.SPI_DataSize = SPI_DataSize_16b;
    sSPI_Init.SPI_NSS = SPI_NSS_Soft;
    sSPI_Init.SPI_CPHA = SPI_CPHA_2Edge;
    sSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_Init(ADS_SPI, &sSPI_Init);

    SPI_Cmd(ADS_SPI, &sSPI_Init);
}

uint16_t SPI_Send_16Bit(SPI_TypeDef *SPIx, uint16_t wData)
{
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
        ;

    SPI_I2S_SendData(SPIx, wData);

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    return (SPI_I2S_ReceiveData(SPIx));
}

uint16_t SPI_Read_16Bit(SPI_TypeDef *SPIx)
{
    return (SPI_Send_16Bit(SPIx, 0));
}
*/
/*********************************ADS1118 驱动**********************************
 *[ConfigREG]   os | MUX2 | MUX1 | MUX0 | PGA2 | PGA1 | PGA0 | MODE
 *  default_H:   1     0     0       0      0     1       0      1
 *  0x8583      DR2| DR1  | DR0  |TS_MODE|PULL | NOP1 | NOP2 | CNV_RDY
 *  default_L:   1    0      0       0      0     0       1      1
 * ****************************************************************************/

ADS_Cnf_typedef ADS1118_REG = {
    .REG_Int16 = 0x8583};

/**
 * @brief : 设置ADS1118 配置参数结构体的默认值
 * @description:
 * @param {ADS_Cnf_typedef*} pCfg_struct
 * @return {*}
 */
void ADS1118_StructInitDef(ADS_Cnf_typedef *pCfg_struct)
{
    pCfg_struct->REG_bits.os = 1;
    pCfg_struct->REG_bits.mux = ADS_CH_A0A1;
    pCfg_struct->REG_bits.pga = ADS_PGA_2048;
    pCfg_struct->REG_bits.mode = 1;
    pCfg_struct->REG_bits.datarate = ADS_DR_128SPS;
    pCfg_struct->REG_bits.TS_Mode = 0;
    pCfg_struct->REG_bits.PullUP = 0;
    pCfg_struct->REG_bits.NOP = 01;
    pCfg_struct->REG_bits.CNV_RDY_FL = 1;

    ADS1118_REG = *pCfg_struct;
}

/**
 * @brief : 初始化ADS1118 寄存器参数
 * @description:
 * @param {ADS_Cnf_typedef*} confInit
 * @return {*}
 */
void ADS1118_Init(ADS_Cnf_typedef *confInit)
{
    // 初始化CS引脚
    GPIO_InitTypeDef    sGPIO;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);      // 开启芯片CS引脚RCC使能
    sGPIO.GPIO_Pin = SPI_CS_Pin;
    sGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    sGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIOx,&sGPIO);

    //初始化 硬件SPI模块
    HW_SPI_Init();

    ADS1118_REG = *confInit;
    ADS1118_REG.REG_bits.NOP = 01; //设置指令保存有效
    ADS_CSbit(0);
    delay_ticks(1);
    SPI_Send_16Bit(ADS_SPI, ADS1118_REG.REG_Int16);
    SPI_Send_16Bit(ADS_SPI, ADS1118_REG.REG_Int16);
    ADS_CSbit(1);
}

void ADS1118_setChannel(uint8_t chId)
{
    ADS1118_REG.REG_bits.mux = chId;
    ADS1118_REG.REG_bits.NOP = 01; //设置指令保存有效
    ADS1118_Init( &ADS1118_REG );
}

void ADS1118_setPGA(uint8_t pga)
{
    ADS1118_REG.REG_bits.pga = pga;
    ADS1118_REG.REG_bits.NOP = 01;
    ADS1118_Init( &ADS1118_REG );
}

uint16_t ADS1118_ReadADC(void)
{
    uint16_t ADCret;
    ADS1118_REG.REG_bits.os = 1;
    ADS1118_REG.REG_bits.NOP = 01;
    ADS_CSbit(0);
    delay_ticks(1);
    SPI_Send_16Bit(ADS_SPI, ADS1118_REG.REG_Int16);
    SPI_Send_16Bit(ADS_SPI, ADS1118_REG.REG_Int16);
    delay_ticks(1);
    ADCret = SPI_Send_16Bit(ADS_SPI, 0);
    ADS_CSbit(1);

    return ADCret;
}


