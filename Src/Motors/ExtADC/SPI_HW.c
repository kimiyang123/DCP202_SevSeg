#include "SPI_HW.h"

void HW_SPI_Init(void)
{
    GPIO_InitTypeDef sGPIO_Init;
    SPI_InitTypeDef sSPI_Init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    sGPIO_Init.GPIO_Pin = SPI_SCK | SPI_MOSI;
    sGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_GPIOx, &sGPIO_Init);

    sGPIO_Init.GPIO_Pin = SPI_MISO;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_GPIOx, &sGPIO_Init);
    GPIO_WriteBit(SPI_GPIOx,SPI_MISO,1);

    // sGPIO_Init.GPIO_Pin = SPI_ADSCS;
    // sGPIO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
    // GPIO_Init(SPI_GPIOx, &sGPIO_Init);
    // GPIO_WriteBit(SPI_GPIOx, SPI_ADSCS, Bit_SET);
    // GPIO_SetBits(SPI_GPIO,SPI_ADSCS);               // 片选信号设置为1

    // ADC CONFIGURATION
    SPI_StructInit(&sSPI_Init);
    sSPI_Init.SPI_Mode = SPI_Mode_Master;
    sSPI_Init.SPI_DataSize = SPI_DataSize_16b;
    sSPI_Init.SPI_NSS = SPI_NSS_Soft; // 软件片选模式
    sSPI_Init.SPI_CPOL = SPI_CPOL_Low;
    sSPI_Init.SPI_CPHA = SPI_CPHA_1Edge;            // ADC7950 需要设置为1Edge
    sSPI_Init.SPI_FirstBit = SPI_FirstBit_MSB;
    sSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_Init(ADS_SPI, &sSPI_Init);

    SPI_Cmd(ADS_SPI, ENABLE);
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
    return (SPI_Send_16Bit(SPIx, DummyValue));
}
