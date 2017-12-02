#ifndef __DEFINES_H
#define __DEFINES_H

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "../motor/motor.h"

#define _DEBUG

typedef enum{
	left=0,
	middle=1,
	right=2
} flagStatus2;

typedef struct
{
	volatile bool flag_running;//是否在运行
	volatile motorStatus flag0;//贴壁装置
	volatile motorStatus flag1;//卷扬机
	volatile motorStatus flag2;//挡料板
	volatile motorStatus flag3;//推板
	volatile motorStatus flag4;//侧挡板
	volatile motorStatus flag5;//推料伸缩机
	//位置,左中右
	unsigned volatile char num;//当前在装载的物料编号,1,2,3,4
	unsigned volatile char height;//卸料装置的高度
	unsigned volatile char column;//整个机器所在列数
	unsigned volatile char localStep;//当前所在的步骤
} sysState;

/**管脚定义
*PA09 Tx ;PA10 Rx
*PC LEDs
*PB6,7,8,9 TIM4 ch1,2,3,4 对应encoder1
**/

#endif
