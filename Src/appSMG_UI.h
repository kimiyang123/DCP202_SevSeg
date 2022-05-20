/*
 * @Date: 2022-05-20 09:21:18
 * @LastEditors: kimiyang
 * @LastEditTime: 2022-05-20 14:03:10
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

/**
 * @description:等待输入字符 
 * @return {*}
 */
char* ui_WaitEnter(uint8_t entPos, uint16_t wTimeMax);




#endif
