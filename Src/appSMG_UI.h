/*
 * @Date: 2022-05-20 09:21:18
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-20 23:54:58
 * @FilePath: \DCP202_SevSeg\Src\appSMG_UI.h
 * @Description: 数码管用户操作人机接口界面程序
 * 
 */
#ifndef __APP_UI_SMG_H__
#define __APP_UI_SMG_H__

#include "KeyPad4x4.h"
#include "BSP.h"
#include "Bsp_sevseg.h"
#include "string.h"
#include "stdlib.h"

#include "ctype.h"

#define  ENTER_TYPE_NUM     0x00
#define  ENTER_TYPE_ALL     0x01

/**
 * @description:等待输入字符 
 * @return {*}
 */
char* ui_WaitEnter(uint8_t entPos, uint8_t maxEnter, uint8_t keyEvent,  uint8_t EnterType);
// char* ui_WaitEnter(uint8_t entPos, uint8_t maxEnter, uint8_t EnterType);


typedef struct 
{
    uint16_t xPos;  //x坐标值
    uint16_t yPos;  //y坐标值
}Point_struct_def;

extern Point_struct_def myPoint;

Point_struct_def ui_getEnterPoint(char *entstr);



uint8_t app_setArea(void);
void app_setArea_reset(void);


#endif
