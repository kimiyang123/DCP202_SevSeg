#include "main.h"
#include "Joystick_Driver.h"
#include "Servo_MotorZ.h"
#include "LCD12864_Driver.h"
#include "TimTask.h"
#include "KeyPad4x4.h"

__WEAK int8_t speedArr[] = {100,0,100,0,-100,-50,0,45,0,-100};

uint8_t Happle[][2] ;
uint8_t Xapple[][2] ;

void drawApple(uint8_t appArr[][2],uint8_t flag);


uint8_t AutoRunState(uint8_t stateCMD);
void showPushTask(void *p);

int main()
{
    uint16_t i=0;
    BSP_Configuration();
    LCD_CMD_Init();
    keyPad_Init();
    Joystick_Init();
    TimTask_Init();
    ServoMotorZ_Init();

    Beep_On(20);
    
    LCD_Printf(0,0,"2022技能大赛");
    LCD_Printf(1,1,"工位号:%0.2d",9);
    LCD_Printf(0,3,"按 E键进入主界面");
    LCD_CursorSet(2,0);
    LCD_CursorON();


    while (1)
    {
        keyPad_Event();

        if(KeyPad_getLastEvent() == 16)
        {
            LCD_Clear_Screen();
            Beep_On(10);
            break;
        }
    }
    
    uint8_t state = 0,perState = 0xff;
   while (1)
   {
       keyPad_Event();

       switch (state)
       {
       case 0: // 主界面
           if(state != perState)
           {
               // 初始化页面画面
                LCD_Clear_Screen();
                LCD_Printf(0,0,"1.区域设定");
                LCD_Printf(0,1,"2.夹持力控制");
                LCD_Printf(0,2,"3.自动操作");
               perState = state;
           }
           // 页面切换按键
           switch (KeyPad_getLastEvent())
           {
           case 1:
               state = 1;
               Beep_On(15);
               break;
           case 2:
               state = 2;
               Beep_On(15);
               break;
           case 3:
               state = 3;
               Beep_On(15);
               break;

           default:
               break;
           }

           break;
       case 1: // 区域设定页面
           if (state != perState)
           {
               LCD_Clear_Screen();
               LCD_Printf(0, 0, "圆心坐标：");
               LCD_Printf(0, 1, "___ mm X___ mm");
               LCD_Printf(0, 2, "查询圆半径：");
               LCD_Printf(0, 3, "___ mm");
               LCD_CursorSet(1, 0);
               perState = state;
           }
           static uint8_t enterid = 0;
           if (enterid == 0)
           {
               ui_WaitEnter(0, 1, 3, KeyPad_getLastEvent(), ENTER_TYPE_NUM);
           }
           else if (enterid == 1)
           {
               ui_WaitEnter(4, 1, 3, KeyPad_getLastEvent(), ENTER_TYPE_NUM);
           }
           else if (enterid == 2)
           {
               ui_WaitEnter(0, 3, 3, KeyPad_getLastEvent(), ENTER_TYPE_NUM);
           }

           if (KeyPad_getLastEvent() == 16)
           {
               state = 0;
               Beep_On(15);
           }

           if (KeyPad_getLastEvent() == 9)
           {
               enterid++;
               enterid %= 3;
           }
           if (KeyPad_getLastEvent() == 13)
           {
               enterid--;
               enterid %= 3;
           }

           break;
       case 2: // 夹持力控制页面
           
           if (state != perState)
           {
               LCD_Clear_Screen();
               LCD_Printf(0, 0, "给定夹持力：");
               LCD_Printf(2, 1, "%.2fV", 3.56);
               LCD_Printf(0, 2, "实际夹持力：");
               LCD_Printf(2, 3, "%3.2fV", 2.37);

               tim2_addTask(showPushTask,10,NULL);
               perState = state;
           }

           if(KeyPad_getLastEvent() == 1)
           {// 夹紧
                Beep_On(10);
                ServoWrite(0);
           }    
           if(KeyPad_getLastEvent() == 2)
           {// 松开
                Beep_On(10);
                ServoWrite(100);
           }

           if (KeyPad_getLastEvent() == 16)
           {
               tim2_delTask(showPushTask);
               state = 0;
               Beep_On(15);
           }
           
           break;
       case 3: // 自动操作页面
           if (state != perState)
           {
               perState = state;
               LCD_Clear_Screen();
               AutoRunState(1);
               // 子页面状态机复位
           }

           if (AutoRunState(0) == 0 && KeyPad_getLastEvent() == 16)
           {
               state = 0;
               Beep_On(15);
           }

           break;
        
       
       default:
           break;
       }
       
   }
   

    // 画直线
    for(i=0;i<30;i++)
    {
        LCD_draw_dot(i+20,i+10,1);
    }

    for(i=0;i<30;i++)
    {
        LCD_draw_dot(i+20,40-i,1);
    }

    delay_ticks(2000);

    LCD_Clear_Screen();
    LCD_Printf(1,1,"绘图完毕");

    // 再次进入绘图模式
    LCD_draw_Img(96,16,FZimgs);
    LCD_draw_Img(96,22,FXimgs);

    delay_ticks(3000);
    LCD_Printf(0,3,"切回正常模式");

    while (1);

    LCD_WriteCommand(0x36); // 打开显示

    delay_ticks(5000);
    LCD_WriteCommand(LCD_BASE_CMD);
    LCD_Clear_Screen();
    LCD_CursorSet(1,0);
    LCD_Printf(0,0,"采摘结束");


    
}



