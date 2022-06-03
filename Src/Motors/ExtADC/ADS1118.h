#ifndef __ADS1118_H__
#define __ADS1118_H__

#include <stm32f10x_conf.h>
#include "BSP.h"

#define SPI_SCK         GPIO_Pin_5   //(PA5)
#define SPI_MISO        GPIO_Pin_6  //(PA6)
#define SPI_MOSI        GPIO_Pin_7  //(PA7)
#define SPI_ADSCS       GPIO_Pin_0 //(PA0)
#define SPI_GPIO        GPIOA

#define ADS_SPI         SPI1

#define ADS_CSbit(bitVal) GPIO_WriteBit(SPI_GPIO, SPI_ADSCS, bitVal);

/*********************************ADS1118 驱动************************************/
// ADS1118 配置数据结构体
typedef union
{
    struct
    {
        uint16_t CNV_RDY_FL : 1;
        uint16_t NOP : 2;
        uint16_t PullUP : 1;
        uint16_t TS_Mode : 1;
        uint16_t datarate : 3;
        uint16_t mode : 1;
        uint16_t pga : 3;
        uint16_t mux : 3;
        uint16_t os : 1;
    } REG_bits;

    uint16_t REG_Int16;
} ADS_Cnf_typedef;

extern ADS_Cnf_typedef ADS1118_REG;

// Operational status/single-shot conversion start
#define ADS1118_CONF_OS 0x8000
#define ADS1118_CONF_OS_bit 15
// Input multiplexer configuration          // 2,1,0   +     -     2,1,0   +     -
#define ADS1118_CONF_MUX 0x7000 // 0 0 0  AIN0  AIN1 | 1 0 0  AIN0  GND
#define ADS1118_CONF_MUX_bit 12
#define ADS1118_CONF_MUX0 0x1000 // 0 0 1  AIN0  AIN3 | 1 0 1  AIN1  GND
#define ADS1118_CONF_MUX1 0x2000 // 0 1 0  AIN1  AIN3 | 1 1 0  AIN2  GND
#define ADS1118_CONF_MUX2 0x4000 // 0 1 1  AIN2  AIN3 | 1 1 1  AIN3  GND
// Programmable gain amplifter configuration
#define ADS1118_CONF_PGA 0x0E00
#define ADS1118_CONF_PGA_bit 9
#define ADS1118_CONF_PGA0 0x0200
#define ADS1118_CONF_PGA1 0x0400
#define ADS1118_CONF_PGA2 0x0800
// Device operationg mode
#define ADS1118_CONF_MODE 0x0100 // 0:Continuous conversion 1:signgle-shot mode
#define ADS1118_CONF_MODE_bit 8
// Data rate
#define ADS1118_CONF_RATE 0X00E0
#define ADS1118_CONF_RATE_bit 5
// Temperature sensor mode
#define ADS1118_CONF_TEMP 0X0010 // 0:ADC mode 1：Temperatur sensor mode
#define ADS1118_CONF_TEMP_bit 4
// Pull-up enable
#define ADS1118_CONF_PULLUP 0X0008 // 0:DOUT Pull-up disable 1: enable
#define ADS1118_CONF_PULLUP_bit 3
// NO operation
#define ADS1118_CONF_NOOP 0x0006 // must be 01
#define ADS1118_CONF_NOOP_bit 1
// conversion ready flag
#define ADS1118_CONF_CRF 0x0001 // 0:data ready 1:data not ready
#define ADS1118_CONF_CRF_bit 0

// ADS1118 通道选择参数
enum
{
    ADS_CH_A0A1 = 0, // default
    ADS_CH_A0A3,
    ADS_CH_A1A3,
    ADS_CH_A2A3,
    ADS_CH_A0GND,
    ADS_CH_A1GND,
    ADS_CH_A2GND,
    ADS_CH_A3GND
};
// ADS1118 PGA选择参数
enum
{
    ADS_PGA_6144 = 0,
    ADS_PGA_4096,
    ADS_PGA_2048, // default
    ADS_PGA_1024,
    ADS_PGA_0512,
    ADS_PGA_0256,
};

// ADS1118 Data rate
enum
{
    ADS_DR_8SPS = 0,
    ADS_DR_16SPS,
    ADS_DR_32SPS,
    ADS_DR_64SPS,
    ADS_DR_128SPS, // default
    ADS_DR_250SPS,
    ADS_DR_475SPS,
    ADS_DR_860SPS
};

/**
 * @brief : SPI 模块配置
 * @description: 
 * @return {*}
 */
void SPI_Configuration(void);

/**
 * @brief : ADS1118 配置参数结构体默认值初始化
 * @description: 
 * @param {ADS_Cnf_typedef} *pCfg_struct
 * @return {*}
 */
void ADS1118_StructInitDef(ADS_Cnf_typedef *pCfg_struct);

/**
 * @brief : ADS1118 寄存器初始化配置
 * @description: 
 * @param {ADS_Cnf_typedef} *confInit
 * @return {*}
 */
void ADS1118_Init(ADS_Cnf_typedef *confInit);

/**
 * @brief : ADS1118 采集通道设置
 * @description: 首次切换通道后需要间隔5mS以上读取通道值
 * @param {uint8_t} chId
 * @return {*}
 */
void ADS1118_setChannel(uint8_t chId);

/**
 * @brief : ADS1118 增益配置
 * @description: 
 * @param {uint8_t} pga
 * @return {*}
 */
void ADS1118_setPGA(uint8_t pga);

/**
 * @brief : ADS1118 读取已设置通道的ADC值
 * @description: 
 * @return {*}
 */
uint16_t ADS1118_ReadADC(void);

#endif
