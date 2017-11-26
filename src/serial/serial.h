#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void SerialPrintf(char *str);
void SerialInit(void);

char SerialReceivedChar(void);
char* SerialReceivedString(unsigned int maxChar);//从此时起到第一个'\0'或达到最大值
//注:本函数在达到结束条件前不会返回

void SerialEnableReponse(FlagStatus isEnable);//定义是否响应(按原样返回输入的值)

void USART1_IRQHandler(void);

#endif
