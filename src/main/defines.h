#ifndef __DEFINES_H
#define __DEFINES_H

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "../motor/motor.h"

#define _DEBUG

typedef enum{
	left=0,
	open=0,
	middle=1,
	right=2,
	close=2
} postionStatus;

typedef struct
{
	volatile bool flag_running;//是否在运行
	volatile motorStatus runningFlagx;//全向轮电机,即控制x方向移动的电机
	volatile motorStatus runningFlag0;//贴壁装置
	volatile motorStatus runningFlag1;//卷扬机
	volatile motorStatus runningFlag2;//挡料板
	volatile motorStatus runningFlag3;//推板
	volatile motorStatus runningFlag4;//侧挡板
	volatile motorStatus runningFlag5;//推料伸缩机
	//位置,左中右,或开闭
	volatile postionStatus postionFlag0;
	volatile postionStatus postionFlag1;
	volatile postionStatus postionFlag2;
	volatile postionStatus postionFlag3;
	volatile postionStatus postionFlag4;
	volatile postionStatus postionFlag5;
	
	unsigned volatile char num;//当前在装载的物料编号,1,2,3,4
	unsigned volatile char height;//卸料装置的高度
	unsigned volatile char column;//整个机器所在列数
	unsigned volatile char localStep;//当前所在的步骤
} sysState;

#define MaxHeight 7//最大高度
#define MaxColumn 8//最大列数 y方向需要进行的循环次数

//定义推板第一次运动的方向的反方向为x方向,机器进入集装箱的方向的反方向为y方向,高度方向为z方向

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
