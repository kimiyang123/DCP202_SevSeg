/*
 * @Date: 
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-30 09:41:27
 * @FilePath: \DCP202_SevSeg\Src\LCD12864_Driver.h
 * @Description: LCD12864 字符屏驱动文件
 * 
 */
#ifndef __LCD12864_DRIVER_H__
#define __LCD12864_DRIVER_H__

    #include <stm32f10x_conf.h>
    #include <stdint.h>
    #include <string.h>

    #include <math.h>


    #include "BSP.h"

    #define LCD_RS_Port     GPIOG
    #define LCD_RS_Pin      GPIO_Pin_8
    #define LCD_RW_Port     GPIOG
    #define LCD_RW_Pin      GPIO_Pin_9
    #define LCD_EN_Port     GPIOG
    #define LCD_EN_Pin      GPIO_Pin_10
    #define LCD_RST_Port    GPIOG
    #define LCD_RST_Pin     GPIO_Pin_11

    #define LCD_DATA_Port   GPIOG
    #define LCD_DATA_PinS   (0x00FF)        // 低8位

    #define LCD_EXTEN_CMD       0x34        //LCD 扩展指令
    #define LCD_BASE_CMD        0x30        //LCD 基本指令
    #define LCD_EXT_SHOWON      0x02        // 扩展指令中绘图显示ON bit
		
		
		void LCD_WriteCommand(uint8_t cmd);
		
    // 打开光标闪烁
    #define LCD_CursorON()      do{\
                                    LCD_WriteCommand(LCD_BASE_CMD); \
                                    LCD_WriteCommand(0x0C | 0x01);  \
                                }while(0);
                                
    // 关闭光标闪烁
    #define LCD_CursorOFF()     do{\
                                    LCD_WriteCommand(LCD_BASE_CMD); \
                                    LCD_WriteCommand(0x0C | 0x00);  \
                                }while(0);


    ///  数组定义放在 *.c文件中，在 *.h文件中会出现重复定义问题。
    extern uint8_t FZimgs[];       // + 字符号
    extern uint8_t FXimgs[];       // * 字符号

    

    void LCD_CMD_Init(void);
    void LCD_WriteData(uint8_t u8data);

    void LCD_CursorSet(uint8_t Line,uint8_t xPos);
    void LCD_StringPrint(char *pStr , uint8_t line,uint8_t xPos);
    void LCD_ShowInt(uint32_t sNum,uint8_t line,uint8_t xPos, uint8_t showBit);

    void LCD_Printf(uint8_t xPos, uint8_t yPos, const char *fmt,...);
    void LCD_Clear_Screen(void);
    void LCD_clearPos(uint8_t xPos,uint8_t yLine, uint8_t amount);

//*******(Graph mode Operate) ***************/
    void LCD_Graph_Clear(void);
    void LCD_draw_dot(uint8_t x,uint8_t y, uint8_t bitOnOrOff);

    // void LCD_draw_Img(  uint8_t xStartPos,uint8_t yStartPos, 
    //                     uint8_t img[],uint8_t img_With,uint8_t img_height);
    
    void LCD_draw_Img(  uint8_t xStartPos,uint8_t yStartPos, uint8_t img[]);

#endif


