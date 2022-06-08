#ifndef __UART_DEBUG_H__
#define __UART_DEBUG_H__

    #include <stm32f10x_conf.h>

    // USB <--> CH340T <--> UART5 
    #define DBG_UART_TxPORT      GPIOC  //PC12
    #define U5TX_Pin             GPIO_Pin_12
    #define DBG_UART_RxPORT      GPIOD  //PD2  
    #define U5RX_Pin             GPIO_Pin_2         
    
    #define DBG_UART    UART5

    /**
     * @brief : debug 串口初始化，包括串口波特率
     * @description: 
     * @param  Baudrate : {uint32_t}串口波特率如:9600 / 115200
     * @return {*}
     */
    void DBG_Uart_Init(uint32_t Baudrate);

    /**
     * @brief : 串口发送一个字节数据
     * @description: 
     * @param  sByte : {uint8_t} 需要发送的字节数据
     * @return {*}
     */
    void DBG_Uart_write(uint8_t sByte);

    /**
     * @brief : 使用格式化打印方法，输出串口数据
     * @description: 
     * @param *fmt :  {char}格式化字符串，可包含转义字符，如DBG_Uart_printf("%0.3d",123);
     * @return {*}
     */
    void DBG_Uart_printf(const char *fmt,...);

#define DEBUG_ON
#ifdef DEBUG_ON
    #define DBG_Info(str,args...)       DBG_Uart_printf("[INFO] "str"\n",##args)
    #define DBG_Warning(str,args...)    DBG_Uart_printf("[WARN] "str"\n",##args)
    #define DBG_Error(str,args...)      DBG_Uart_printf("[ERR ] "str"\n",##args)
    // #define DBG_Warning(str,...)    DBG_Uart_printf("[WARN] "str ,  __VA_ARGS__)
    // #define DBG_Error(str,...)      DBG_Uart_printf("[ERR] "str ,  __VA_ARGS__)
#else
    #define DBG_Info(str,...)       {}
    #define DBG_Warning(str,...)    {}
    #define DBG_Error(str,...)      {}

#endif
    


#endif

