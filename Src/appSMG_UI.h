/*
 * @Date: 2022-05-20 09:21:18
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-21 14:01:46
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



char* ui_WaitEnter(uint8_t entPos, uint8_t maxEnter, uint8_t keyEvent,  uint8_t EnterType);
// char* ui_WaitEnter(uint8_t entPos, uint8_t maxEnter, uint8_t EnterType);

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
