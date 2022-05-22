#include "operateSetArea.h"




Point_struct_def myPoint;

/**
 * @description: 
 * @param entstr: 从按键获取的字符串
 * @return {*}
 */
Point_struct_def ui_getEnterPoint(char *entstr)
{
    Point_struct_def _Point;
    char xPosStr[5];
    char yPosStr[5];

    if (entstr == NULL)
        return;

    memcpy(xPosStr, entstr, 4);
    if (strlen(entstr) > 4)
    {
        memcpy(yPosStr, entstr + 4, 4);
    }
    _Point.xPos = atoi(xPosStr);
    _Point.yPos = atoi(yPosStr);

    return _Point;
}

uint8_t funState = 0, perfunState = 0xff;
// 设置矩形坐标
Point_struct_def pointStart, pointEnd;

void app_setArea_reset(void)
{
    funState = 0;
    perfunState = 0xff;
}

/**
 * @description: area函数描述
 * @return {*}
 */
uint8_t app_setArea(void)
{
    char cmd[4];
    char *pStr;
    uint8_t keytTMP;
    keytTMP = getKeyLast();
    switch (funState)
    {
    case 0: //主菜单
        if (perfunState != funState)
        {
            perfunState = funState;
            SMG_print("S00     ", 0);
            delay_ticks(1000);
            enterBuff_clean();
        }
        pStr = ui_WaitEnter(1, 3, keytTMP, ENTER_TYPE_ALL);
        if (pStr != NULL)
        {
            //拷贝前3个字符到cmd中
            strncpy(cmd, pStr, 3);
            if (cmd[0] == 'S' && isdigit(cmd[1]) && isdigit(cmd[2]))
            {
                // 获取命令S00后面的数字
                funState = atoi(&cmd[1]);
            }
            else
            {
                SMG_print("err", 0);
                Beep_On(200);
                delay_ticks(1000);
            }
            SMG_CleanPos(0, 3);
        }
        break;

    case 01: //设置起点
        if (perfunState != funState)
        {
            perfunState = funState;
            SMG_ShowInt(pointStart.yPos, 0, 4);
            SMG_ShowInt(pointStart.xPos, 4, 4);
            delay_ticks(1500);
        }

        pStr = ui_WaitEnter(0, 8, keytTMP, ENTER_TYPE_NUM);
        if (pStr != NULL)
        {
            pointStart = ui_getEnterPoint(pStr);
        }
        if (keytTMP == 16)
        {
            funState = 0; //回到主菜单
            SMG_CleanPos(0, 8);
        }
        if(getKeyLast() == 9 && get_entBuff_length() == 0)
             perfunState = 0xff;
        break;
    case 02: //设置终点
        if (perfunState != funState)
        {
            perfunState = funState;
            SMG_ShowInt(pointEnd.yPos, 0, 4);
            SMG_ShowInt(pointEnd.xPos, 4, 4);
            delay_ticks(1500);
        }

        pStr = ui_WaitEnter(0, 8, keytTMP, ENTER_TYPE_NUM);
        if (pStr != NULL)
        {
            pointEnd = ui_getEnterPoint(pStr);
        }
        if (keytTMP == 16)
        {
            funState = 0; //回到主菜单
            SMG_CleanPos(0, 8);
        }
        if(getKeyLast() == 9 && get_entBuff_length() == 0)
            perfunState = 0xff;
        break;

    default:
        funState = 0;
        break;
    }

    return funState;
}



/**
 * @description: 查看内部参数
 * @return {*}
 */
uint8_t app_viewParam(void)
{
    static uint8_t state,perState;
    char *pstr;
    char cmd[4];
    switch (state)
    {
    case 0:     // 主页面，等待输入查询命令
        if(state != perState)
        {
            SMG_CleanAll();
            SMG_print("view",0);
            perState = state;
        }

        pstr = ui_WaitEnter(1,3,getKeyLast(),ENTER_TYPE_ALL);
        if(pstr !=NULL)
        {
            strncpy(cmd,pstr,3);
            if(cmd[0] == 'F' && isdigit(cmd[1]) && isdigit(cmd[2]))
            {
                state = atoi(&cmd[1]);
            }
            else{
                SMG_print("err",0);
                Beep_On(200);
                delay_ticks(1000);
            }
            
        }
        break;
    case 1:
        if(perState != state)
        {
            SMG_print("F01",0);
            state = perState;
        }
        if(getKeyLast() == 1) state = 0;
        break;
    case 2:
        if(perState != state)
        {
            SMG_print("F02",0);
            state = perState;
        }
        if(getKeyLast() == 1) state = 0;
        break;
    case 3:
        if(perState != state)
        {
            SMG_print("F03",0);
            state = perState;
        }
        if(getKeyLast() == 1) state = 0;
        break;
    default:
        break;
    }

}