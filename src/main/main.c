#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "defines.h"
#include "../threads/threads.h"
#include "../serial/serial.h"

//osThreadDef (task1, osPriorityNormal, 1, 0);
//osThreadId id1,id2;
os_pthread thread_main0=(os_pthread)thread_main;
os_pthread thread_serial_send0=(os_pthread)thread_serial_send;
os_pthread thread_serial_receive0=(os_pthread)thread_serial_receive;
os_pthread thread_LED0=(os_pthread)thread_LED;


osMutexDef (uart_mutex);    
osMutexId  uart_mutex_id; // Mutex ID

sysState state;
IDs ids;
Motors motors;
	
void motorsConfig(Motors *ms);
void flagInit(sysState *flags);

int main()
{
	flagInit(&state);
	SerialInit();
	motorsConfig(&motors);//电机配置
	if(osKernelInitialize ()==osOK)
		;//系统成功初始化时的代码
#ifdef _DEBUG
	SerialPrintf("system init\n");
#endif
//添加thread	
//	id1 = osThreadCreate (osThread (task1), NULL); 
	osThreadDef(thread_main0,osPriorityNormal, 1, 1000);
	ids.id_main=osThreadCreate (osThread (thread_main0), NULL);
#ifdef _DEBUG
	SerialPrintf("thread_main create\n");
#endif
	osThreadDef(thread_serial_send0,osPriorityNormal,1,8*((sizeof(sysState *)+30*sizeof(char))));//(sizeof(sysState *)+30*sizeof(char))
	ids.id_serial_send=osThreadCreate(osThread(thread_serial_send0),&state);
#ifdef _DEBUG
	SerialPrintf("thread_serial_send create\n");
#endif
	osThreadDef(thread_serial_receive0,osPriorityNormal,1,200);//5*sizeof(char)
	ids.id_serial_receive=osThreadCreate(osThread(thread_serial_receive0),NULL);
#ifdef _DEBUG
	SerialPrintf("thread_serial_receive create\n");
#endif
	osThreadDef(thread_LED0,osPriorityNormal, 1, 100);//2*sizeof(int)
	ids.id_LED=osThreadCreate (osThread (thread_LED0), NULL);
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

void motorsConfig(Motors *ms)
{
	ms->motor0->id=0;
	ms->motor0->GPIOx=GPIOA;
	ms->motor0->GPIO_Pin_x1=GPIO_Pin_2;
	ms->motor0->GPIO_Pin_x2=GPIO_Pin_3;
	motorInit(ms->motor0);
	//其他电机的配置参考0号电机
	
	#ifdef _DEBUG
	SerialPrintf("motor config\n");
#endif
}
void flagInit(sysState *flags)
{
	
	#ifdef _DEBUG
	SerialPrintf("flags init\n");
#endif
}
