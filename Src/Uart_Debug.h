#ifndef __UART_DEBUG_H__
#define __UART_DEBUG_H__

    #include <stm32f10x_conf.h>

    typedef struct Uart_Debug
    {
        uint8_t uState;
        uint8_t TxBuff[25];
        uint8_t RxBuff[25];
    } UARTobj_def;


    // USB <--> CH340T <--> UART5 
    #define DBG_UART_RxPORT      GPIOC  //PC11
    #define DBG_UART_TxPORT      GPIOD  //PD2         
    #define U5RX_Pin             GPIO_Pin_11
    #define U5TX_Pin             GPIO_Pin_2        

    #define DBG_UART    UART5



    void DBG_Uart_Init(uint16_t Baudrate);

    void DBG_Uart_write(uint8_t sByte);

    void DBG_Uart_printf();




#endif

