#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

typedef enum {
	off=0,//关闭
	on=1,//正转
	rev=2//反转
} motorStatus;

typedef struct
{
	int id;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin_x1;//控制电机的管脚
	uint16_t GPIO_Pin_x2;//
} motor;

void motorInit(motor *motorPointer);//指定电机的初始化
/**
*需要知道控制协议,然后补充
*当前设置pin1为RESET,pin2为SET正转,相反为反转,全为RESET为停止
**/
void motorTurn(motor *motorPointer,motorStatus status);//设置电机正转,反转或停止

#endif
