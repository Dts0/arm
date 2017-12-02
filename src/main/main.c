#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "defines.h"
#include "../threads/threads.h"
#include "../serial/serial.h"

//osThreadDef (task1, osPriorityNormal, 1, 0);
//osThreadId id1,id2;

osThreadDef(thread_main,osPriorityNormal, 1, 0);
osThreadDef(thread_serial_send,osPriorityNormal,1,8*((sizeof(sysState *)+30*sizeof(char))));//(sizeof(sysState *)+30*sizeof(char))
osThreadDef(thread_serial_receive,osPriorityNormal,1,200);//5*sizeof(char)
osThreadDef(thread_LED,osPriorityNormal, 1, 100);//2*sizeof(int)

osMutexDef (uart_mutex);    
osMutexId  uart_mutex_id; // Mutex ID

sysState state;
ids id;
	
int main()
{
	SerialInit();
	if(osKernelInitialize ()==osOK)
		;//系统成功初始化时的代码
#ifdef _DEBUG
	SerialPrintf("system init\n");
#endif
//添加thread	
//	id1 = osThreadCreate (osThread (task1), NULL); 
	id.id_main=osThreadCreate (osThread (thread_main), NULL);
#ifdef _DEBUG
	SerialPrintf("thread_main create\n");
#endif
	id.id_serial_send=osThreadCreate(osThread(thread_serial_send),&state);
#ifdef _DEBUG
	SerialPrintf("thread_serial_send create\n");
#endif
	id.id_serial_receive=osThreadCreate(osThread(thread_serial_receive),NULL);
#ifdef _DEBUG
	SerialPrintf("thread_serial_receive create\n");
#endif
	id.id_LED=osThreadCreate (osThread (thread_LED), NULL);
#ifdef _DEBUG
	SerialPrintf("thread_LED create\n");
#endif

#ifdef _DEBUG
	SerialPrintf("system start\n");
#endif
	uart_mutex_id = osMutexCreate(osMutex(uart_mutex));//进程锁创建
	state.flag_running=true;
//启动	
	osKernelStart ();

	while(1);//正常情况下程序无法执行到这里
}
