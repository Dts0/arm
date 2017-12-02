#include "motor.h"


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


void motorInit(motor *motorPointer)
{
	if(motorPointer->id == -1) return;
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
	} 
	else if(status == on)//开
	{
		GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1);
		GPIO_SetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x2);
	} 
	else if(status == rev)//反转
	{
		GPIO_ResetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x2);
		GPIO_SetBits(motorPointer->GPIOx,motorPointer->GPIO_Pin_x1);
	}
	else ;
}
