#ifndef __CONFIG_H
#define __CONFIG_H

#define _DEBUG

//定义推板第一次运动的方向的反方向为x方向,机器进入集装箱的方向的反方向为y方向,高度方向为z方向

#define motorThreadStackSize 100

#define _SEND_DELAY_TIME 1000//串口发送数据的间隔

#define MaxHeight 7//最大高度
#define MaxColumn 8//最大列数 y方向需要进行的循环次数


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