/**
 * @brief : 自动操作状态机
 * @description: 
 * @return {*}
 */
uint8_t AutoRunState(uint8_t stateCMD)
{
    static stateM_def sAR_state = {.perState = 0xff};

    if(stateCMD == 1)
    {
        sAR_state.perState = 0xFF;
    }

    switch (sAR_state.thisState)
    {
    case 0:
        if(sAR_state.perState != sAR_state.thisState)
        {
            // to do once
            LCD_Clear_Screen();
            LCD_Printf(0,0,"自动操作");
            LCD_Printf(0,1,"1.绘制扫描结果");
            LCD_Printf(0,2,"2.*******");
            LCD_Printf(0,3,"3.*******");
            sAR_state.perState = sAR_state.thisState;
        }

        // to do loop function

        // to do state change
        if(KeyPad_getLastEvent() == 1)
        {
            Beep_On(10);
            sAR_state.thisState = 1;
        }

        if(KeyPad_getLastEvent() == 2)
        {
            Beep_On(10);
            LED1_Toggle();
        }

        if(KeyPad_getLastEvent() == 3)
        {
            Beep_On(10);
            LED2_Toggle();
        }

        break;
    case 1:
        if(sAR_state.perState != sAR_state.thisState)
        {
            sAR_state.perState = sAR_state.thisState;
            // to do once
            LCD_CursorON();
            LCD_Clear_Screen();
            LCD_Graph_Clear();
            LCD_CursorOFF();
            // LCD_draw_Img(0,23,FZimgs);
            drawApple(Happle,1);
            drawApple(Xapple,0);
            Beep_On(100);
        }

        // to do loop function
        if(KeyPad_getLastEvent() == 16)
        {
            Beep_On(10);
            sAR_state.thisState = 0;
        }

        break;

    default:
        break;
    } 

    return sAR_state.perState;
}

uint8_t Happle[][2] = {{100,5},{25,50},{78,35}};
uint8_t Xapple[][2] = {{30,12},{87,55},{120,15}};

void drawApple(uint8_t appArr[][2],uint8_t flag)
{
    uint8_t i=0;
    if(flag)
    {
        for(i=0;i<3;i++)
        {
            LCD_draw_Img( appArr[i][0], appArr[i][1],FZimgs);
        }  
    }
    else
    {
        for(i=0;i<3;i++)
        {
            LCD_draw_Img( appArr[i][0], appArr[i][1],FXimgs);
        }  
    }
}


void showPushTask(void *p)
{
	// static int i=0;
	// // i++;
    // // LED1_ON();
    // // delay_ticks(5);
    // // LED1_OFF();
    LCD_Printf(2,3,"%3.2f" ,  (float)getHandGrasp_Push(1)/1000.0 );
}






int main1()
{

    BSP_Configuration();

    LED1_ON();

    ServoMotorZ_Init();  //Z
    ServoWrite(0);
    MotorZ_Stop();

    Beep_On(20);
    LED1_OFF();

    delay_ticks(1000);

    Beep_On(10);
    LED1_Toggle();
    uint8_t i=0;
    while (1)
    {
       if(MotorZ_MoveByTime(speedArr[i],4000) == enMOTORz_TIMOUT)
       {
            i++;
            i %= 10;
            LED1_Toggle();
            Beep_On(10);
            
            ServoWrite( rand() % 100 );
       }
    }
    
    

    while (1)
    {        
        delay_ticks(2000);
        ServoWrite(50);
            MotorZ_RunSig(-50);
        delay_ticks(3000);

            MotorZ_Stop();
        ServoWrite(30);
            MotorZ_RunSig(30);
        delay_ticks(4000);

        ServoWrite(70);
        delay_ticks(1000);
            MotorZ_Stop();

    }
    

}

