
#include "LCD12864_Driver.h"

#include <stdarg.h>


#define DATA_MODE_INPUT         0
#define DATA_MODE_OUTPUT        1

/**
 * @brief : LCD数据管脚模式设定
 * @description: 
 * @param sMode: {uint8_t} DATA_MODE_INPUT or DATA_MODE_OUTPUT
 * @return {*}
 */
void LCD12864_DatePort_Mode(uint8_t sMode )
{
    GPIO_InitTypeDef sGPIOinit;
    sGPIOinit.GPIO_Mode = sMode == 0 ? GPIO_Mode_IPU : GPIO_Mode_Out_PP;
    sGPIOinit.GPIO_Pin = LCD_DATA_PinS;
    sGPIOinit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_DATA_Port,&sGPIOinit);
}


void LCD12864_PortInit(void)
{

    // 修改管脚，需要修改管脚的GPIO RCC使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

    /*** 初始化 LCD控制引脚为输出 ***/
    GPIO_InitTypeDef sGPIOinit;
    sGPIOinit.GPIO_Mode = GPIO_Mode_Out_PP;
    sGPIOinit.GPIO_Pin = LCD_RS_Pin;
    sGPIOinit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_RS_Port,  &sGPIOinit);

    sGPIOinit.GPIO_Pin = LCD_RW_Pin;
    GPIO_Init(LCD_RW_Port,  &sGPIOinit);

    sGPIOinit.GPIO_Pin = LCD_EN_Pin;
    GPIO_Init(LCD_EN_Port,  &sGPIOinit);

    sGPIOinit.GPIO_Pin = LCD_RST_Pin;
    GPIO_Init(LCD_RST_Port,  &sGPIOinit);

    LCD12864_DatePort_Mode(DATA_MODE_OUTPUT);
    
}



uint8_t LCD_ReadBusy(void)
{
    uint8_t Result;
    LCD12864_DatePort_Mode(DATA_MODE_INPUT);
    
    GPIO_WriteBit(LCD_RS_Port,LCD_RS_Pin,0);
    GPIO_WriteBit(LCD_RW_Port,LCD_RW_Pin,1);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,1);
    
    delay_ticks(1);
    Result = LCD_DATA_Port->IDR & 0x80;

    // CLEAR_BIT( LCD_EN_Port->ODR, LCD_EN_Pin);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);

    LCD12864_DatePort_Mode(DATA_MODE_OUTPUT);
    return Result;
}


/**
 * @brief : 写指令到LCD
 * @description: RS=L，RW=L，E=高脉冲，D0-D7=指令码。
 * @param {uint8_t} cmd
 * @return {*}
 */
void LCD_WriteCommand(uint8_t cmd)
{
    while (LCD_ReadBusy());     //判忙

    GPIO_WriteBit(LCD_RS_Port,LCD_RS_Pin,0);
    GPIO_WriteBit(LCD_RW_Port,LCD_RW_Pin,0);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);

    LCD_DATA_Port->ODR = (LCD_DATA_Port->ODR & 0xFF00) | cmd;
    delay_ticks(0);     // 可屏蔽，提高显示效率
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,1);

    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);
    
}


/**
 * @brief : 写显示数据到LCD   
 * @description: RS=H，RW=L，E=高脉冲，D0-D7=数据。  
 * @param {uint8_t} u8data
 * @return {*}
 */
void LCD_WriteData(uint8_t u8data)
{
    while (LCD_ReadBusy());     //判忙

    GPIO_WriteBit(LCD_RS_Port,LCD_RS_Pin,1);
    GPIO_WriteBit(LCD_RW_Port,LCD_RW_Pin,0);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);

    LCD_DATA_Port->ODR = (LCD_DATA_Port->ODR & 0xFF00) | u8data;
    delay_ticks(1);

    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,1);

    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);
}



uint8_t LCD_ReadData(void)
{
    uint8_t r_data;
    // Graph 扩展指令下的读取数据无需判忙，否则读取数据不正确
    // while (LCD_ReadBusy());     //判忙

    LCD12864_DatePort_Mode(DATA_MODE_INPUT);
    GPIO_WriteBit(LCD_RS_Port,LCD_RS_Pin,1);
    GPIO_WriteBit(LCD_RW_Port,LCD_RW_Pin,1);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,1);

    delay_ticks(1);
    r_data = LCD_DATA_Port->IDR & 0x00ff;
    
    GPIO_WriteBit(LCD_EN_Port,LCD_EN_Pin,0);

    LCD12864_DatePort_Mode(DATA_MODE_OUTPUT);

    return r_data;
}


/**
 * @brief : LCD屏 端口+功能指令初始化
 * @description: 
 * @return {*}
 */
