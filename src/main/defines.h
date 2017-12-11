#ifndef __DEFINES_H
#define __DEFINES_H

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "../motor/motor.h"


typedef enum{
	/*
	left=0,
	close=0,
	middle=1,
	right=2,
	open=2
	*/
	close=0,
	open=1
} positionStatus;


typedef struct
{
	//电机或汽缸状态 on,off,rev
	volatile bool flag_running;//是否在运行
	volatile motorStatus runningFlagy;//伸缩机,即控制y方向移动的电机,on为进箱(前行)
	volatile motorStatus runningFlag0_TieBiZhuangZhi;//贴壁汽缸,on对应伸出
	volatile motorStatus runningFlag1_JuanYangJi;//卷扬机,卷扬机on对应卸料板下行
	volatile motorStatus runningFlag2_DangLiaoBanChuiZhi;//挡料板垂直移动汽缸
	volatile motorStatus runningFlag3_TuiBanChuiZhi;//推板垂直移动汽缸
	volatile motorStatus runningFlag4_CeDangBan;//侧挡板汽缸
	volatile motorStatus runningFlag5_DangLiaoBanTuiChu;//挡料板推出汽缸,推出为on,缩回为rev
	volatile motorStatus runningFlag6_TuiBanTuiChu;//推板推出汽缸
	volatile motorStatus runningFlag7_DongLiGunTong;//动力滚筒
	volatile motorStatus runningFlag8_ChuanSongDai;//伸缩机传送带电机
	volatile motorStatus runningFlag9_WuGan;//无杆汽缸
	/*//加入了传感器之后不再需要这些表示位置的flag
	volatile positionStatus positionFlag0_TieBiZhuangZhi;//贴壁汽缸缩回为close
	volatile positionStatus positionFlag1_JuanYangJi;//delete
	volatile positionStatus positionFlag2_DangLiaoBanChuiZhi;//
	volatile positionStatus positionFlag3_TuiBanChuiZhi;
	volatile positionStatus positionFlag4_CeDangBan;
	volatile positionStatus positionFlag5_TuiLiaoShenSuoJi;
	volatile positionStatus positionFlag6_TuiBanTuiChu;//推板推出汽缸后位为close
	*/
	
	unsigned volatile short count;//当前已完成装载的物料数,4的整数倍
	volatile int height;//卸料装置的高度计数-6到7,共14层
	unsigned volatile char column;//整个机器所在列数
	unsigned volatile char localM;//当前所在装载的物料编号
	
} sysState;

#endif
