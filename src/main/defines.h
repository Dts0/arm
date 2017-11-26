#ifndef __DEFINES_H
#define __DEFINES_H

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"

typedef struct
{
	volatile bool flag0;//贴壁装置
	volatile bool flag1;//卷扬机
	volatile bool flag2;//挡料板
	unsigned volatile char flag3;//推板位置,左中右
	volatile bool flag4;//侧挡板
	volatile bool flag5;//推料伸缩机
	unsigned volatile char num;//当前在装载的物料编号
	unsigned volatile char height;//卸料装置的高度
	unsigned volatile char column;//整个机器所在列数
	unsigned volatile char localStep;//当前所在的步骤
} sysState;



#define _open true
#define _close false
	
#define _left (unsigned volatile char)0
#define _middle (unsigned volatile char)127
#define _right (unsigned volatile char)255

/**管脚定义
*PA09 Tx ;PA10 Rx
*PC LEDs
*PB6,7,8,9 TIM4 ch1,2,3,4 对应encoder1
**/

#endif