void LCD_CMD_Init(void)
{
    LCD12864_PortInit();

    // RESET pin 复位
    GPIO_WriteBit(LCD_RST_Port,LCD_RST_Pin,0);
    delay_ticks(3);
    GPIO_WriteBit(LCD_RST_Port,LCD_RST_Pin,1);

    LCD_WriteCommand(0X34);     // 扩充指令操作
    delay_ticks(5);
    LCD_WriteCommand(0X30);     // 基本指令操作
    delay_ticks(5); 
    LCD_WriteCommand(0X0C);     //
    delay_ticks(5);
    LCD_WriteCommand(0X01);

}


/**
 * @brief : 设置屏幕光标位置
 * @description:  0x0C|0x02 : 光标显示
 * @param  Line : {uint8_t} Y 高度值[0-7]
 * @param  xPos : {uint8_t} X 横向坐标[0-127]
 * @return {*}  
 */
void LCD_CursorSet(uint8_t Line,uint8_t xPos )
{
    
    uint8_t CursorPos = 0;
    // 限定参数范围
    if(Line >= 4) Line = 3;
    if(xPos >= 8) xPos = 7;

    LCD_WriteCommand(LCD_BASE_CMD);     //切换到基本指令

    if(Line == 0)           CursorPos = 0x80;
    else if(Line == 1)      CursorPos = 0x90;
    else if(Line == 2)      CursorPos = 0x88;
    else if ( Line == 3)    CursorPos = 0x98;

    CursorPos += xPos;
    LCD_WriteCommand(CursorPos);
}


void LCD_StringPrint(char *pStr , uint8_t line,uint8_t xPos)
{
    LCD_CursorSet(line,xPos);
    while (*pStr != '\0')   // 字符结束符
    {
        LCD_WriteData(*pStr++);
    }
}


/**
 * @brief : 液晶屏显示整数方法
 * @description: 
 * @param  sNum:{uint32_t} 需要显示的数值
 * @param  line:{uint8_t} 显示在哪一行[0-3]
 * @param  xPos:{uint8_t} 从哪一列开始显示[0-7]
 * @param  showBit:{uint8_t} sNum数值显示几位数[0-6]
 * @return {void}
 */
void LCD_ShowInt(uint32_t sNum,uint8_t line,uint8_t xPos, uint8_t showBit)
{
    char fmt[10];
    char sBuff[10];
    uint8_t *p = sBuff;

    sNum %=(uint16_t)pow(10,showBit);

    LCD_CursorSet(line,xPos);
    sprintf(fmt,"%%0.%dd",showBit);
    sprintf(sBuff,fmt,sNum);

    while (*p != '\0')
    {
        LCD_WriteData(*p++);
    }
}


/**
 * @brief : LCD12864 格式化打印显示函数
 * @description: 
 * @param xPos :{uint8_t} 打印字符的X位置
 * @param yPos :{uint8_t} 打印字符的Y位置
 * @param fmt : 格式化字符串，可带有转义字符如："num:%0.3d",25 固定显示3位数据，不足位
 *              以0不足
 * @return {*}
 */
void LCD_Printf(uint8_t xPos, uint8_t yPos, const char *fmt,...)
{
    char sBuff[25] = {0}; 
    uint8_t *p = sBuff;
    va_list args;
    va_start(args,fmt);
    vsprintf(sBuff,fmt,args);
    va_end(args);

    LCD_CursorSet(yPos,xPos);
    while (*p != '\0')
    {
        LCD_WriteData(*p++);
    }
    
}


/**
 * @brief : 清除12864屏 指定行和X位置的 指定数量个屏幕数据
 * @description: 
 * @param xPos: {uint8_t} 屏幕坐标系X位置
 * @param yLine: {uint8_t} 屏幕行号
 * @param amount:{uint8_t} 清除字符数 AscII码
 * @return {*}
 */
void LCD_clearPos(uint8_t xPos,uint8_t yLine, uint8_t amount)
{
    if(amount > 16) amount = 0;
    LCD_CursorSet(yLine,xPos);
    while (amount != 0)
    {
        LCD_WriteData(' ');
        amount --;
    }
}


/**
 * @brief : 清屏指令，在基本指令模式下有效。
 * @description: 
 * @return {*}
 */
void LCD_Clear_Screen(void)
{
    LCD_WriteCommand(LCD_BASE_CMD);     //切换到基本指令
    LCD_WriteCommand(0x01);  //清屏指令
}


/**
 * @brief : Graph RAM  绘点模式下的清空
 * @description: 
 * @return {*}
 */
