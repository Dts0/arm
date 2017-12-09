#ifndef __CONFIG_H
#define __CONFIG_H

#define _DEBUG
#include "stdbool.h"

bool HasEnteredBox();//光电传感器返回是否已经进箱子
bool HasItems();//检查动力滚筒上是否有料
bool HasArrivedAtExtremePosition();//检测料到达动力滚筒极限位置
bool HasArrivedAtB();//B传感器检测位置推板
bool HasArrivedAtC();
bool HasArrivedAtD();
bool HasArrivedAtE();

positionStatus getTieBiPosition();//返回贴壁汽缸的汽缸电磁传感器值
positionStatus getCeDangBanPosition();//返回侧挡板汽缸的汽缸电磁传感器值
positionStatus getTuiBanPosition();//返回推板垂直汽缸的汽缸电磁传感器值
positionStatus getDaiDaoGanPosition();//返回带导杆汽缸的汽缸电磁传感器值
positionStatus getDangLiaoBanChuiZhiPosition();//返回挡料板垂直汽缸的汽缸电磁传感器值
positionStatus getDangLiaoBanTuiChuPosition();//返回挡料板推出气缸的汽缸电磁传感器值

void motor0_TieBi_stopFunc();

#define motorThreadStackSize 100

#define _SEND_DELAY_TIME 1000//串口发送数据的间隔

#define D_JuanYangJi 100//卷扬机轮直径
#define D_ShenSuoJiDianJi 100//伸缩机电机轮直径,单位mm 需要在计算'离集装箱底的距离'时使用
#define BoxLength 12000//集装箱长度,单位mm 需要在计算'离集装箱底的距离'时使用
#define CarLength 2980//小车长度,单位mm 需要在计算'离集装箱底的距离'时使用
#define DistanceLeft 10//'离集装箱底的距离'的标准值

#define ItemLength 910//物料长度,等于小车后退的距离
#define ItemHeight 960//物料高度,等于卸料盘每层的上升或下降高度
#define ItemWidth //物料宽度
#define MaxHeight 7//最大高度
#define MaxColumn 8//最大列数 y方向需要进行的循环次数


//未定义仅声明的函数


/**管脚定义
*PA09 Tx ;PA10 Rx
*PC LEDs
*PB6,7,8,9 TIM4 ch1,2,3,4 对应encoder1
**/


/**需要再额外配置的位置
*1> main.c 70 void motorsConfig(Motors *ms)中对添加的电机的配置,注意添加电机停止条件函数stopFunc的指针
*2> 添加关于步骤localStep的修改代码
*3> encoder.h,encoder.c 中对编码器的控制,如果编码器数量大于1
*4> thread.c 32 void thread_main(void *p)中对主函数根据工作过程修改
*5> 如果需要,添加LCD显示屏模块
*6>main.c 78 void flagInit(sysState *flags)中对flag初始化
**/
#endif
