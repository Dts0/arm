#ifndef __CONFIG_H
#define __CONFIG_H

#define _DEBUG//调试模式
#include "../main/defines.h"
#include "stdbool.h"


#define motorThreadStackSize 500

#define _SEND_START_CHAR "{"//'发送开始'标志
#define _SEND_END_CHAR "}"//'发送停止'标志
#define _SEND_DELAY_TIME 1000//串口发送数据的间隔

#define D_JuanYangJi 100//卷扬机轮直径
#define D_ShenSuoJiDianJi 100//伸缩机电机轮直径,单位mm 需要在计算'离集装箱底的距离'时使用
#define BoxLength 12000//集装箱长度,单位mm 需要在计算'离集装箱底的距离'时使用
#define CarLength 2980//小车长度,单位mm 需要在计算'离集装箱底的距离'时使用
#define DistanceLeft 10//'离集装箱底的距离'的标准值

#define ItemLength 910//物料长度,等于小车后退的距离
#define ItemHeight 960//物料高度,等于卸料盘每层的上升或下降高度
#define ItemWidth //物料宽度,暂不需要
#define MaxHeight 7//最大高度
#define MaxColumn 8//最大列数 y方向需要进行的循环次数


/**管脚定义
*PA09 Tx ;PA10 Rx
*PE06 急停用,控制一个继电器
*PC0-13 LEDs
*encoder1暂未配置,卷扬机上下 绝对编码器
*PB6,7 TIM4 ch1,2 对应encoder2,小车前后,增量编码器,PB1为归零
*LCD屏幕:PB0,PD0,1,4,5,8,9,10,14,15,PE7-15,PG0,12
*传感器(按照传感器函数的声明顺序,当前总共13个):PD2,3,6,7,11,12,13,PE0-5
*电机或汽缸:motory:PF0,1	motor0:PF2,3	motor1:PF4,5	motor2:PF6,7	motor3:PF8,9	motor4:PF10,11
*						motor5:PF12,13	motor6:PF14,15	motor7:PG2,3	motor8:PG4,5	motor9:PG6,7
**/


/**需要再额外配置的位置
*3> encoder1的配置
*5> 如果需要,添加LCD显示屏模块
**/


//传感器函数声明,默认传感器都是检测到为高电位
void sensorsInit(void);//传感器初始化

bool HasEnteredBox(void);//光电传感器返回是否已经进箱子
bool HasItems(void);//检查动力滚筒上是否有料
bool HasArrivedAtExtremePosition(void);//检测料到达动力滚筒极限位置
bool HasArrivedAtB(void);//B传感器检测位置推板
bool HasArrivedAtC(void);
bool HasArrivedAtD(void);
bool HasArrivedAtE(void);

positionStatus getTieBiPosition(void);//返回贴壁汽缸的汽缸电磁传感器值
positionStatus getCeDangBanPosition(void);//返回侧挡板汽缸的汽缸电磁传感器值
positionStatus getTuiBanPosition(void);//返回推板垂直汽缸的汽缸电磁传感器值
positionStatus getTuiBanTuiChuPosition(void);//返回推板推出汽缸的汽缸电磁传感器值
positionStatus getDangLiaoBanChuiZhiPosition(void);//返回挡料板垂直汽缸的汽缸电磁传感器值
positionStatus getDangLiaoBanTuiChuPosition(void);//返回挡料板推出气缸的汽缸电磁传感器值

#ifdef _DEBUG
void SetHasEnteredBox(bool msg);//设置光电传感器,是否已经进箱子
void SetHasItems(bool msg);//设置动力滚筒上是否有料
void SetHasArrivedAtExtremePosition(bool msg);//设置料到达动力滚筒极限位置
void SetHasArrivedAtB(bool msg);//设置B传感器
void SetHasArrivedAtC(bool msg);
void SetHasArrivedAtD(bool msg);
void SetHasArrivedAtE(bool msg);

void setTieBiPosition(bool msg);//设置贴壁汽缸的汽缸电磁传感器值
void setCeDangBanPosition(bool msg);//设置侧挡板汽缸的汽缸电磁传感器值
void setTuiBanPosition(bool msg);//设置推板垂直汽缸的汽缸电磁传感器值
void setTuiBanTuiChuPosition(bool msg);//设置推板推出汽缸的汽缸电磁传感器值
void setDangLiaoBanChuiZhiPosition(bool msg);//设置挡料板垂直汽缸的汽缸电磁传感器值-
void setDangLiaoBanTuiChuPosition(bool msg);//设置挡料板推出气缸的汽缸电磁传感器值


#endif
//电机或汽缸停止函数声明
void motor0_TieBi_stopFunc(void);
void motor4_CeDangBan_stopFunc(void);

#endif
