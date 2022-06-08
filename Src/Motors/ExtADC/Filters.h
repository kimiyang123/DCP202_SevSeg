#ifndef __KALMAN_FILTER_H__
#define __KALMAN_FILTER_H__

    //1. 结构体类型定义
    typedef struct 
    {
        float LastP;        //上次估算协方差 初始化值为0.02
        float Now_P;        //当前估算协方差 初始化值为0
        float out;          //卡尔曼滤波器输出 初始化值为0
        float Kg;           //卡尔曼增益 初始化值为0
        float Q;            //过程噪声协方差 初始化值为0.001
        float R;            //观测噪声协方差 初始化值为0.543
    }KFP_typedef;       //Kalman Filter parameter


    extern KFP_typedef KFP_param ;

    float kalmanFilter(KFP_typedef *kfp,float input);


    // 定义ADC采集数据是否使用 滤波器 filter，不需要则屏蔽以下define
    #define ADC_FILTER(inData)      kalmanFilter(&KFP_param,inData)

#endif