void LCD_Graph_Clear(void)
{
    uint8_t i=0,j;
    LCD_WriteCommand(LCD_EXTEN_CMD);    //进入扩展模式 并关闭显示

    //  清空上半屏
    for ( i = 0; i < 32; i++)
    {
        LCD_WriteCommand(0x80+i);        // 1- 写入水平坐标 Y
        LCD_WriteCommand(0x80);        // 2- 写入垂直坐标   X
        for ( j = 0; j < 16; j++)
        {
            LCD_WriteData(0x00);
        }
    }

    //  清空下半屏
    for ( i = 0; i < 32; i++)
    {
        LCD_WriteCommand(0x80+i);        // 1- 写入水平坐标 Y
        LCD_WriteCommand(0x88);        // 2- 写入垂直坐标 X
        for ( j = 0; j < 16; j++)
        {
            LCD_WriteData(0x00);
        }
    }
    
    LCD_WriteCommand(LCD_EXTEN_CMD | LCD_EXT_SHOWON);    //进入扩展模式 并开启显示
}


/**
 * @brief : 设置图形模式下的光标位置，必须先进入扩展指令模式
 * @description: 
 * @param  x : {uint8_t} 横向坐标值 [0-7]  X位置指向横向的16bit数据
 * @param  y : {uint8_t} 纵向坐标值 [0-63]
 * @return {*}
 */
void LCD_Graph_XY(uint8_t x,uint8_t y)
{
    uint8_t xPos,yPos;
    // 限定范围
    if(x>=8) x = 8;
    if(y>=64) y = 63;

    xPos = x + (y>=32 ? 0x88 : 0x80);
    yPos = 0x80 + (y % 32);

    LCD_WriteCommand(yPos);        // 1- 写入水平坐标 Y
    LCD_WriteCommand(xPos);        // 2- 写入垂直坐标 X

}


/**
 * @brief : LCD 扩展画点方法，注意必须开启屏幕的扩展模式指令
 * @description: 
 * @param x : {uint8_t} 以像素点bit位为坐标，范围：[0-127]
 * @param y : {uint8_t} 以像素点为坐标  范围：[0-63]
 * @param bitOnOrOff : 1 or 0 
 * @return {*}
 */
void LCD_draw_dot(uint8_t x,uint8_t y, uint8_t bitOnOrOff)
{
    uint16_t wData = 0;     // 16bit数据
    //  限定x y 坐标 X<128 Y<64
    
    if(x > 127) x = 127;
    if(y > 63)  y = 63;

    // LCD_WriteCommand(LCD_EXTEN_CMD);    //进入扩展命令模式
    LCD_WriteCommand(LCD_EXTEN_CMD | LCD_EXT_SHOWON);    //进入扩展模式 并开启显示

//1- 读，读取1个16位数据
    LCD_Graph_XY(x/16,y);
    LCD_ReadData();         //预读取数据 【重要操作】
    wData |= LCD_ReadData() & 0x00ff;       //读取当前显示高8位数据
    wData <<= 8;
    wData |= LCD_ReadData() & 0x00ff;       //读取当前显示低8位数据

//2- 改，修改对于的bit位
    if(bitOnOrOff)
    {
        wData |= 0x8000 >> ( x%16 );
    }
    else
    {
        wData &= ~( 0x8000 >> (x%16) );
    }

//3- 写，写入修改后的数据到屏幕指定坐标
    LCD_Graph_XY(x/16,y);
    LCD_WriteData(wData >> 8);
    LCD_WriteData(wData);

}



// 图像取模数据，前2个字节数据分别表示图像的 宽度 和 高度 (像素点)
uint8_t FZimgs[] = {8,5,  0x10,0x10,0x7C,0x10,0x10  };      // + 字符号
uint8_t FXimgs[] = {8,5,  0x44,0x28,0x7C,0x28,0x44  };      // * 字符号


/**
 * @brief : 绘制点阵图像
 * @description: 
 * @param  xStartPos : 图像左上角绘制 起始X轴位置
 * @param  yStartPos : 图像左上角绘制 起始Y轴位置
 * @param  img : {uint8_t*}图像数组地址指针，图像数组中前2个必须包含图像(宽度)和(高度)数据
 * @return {*}
 */
void LCD_draw_Img(  uint8_t xStartPos,uint8_t yStartPos, uint8_t img[])
{
    uint16_t imgMaxDot;
    uint16_t drawIndex;
    uint8_t img_With;
    // imgMaxDot = sizeof(img)/sizeof(uint8_t) * 8;    //图像数据bit大小
    // if(img_With * img_height < imgMaxDot) imgMaxDot = img_With*img_height;
    // imgMaxDot = img_With*img_height;
    img_With = img[0];
    imgMaxDot = img[0]*img[1];

    img += 2;   // 跳过宽度和高度数据字节

    for(drawIndex = 0; drawIndex < imgMaxDot ; drawIndex ++)
    {
        uint8_t bitMask = 0;
        
        bitMask = img[ drawIndex / 8 ] & (0x80 >> (drawIndex % 8) );
        LCD_draw_dot(xStartPos + drawIndex%img_With ,
                    yStartPos + drawIndex/img_With,
                    bitMask);
    }

}   

