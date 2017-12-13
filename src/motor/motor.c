#include "motor.h"
#include "../config/config.h"

#ifdef _DEBUG
#include "../threads/threads.h"
#include <stdio.h>
#include <string.h>
#endif

void motorInit(motor *motorPointer)
{
	_CLK_ENABLE_GPIO(motorPointer->GPIOx);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=motorPointer->GPIO_Pin_x1|motorPointer->GPIO_Pin_x2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(motorPointer->GPIOx, &GPIO_InitStructure);
	
	GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1|motorPointer->GPIO_Pin_x2);
}

void motorTurn(motor *motorPointer,motorStatus status)
{
	if(status == off)//关
	{
		GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1|motorPointer->GPIO_Pin_x2);
		_setStateRunFlag(motorPointer->id,status);
#ifdef _DEBUG
		char s[10]="";
		sprintf(s,"%smotor%d 停止\n",s,motorPointer->id);
		os_serialPrintf(s);
#endif
	} 
	else if(status == on)//开
	{
		GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1);
		GPIO_SetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x2);
		_setStateRunFlag(motorPointer->id,status);
#ifdef _DEBUG
		char s[10]="";
		sprintf(s,"%smotor%d 正转\n",s,motorPointer->id);
		os_serialPrintf(s);
#endif
	} 
	else if(status == rev)//反转
	{
		GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x2);
		GPIO_SetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1);
		_setStateRunFlag(motorPointer->id,status);
#ifdef _DEBUG
		char s[10]="";
		sprintf(s,"%smotor%d 反转\n",s,motorPointer->id);
		os_serialPrintf(s);
#endif
	}
	else ;
}


#include "../main/defines.h"
extern sysState state;
void _setStateRunFlag(int id,motorStatus flag)
{
	switch(id)
	{
		case 0:state.runningFlag0_TieBiZhuangZhi=flag;break;
		case 1:state.runningFlag1_JuanYangJi=flag;break;
		case 2:state.runningFlag2_DangLiaoBanChuiZhi=flag;break;
		case 3:state.runningFlag3_TuiBanChuiZhi=flag;break;
		case 4:state.runningFlag4_CeDangBan=flag;break;
		case 5:state.runningFlag5_DangLiaoBanTuiChu=flag;break;
		case 6:state.runningFlag6_TuiBanTuiChu=flag;break;
		case 7:state.runningFlag7_DongLiGunTong=flag;break;
		case 8:state.runningFlag8_ChuanSongDai=flag;break;
		case 9:state.runningFlag9_WuGan=flag;break;
		case -1:state.runningFlagy=flag;break;
		default:;
	}
}
void _CLK_ENABLE_GPIO(GPIO_TypeDef* GPIOx)
{
	switch((uint32_t)GPIOx)
	{
		case (uint32_t)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);break;
		case (uint32_t)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);break;
		case (uint32_t)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);break;
		case (uint32_t)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);break;
		case (uint32_t)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);break;
		case (uint32_t)GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);break;
		case (uint32_t)GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);break;
//		deafult:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);break;
	}
}


