#ifndef __ENCODE_H__
#define __ENCODE_H__
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

//配置时需要在stm32f10x_it.h和.c中配置中断函数
//根据TIM配置A和B;Z相随意
//TIM2 
//TIM3
//TIM4 ch1,2,3,4=PB6,7,8,9
//TIM5 ch1,2,3,4=PA0,1,2,3

void Encoder1Init(void);
void Encoder1LEDInit(void);
uint32_t Encoder1GetCount(void);//获得计数数
uint32_t Encoder1GetDeg(void);//获得当前位置的度数,保留到整数
int Encoder1GetTurn(void);//获得圈数
void Encoder1SetTurn(int turn);//设置圈数
void Encoder1SetCount(int count);//设置计数
int Encoder1GetDistance(int d);//获得距离


void Encoder2Init(void);
uint32_t Encoder2GetCount(void);//获得计数数
uint32_t Encoder2GetDeg(void);//获得当前位置的度数,保留到整数
int Encoder2GetTurn(void);//获得圈数
int Encoder2GetDistance(int d);//获得距离


#endif
