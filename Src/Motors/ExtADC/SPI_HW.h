/*
 * @Date: 2022-06-04 12:56:36
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-06-04 13:29:59
 * @FilePath: \DCP202_SevSeg\Src\Motors\ExtADC\SPI_HW.h
 * @Description: STM32 SPI硬件初始化
 * 
 */
#ifndef __SPI_HW_H__
#define __SPI_HW_H__

#include <stm32f10x_conf.h>

#define SPI_SCK         GPIO_Pin_5   //(PA5)
#define SPI_MISO        GPIO_Pin_6  //(PA6)
#define SPI_MOSI        GPIO_Pin_7  //(PA7)
#define SPI_ADSCS       GPIO_Pin_0 //(PA0)
#define SPI_GPIOx       GPIOA

#define ADS_SPI         SPI1


#define DummyValue      0x00

/**
 * @brief : 硬件SPI初始化
 * @description: 
 * @return {*}
 */
void HW_SPI_Init(void);

/**
 * @brief : SPI发送16bit数据
 * @description: 
 * @param {SPI_TypeDef} *SPIx
 * @param {uint16_t} wData
 * @return {*}
 */
uint16_t SPI_Send_16Bit(SPI_TypeDef *SPIx, uint16_t wData);

/**
 * @brief : SPI单独接收16bit数据
 * @description: 
 * @param {SPI_TypeDef} *SPIx
 * @return {*}
 */
uint16_t SPI_Read_16Bit(SPI_TypeDef *SPIx);


#endif

