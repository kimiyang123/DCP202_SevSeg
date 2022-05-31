#include "Uart_Debug.h"

#include "string.h"
#include "stdarg.h"

void DBG_Uart_Init(uint16_t Baudrate)
{
    GPIO_InitTypeDef sGPIO_Init;
    USART_InitTypeDef sUART_Init;
    NVIC_InitTypeDef sNVIC_Init;
    // 时钟开启
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    // 设置TX RX管脚模式
    sGPIO_Init.GPIO_Pin = U5TX_Pin;
    sGPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(DBG_UART_TxPORT, &sGPIO_Init);
    sGPIO_Init.GPIO_Pin = U5RX_Pin;
    sGPIO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DBG_UART_RxPORT, &sGPIO_Init);

    // 串口中断功能，有需要可开启
    // sNVIC_Init.NVIC_IRQChannel = UART5_IRQn;
    // sNVIC_Init.NVIC_IRQChannelPreemptionPriority = 2;
    // sNVIC_Init.NVIC_IRQChannelSubPriority = 0;
    // sNVIC_Init.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&sNVIC_Init);

    // UART串口模式设置
    sUART_Init.USART_BaudRate = Baudrate;
    sUART_Init.USART_WordLength = USART_WordLength_8b;
    sUART_Init.USART_StopBits = USART_StopBits_1;
    sUART_Init.USART_Parity = USART_Parity_No;
    sUART_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    sUART_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DBG_UART, &sUART_Init);

    // USART_ITConfig(DBG_UART,USART_IT_RXNE,ENABLE);
    USART_Cmd(DBG_UART, ENABLE);
}

void DBG_Uart_write(uint8_t sByte)
{
    USART_SendData(DBG_UART, sByte);
}

void DBG_Uart_printf()
{
    
}

/**
 * @brief : 中断回调函数
 * @description:
 * @return {*}
 */
void USARTx_IRQHandler()
{
}


