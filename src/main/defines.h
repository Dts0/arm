#ifndef __DEFINES_H
#define __DEFINES_H

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "../motor/motor.h"
#include "../main/config.h"


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
	volatile motorStatus runningFlagy;//全向轮电机,即控制x方向移动的电机
	volatile motorStatus runningFlag0_TieBiZhuangZhi;//贴壁装置
	volatile motorStatus runningFlag1_JuanYangJi;//卷扬机
	volatile motorStatus runningFlag2_DangLiaoBan;//挡料板
	volatile motorStatus runningFlag3_TuiBan;//推板
	volatile motorStatus runningFlag4_CeDangBan;//侧挡板
	volatile motorStatus runningFlag5_TuiLiaoShenSuoJi;//推料伸缩机
	//位置,左中右,或开闭
	volatile postionStatus postionFlag0_TieBiZhuangZhi;
	volatile postionStatus postionFlag1_JuanYangJi;
	volatile postionStatus postionFlag2_DangLiaoBan;
	volatile postionStatus postionFlag3_TuiBan;
	volatile postionStatus postionFlag4_CeDangBan;
	volatile postionStatus postionFlag5_TuiLiaoShenSuoJi;
	
	unsigned volatile char num;//当前在装载的物料编号,1,2,3,4
	volatile char height;//卸料装置的高度
	unsigned volatile char column;//整个机器所在列数
	unsigned volatile char localStep;//当前所在的步骤
} sysState;

#endif
