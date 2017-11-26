#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "cmsis_os.h"                  // ::CMSIS:RTOS2

#include <string.h>
#include <stdio.h>
#include "../threads/threads.h"
#include "../serial/serial.h"
#include "../main/defines.h"
#include "../encoder/encoder.h"

void thread_main(void *p)
{
	while(1);
}
void thread_serial_send(void *p)
{
	SerialEnableReponse(RESET);
	sysState *state=(sysState *)p;
	char msg[30];
	while(1)
	{
	sprintf(msg,"%s%d ",msg,state->flag0);
	sprintf(msg,"%s%d ",msg,state->flag1);
	sprintf(msg,"%s%d ",msg,state->flag2);
	sprintf(msg,"%s%u ",msg,state->flag3);
	sprintf(msg,"%s%d ",msg,state->flag4);
	sprintf(msg,"%s%d ",msg,state->flag5);
	sprintf(msg,"%s%u ",msg,state->num);
	sprintf(msg,"%s%u ",msg,state->height);
	sprintf(msg,"%s%u ",msg,state->column);
	sprintf(msg,"%s%u ",msg,state->localStep);
	sprintf(msg,"%s   ",msg);
	SerialPrintf(msg);
	sprintf(msg,"");//清空msg
	osDelay(1000);//每秒发送一次消息
	}
}
const char* cmds[]={"STOP","RST","CONT","RSD"};//////////////////////////////////////////
//stop,reset,continue,resend
void thread_serial_receive(void *p)
{
	char cmd[5];
	while(1)
	{
		SerialReceivedString(5);
	if(strcmp(cmd,cmds[0])==0) 
		{
			if(osKernelLock ()==1) 
				SerialPrintf("OK");
			else SerialPrintf("ERR");
		}
	else if(strcmp(cmd,cmds[1])==0) 
	{
		SerialPrintf("OK");
		NVIC_SystemReset();// 复位
	}
	else if(strcmp(cmd,cmds[2])==0) 
	{
		if(osKernelLock ()==0) 
				SerialPrintf("OK");
		else SerialPrintf("ERR");
	}
	else SerialPrintf("ERR,unknown cmd");;
		sprintf(cmd,"");
	}
}
void thread_LED(void *p)
{
	int count=0;
	//int deg=0;
	int turn=0;
	
	while(1)
	{
		count=Encoder1GetCount();
		turn=Encoder1GetTurn();
		GPIO_SetBits(GPIOC,GPIO_Pin_All);
		if((turn==-1 && count>395)||(turn==0 && count <=5))
			GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		else if(turn==0 && count>5 && count<=15)
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		else if(turn==-1 && count>385 && count<=395)
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);
		else if(turn==0 && count>15 && count<=25)
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
		else if(turn==-1 && count>375 && count<=385)
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		else if((turn>0)||(turn==0 && count>25))
			GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		else if((turn<-1)||(turn==-1 && count<=375))
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
	}
}

