#include "Filters.h"


KFP_typedef KFP_param = {
    .LastP = 0.02,      //上次估算协方差 初始化值为0.02
    .Now_P = 0,         //当前估算协方差 初始化值为0
    .out = 0,           //卡尔曼滤波器输出 初始化值为0
    .Kg = 0,            //卡尔曼增益 初始化值为0
    .Q = 0.001,         //过程噪声协方差 初始化值为0.001
    .R = 0.543          //观测噪声协方差 初始化值为0.543
};

 /**
  * @brief : 
  * @description: 
  * @param {KFP_typedef} *kfp: 卡尔曼滤波器结构体参数
  * @param {float} input : 需要滤波的参数测量值（即传感器采集值）
  * @return {*}滤波后的参数（最优值）
  */
 float kalmanFilter(KFP_typedef *kfp,float input)
 {
     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
 }





