#ifndef __SENSOR_ENCODER_H__
#define __SENSOR_ENCODER_H__
		
    #include "Motor_HPWM.h"
    #include "Motor_LocSensor.h"

    #define NULL 0

    // 定义传感器RAW 值与距离 mm直接的比例关系
    #define RAW_DIS_RATE        0.025           //0.025mm/脉冲

    /**
     * @brief : 用于将距离测量传感器与电机轴的绑定。
     * @description: 
     * @param  motX:{motorInfo_def*} X轴电机对象指针
     * @param  motY:{motorInfo_def*} Y轴电机对象指针
     * @return {void}
     */    
    void senEncoder_Init(motorInfo_def* motX,motorInfo_def* motY);


    /**
     * @brief : 将传感器原始数据RAW，转换为距离mm值
     * @description: 
     * @param  senRaw: {int32_t} 传递需要转换的传感器RAW
     * @return {*}
     */
    int16_t senRawToDis(int32_t senRaw);

    /**
     * @brief : 将距离值mm，转换为传感器原始数据RAW
     * @description: 
     * @param  dis_mm: {uint16_t}传递需要转换的距离数值mm
     * @return {*}
     */    
    int32_t disToSenRaw(uint16_t dis_mm);


#endif


