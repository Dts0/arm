#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "cmsis_os.h"                  // ::CMSIS:RTOS2

#include <string.h>
#include <stdio.h>
#include "../threads/threads.h"
#include "../serial/serial.h"
#include "../main/defines.h"
#include "../encoder/encoder.h"
   
extern osMutexId  uart_mutex_id; // Mutex ID
extern sysState state;
char *printfBuf;

void os_serialPrintf(char *str)//加了进程锁的串口输出,防止在传数据过程中被打断
{
	osMutexWait(uart_mutex_id, osWaitForever);
	printfBuf=str;
	SerialPrintf(str);
	osMutexRelease(uart_mutex_id);
}
char *os_serialReceivedString(unsigned int num)
{
	char *s;
	//osMutexWait(uart_mutex_id, osWaitForever);
	s=SerialReceivedString(num);
	//osMutexRelease(uart_mutex_id);
	return s;
}

void thread_main(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_main start\n");
#endif
	while(1);
}
void thread_serial_send(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_serial_send start\n");
#endif
	SerialEnableReponse(RESET);
	sysState *state=(sysState *)p;
	char msg[30];
	while(1)
	{
	sprintf(msg,"%s%d ",msg,state->flag_running);
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
	os_serialPrintf(msg);
	sprintf(msg,"");//清空msg
	osDelay(1000);//每秒发送一次消息
	}
}
const char* cmds[]={"STOP","RST","CONT","RSD"};//////////////////////////////////////////
//stop,reset,continue,resend
void thread_serial_receive(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_serial_receive start\n");
#endif
	char *cmd;
	while(1)
	{
		cmd = os_serialReceivedString(5);
	if(strcmp(cmd,cmds[0])==0) 
		{
			if(state.flag_running==true) 
			{
				osMutexWait(uart_mutex_id, osWaitForever);
				os_serialPrintf("OK,stop\n");
				state.flag_running=false;
			}
			else os_serialPrintf("OK,but error\n");
		}
	else if(strcmp(cmd,cmds[1])==0) 
	{
		os_serialPrintf("OK,reset\n");
		NVIC_SystemReset();// 复位
	}
	else if(strcmp(cmd,cmds[2])==0) 
	{
		if(state.flag_running==false) 
		{
			state.flag_running=true;
			osMutexRelease(uart_mutex_id);
			os_serialPrintf("OK,continue\n");}
		else os_serialPrintf("OK,but error\n");
	}
	else if(strcmp(cmd,cmds[3])==0)
	{
		os_serialPrintf(printfBuf);
	} else os_serialPrintf("ERR,unknown cmd\n");
		sprintf(cmd,"");
	}
}
void thread_LED(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_LED start\n");
#endif
	
	Encoder1LEDInit();
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

void thread_turnOnMotor(void *p)
{
	motor *motorPointer=(motor *)p;
	motorTurn(motorPointer,on);
	switch(motorPointer->id)
	{
		case 0:state.flag0=on;break;
		case 1:state.flag1=on;break;
		case 2:state.flag2=on;break;
		case 3:state.flag3=on;break;
		case 4:state.flag4=on;break;
		case 5:state.flag5=on;break;
		default:;
	}
	while(1)
	{
		
		break;
	}
	switch(motorPointer->id)
	{
		case 0:state.flag0=off;break;
		case 1:state.flag1=off;break;
		case 2:state.flag2=off;break;
		case 3:state.flag3=off;break;
		case 4:state.flag4=off;break;
		case 5:state.flag5=off;break;
		default:;
	}
	osThreadExit();
}
