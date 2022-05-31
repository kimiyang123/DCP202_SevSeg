/*
 * @Date: 2022-05-20 09:21:18
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-30 16:52:08
 * @FilePath: \DCP202_SevSeg\Src\appSMG_UI.h
 * @Description: 用户操作人机接口界面程序
 * 
 */
#ifndef __APP_UI_SMG_H__
#define __APP_UI_SMG_H__

#include "KeyPad4x4.h"
#include "BSP.h"
#include "string.h"
#include "stdlib.h"

#include "ctype.h"

#include "LCD12864_Driver.h"
// #include "Bsp_sevseg.h"

#define  ENTER_TYPE_NUM     0x00
#define  ENTER_TYPE_ALL     0x01

__WEAK void UI_setCursor(uint8_t curId);

#if !defined(__LCD12864_DRIVER_H__) && !defined(__BSP_SEVSEG_H__)
    #error Please include SMG.h or LCD12864_xxx.h in the file of appSMG_UI.h
#endif

#if defined(__LCD12864_DRIVER_H__) && defined(__BSP_SEVSEG_H__)
    #error Only Can one be INCLUDE between the files of Bsp_sevseg.h and LCD12864.h
#endif

// todo... 需要根据使用的显示器类型实现用户UI 操作的显示接口函数
#ifdef __LCD12864_DRIVER_H__ 
    //1- 清除光标输入位置显示内容
    #define UI_CLEARPOS(xPos,yLine,maxEnter)   LCD_clearPos(xPos,yLine,maxEnter)
    //2- 输入位置内容打印函数
    #define UI_PRINT(xPos,yLine,str)  LCD_Printf(xPos,yLine,str)
    //3- 显示一个待输入的光标符号
    #define UI_SETCURSOR(xPos,yLine)  LCD_CursorON()
#endif

//数码管驱动
#ifdef __BSP_SEVSEG_H__ 
    //1- 清除光标输入位置显示内容
    #define UI_CLEARPOS(xPos,yLine,maxEnter)   SMG_CleanPos(xPos,maxEnter)
    //2- 输入位置内容打印函数
    #define UI_PRINT(xPos,yLine,str)  SMG_print(str,xPos)
    //3- 显示一个待输入的光标符号
    #define UI_SETCURSOR(xPos,yLine)  UI_setCursor(xPos)
#endif


char* ui_WaitEnter(uint8_t entPos, uint8_t yLine, uint8_t maxEnter, uint8_t keyEvent,  uint8_t EnterType);

/**
 * @description: 获取UI按键数据缓存区按键数量
 * @return {*}
 */
uint8_t get_entBuff_length(void);

/**
 * @description: 清空按键缓冲区数据
 * @return {*}
 */
void enterBuff_clean(void);



#endif
