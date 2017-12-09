#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "defines.h"
#include "../threads/threads.h"
#include "../serial/serial.h"
#include "../encoder/encoder.h"

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
void reZero(sysState *flags);//调零

int main()
{
	SerialInit();
	motorsConfig(&motors);//电机配置
	Encoder1Init();
	Encoder2Init();
	reZero(&state);//调零,将卸料板调到与动力滚筒同一高度
	
	//以下为操作系统的初始化以及进程调度,主进程转到threads.c/thread_main()函数,进入后thread_LED进程点亮原点灯
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
	ms->motor0_TieBiZhuangZhi->id=0;
	ms->motor0_TieBiZhuangZhi->GPIOx=GPIOA;
	ms->motor0_TieBiZhuangZhi->GPIO_Pin_x1=GPIO_Pin_2;
	ms->motor0_TieBiZhuangZhi->GPIO_Pin_x2=GPIO_Pin_3;
	motorInit(ms->motor0_TieBiZhuangZhi);
	//其他电机的配置参考0号电机
	
#ifdef _DEBUG
	SerialPrintf("motor config\n");
#endif
}
void reZero(sysState *flags)
{
	//卸料板调零
	if(Encoder1GetTurn()!=0 || Encoder1GetCount()!=0)//卸料板当前不在零位时
	{
		if(Encoder1GetTurn()>=0)//当前位置高于0位时,卷扬机正转,卸料板下行;否则,反转
			motorTurn(motors.motor1_JuanYangJi,on);
		else motorTurn(motors.motor1_JuanYangJi,rev);
		while(1)
		{
			if(Encoder1GetTurn()==0 && Encoder1GetCount()==0)//到达零点时
				break;
		}
		//卸料板调零完毕
		Encoder1SetCount(0);//编码器计数归零
		Encoder1SetTurn(0);
		
		//flags初始化,flags归零
		flagInit(flags);
	}
#ifdef _DEBUG
	SerialPrintf("reZero\n");
#endif
}
void flagInit(sysState *flags)
{
	flags->flag_running=false;
	flags->runningFlag0_TieBiZhuangZhi=off;
	flags->runningFlag1_JuanYangJi=off;
	flags->runningFlag2_DangLiaoBanChuiZhi=off;
	flags->runningFlag3_TuiBanChuiZhi=off;
	flags->runningFlag4_CeDangBan=off;
	flags->runningFlag5_DangLiaoBanTuiChu=off;
	flags->runningFlag6_DaiDaoGan=off;
	flags->runningFlag7_DongLiGunTong=off;
	flags->runningFlagy=off;
	flags->positionFlag0_TieBiZhuangZhi=close;
	flags->positionFlag1_JuanYangJi=close;
	flags->positionFlag2_DangLiaoBanChuiZhi=close;
	flags->positionFlag3_TuiBanChuiZhi=close;
	flags->positionFlag4_CeDangBan=close;
	flags->positionFlag5_TuiLiaoShenSuoJi=close;
	flags->count=0;
	flags->column=0;
	flags->height=0;
	flags->localM=0;
#ifdef _DEBUG
	SerialPrintf("flags init\n");
#endif
}
