
#ifndef __ADS7950_H__
#define __ADS7950_H__

    #include <stm32f10x_conf.h>
    #include "stddef.h"
    #include "SPI_HW.h"         // 底层需要使用SPI接口
    #include "BSP.h"

    #include "Uart_Debug.h"


    #define SPI_CS_Pin       GPIO_Pin_0 //(PA0)
    #define SPI_CS_GPIOx     GPIOA
    #define ADS_SPI          SPI1

    #define ADS_CSbit(bitVal) GPIO_WriteBit(SPI_CS_GPIOx, SPI_CS_Pin, bitVal);
    
    #define ADS7950

    #if defined(ADS7950) || defined(ADS7954) ||defined(ADS7958)
        #define ADC_CHANNEL_NUM     4
    #elif defined(ADS7951) || defined(ADS7955) || defined(ADS7959)
        #define ADC_CHANNEL_NUM     8
    #elif defined(ADS7952) || defined(ADS7956) || defined(ADS7960)
        #define ADC_CHANNEL_NUM     12
    #elif defined(ADS7953) || defined(ADS7957) || defined(ADS7961)
        #define ADC_CHANNEL_NUM     16
    #endif

    // ADS79xx 工作模式
     #define ADS79xx_MODE_BIT     12  
    enum ADS79xx_Mode{
        NOT_USE = 0,
        ADS79xx_MANUAL = 1,   //手动模式
        ADS79xx_AUTO_1 = 2,     // 只扫描bit位为1的通道
        ADS79xx_AUTO_2 = 3,     // 从0扫描到aaaa设置通道
    };

    #define ADS79xx_RANGE_BIT     6  
    enum ADS79xx_Range {
        RANGE_1 = 0,        //0 - Vref
        RANGE_2 = 1,        //0 - 2*Vref
    };

    #define ADS79xx_PROG_ENABLE     (1<<11)

    // ADS7950 mode control Register 
    typedef union ADS7950
    {
        struct 
        {
            uint16_t DI03_00_GPIOOUT    : 4;
            uint16_t DI04_GPIOorCH      : 1;
            uint16_t DI05_PORDOWN       : 1;
            uint16_t DI06_RANGE         : 1;
            uint16_t DI10_07_CH         : 4;
            uint16_t DI11_PRO_EN        : 1;
            uint16_t DI15_12_MODE       : 4;
        } REG_bits;
        
        uint16_t REG_Int16;
    } ADS79xx_MCR_typedef;
    
    extern ADS79xx_MCR_typedef ads79xx_MCR;


    /**
     * @brief : ADS79xx 芯片及SPI接口初始化
     * @description: 
     * @return {*}
     */
    void ADS79xx_Init(void);

    /**
     * @brief : 设置ADS79xx 模式
     * @description: 
     * @param {enum ADS79xx_Mode} modeId
     * @param {enum ADS79xx_Range} rangId
     * @param {uint16_t} chid
     * @return {*}
     */   
    void ADS79xx_setMode(enum ADS79xx_Mode modeId,enum ADS79xx_Range rangId,uint16_t chid);


    uint16_t ADS79xx_ReadADC_Continue(uint8_t *rd_chid);


    uint16_t ADS79xx_ReadADC(uint16_t chId);




#endif
