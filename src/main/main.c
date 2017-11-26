#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "defines.h"
#include "../threads/threads.h"
#include "../serial/serial.h"

//osThreadDef (task1, osPriorityNormal, 1, 0);
//osThreadId id1,id2;

osThreadDef(thread_main,osPriorityNormal, 1, 0);
osThreadDef(thread_serial_send,osPriorityNormal,1,(sizeof(sysState *)+30*sizeof(char)));
osThreadDef(thread_serial_receive,osPriorityNormal,1,5*sizeof(char));
osThreadDef(thread_LED,osPriorityNormal, 1, 2*sizeof(int));

int main()
{
	
	static sysState *state;
	static ids *id;
	SerialInit();
	if(osKernelInitialize ()==osOK)
		;//系统成功初始化时的代码

//添加thread	
//	id1 = osThreadCreate (osThread (task1), NULL); 
	id->id_main=osThreadCreate (osThread (thread_main), NULL);
	id->id_serial_send=osThreadCreate(osThread(thread_serial_send),state);
	id->id_serial_receive=osThreadCreate(osThread(thread_serial_receive),NULL);
	id->id_LED=osThreadCreate (osThread (thread_LED), NULL);

//启动	
	osKernelStart ();

	while(1);//正常情况下程序无法执行到这里
}
