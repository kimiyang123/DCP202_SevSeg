#ifndef __OPERATE_AREA_H__
#define __OPERATE_AREA_H__

    #include "appSMG_UI.h"
    typedef struct
    {
        uint16_t xPos; // x坐标值
        uint16_t yPos; // y坐标值
    } Point_struct_def;

    extern Point_struct_def myPoint;

    Point_struct_def ui_getEnterPoint(char *entstr);

    uint8_t app_setArea(void);

    void app_setArea_reset(void);



    uint8_t app_viewParam(void);

#endif
