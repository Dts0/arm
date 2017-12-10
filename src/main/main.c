#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "defines.h"
#include "../threads/threads.h"
#include "../serial/serial.h"
#include "../encoder/encoder.h"
#include "../config/config.h"

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
	
void flagInit(sysState *flags);
void motorsConfig(Motors *ms);//电机配置函数声明
void reZero(sysState *flags);//调零

int main()
{
	SerialInit();//串口初始化
	motorsConfig(&motors);//电机配置
	Encoder1Init();
	Encoder2Init();
	reZero(&state);//调零,将卸料板调到与动力滚筒同一高度
	flagInit(&state);//state的内容初始化为零点
	
	//以下为操作系统的初始化以及进程调度,主进程转到threads.c/thread_main()函数,进入后thread_LED进程点亮原点灯
	if(osKernelInitialize ()==osOK)
		;//系统成功初始化时的代码
#ifdef _DEBUG
	SerialPrintf("操作系统初始化...\n");
#endif
//添加thread	
//	id1 = osThreadCreate (osThread (task1), NULL); 
	osThreadDef(thread_main0,osPriorityNormal, 1, 1000);
	ids.id_main=osThreadCreate (osThread (thread_main0), NULL);
#ifdef _DEBUG
	SerialPrintf("线程thread_main创建\n");
#endif
	osThreadDef(thread_serial_send0,osPriorityNormal,1,8*((sizeof(sysState *)+50*sizeof(char))));//(sizeof(sysState *)+30*sizeof(char))
	ids.id_serial_send=osThreadCreate(osThread(thread_serial_send0),&state);
#ifdef _DEBUG
	SerialPrintf("线程thread_serial_send创建\n");
#endif
	osThreadDef(thread_serial_receive0,osPriorityNormal,1,200);//5*sizeof(char)
	ids.id_serial_receive=osThreadCreate(osThread(thread_serial_receive0),NULL);
#ifdef _DEBUG
	SerialPrintf("线程thread_serial_receive创建\n");
#endif
	osThreadDef(thread_LED0,osPriorityNormal, 1, 100);//2*sizeof(int)
	ids.id_LED=osThreadCreate (osThread (thread_LED0), NULL);
#ifdef _DEBUG
	SerialPrintf("线程thread_LED创建\n");
#endif

	uart_mutex_id = osMutexCreate(osMutex(uart_mutex));//进程锁创建
	state.flag_running=true;
//启动	
#ifdef _DEBUG
	SerialPrintf("操作系统启动,开始任务调度\n");
#endif
	osKernelStart ();

	while(1);//正常情况下程序无法执行到这里
}

void motorsConfig(Motors *ms)//电机配置
{
	ms->motory->id=-1;
	ms->motory->GPIOx=GPIOF;
	ms->motory->GPIO_Pin_x1=GPIO_Pin_0;
	ms->motory->GPIO_Pin_x2=GPIO_Pin_1;
	ms->motory->stopFunc=NULL;
	motorInit(ms->motory);
	
	ms->motor0_TieBiZhuangZhi->id=0;
	ms->motor0_TieBiZhuangZhi->GPIOx=GPIOF;
	ms->motor0_TieBiZhuangZhi->GPIO_Pin_x1=GPIO_Pin_2;
	ms->motor0_TieBiZhuangZhi->GPIO_Pin_x2=GPIO_Pin_3;
	ms->motor0_TieBiZhuangZhi->stopFunc=motor0_TieBi_stopFunc;
	motorInit(ms->motor0_TieBiZhuangZhi);
	
	ms->motor1_JuanYangJi->id=1;
	ms->motor1_JuanYangJi->GPIOx=GPIOF;
	ms->motor1_JuanYangJi->GPIO_Pin_x1=GPIO_Pin_4;
	ms->motor1_JuanYangJi->GPIO_Pin_x2=GPIO_Pin_5;
	ms->motor1_JuanYangJi->stopFunc=NULL;
	motorInit(ms->motor1_JuanYangJi);
	
	ms->motor2_DangLiaoBanChuiZhi->id=2;
	ms->motor2_DangLiaoBanChuiZhi->GPIOx=GPIOF;
	ms->motor2_DangLiaoBanChuiZhi->GPIO_Pin_x1=GPIO_Pin_6;
	ms->motor2_DangLiaoBanChuiZhi->GPIO_Pin_x2=GPIO_Pin_7;
	ms->motor2_DangLiaoBanChuiZhi->stopFunc=NULL;
	motorInit(ms->motor2_DangLiaoBanChuiZhi);
		
	ms->motor3_TuiBanChuiZhi->id=3;
	ms->motor3_TuiBanChuiZhi->GPIOx=GPIOF;
	ms->motor3_TuiBanChuiZhi->GPIO_Pin_x1=GPIO_Pin_8;
	ms->motor3_TuiBanChuiZhi->GPIO_Pin_x2=GPIO_Pin_9;
	ms->motor3_TuiBanChuiZhi->stopFunc=NULL;
	motorInit(ms->motor3_TuiBanChuiZhi);
	
	ms->motor4_CeDangBan->id=4;
	ms->motor4_CeDangBan->GPIOx=GPIOF;
	ms->motor4_CeDangBan->GPIO_Pin_x1=GPIO_Pin_10;
	ms->motor4_CeDangBan->GPIO_Pin_x2=GPIO_Pin_11;
	ms->motor4_CeDangBan->stopFunc=motor4_CeDangBan_stopFunc;
	motorInit(ms->motor4_CeDangBan);
	
	ms->motor5_DangLiaoBanTuiChu->id=5;
	ms->motor5_DangLiaoBanTuiChu->GPIOx=GPIOF;
	ms->motor5_DangLiaoBanTuiChu->GPIO_Pin_x1=GPIO_Pin_12;
	ms->motor5_DangLiaoBanTuiChu->GPIO_Pin_x2=GPIO_Pin_13;
	ms->motor5_DangLiaoBanTuiChu->stopFunc=NULL;
	motorInit(ms->motor5_DangLiaoBanTuiChu);
	
	ms->motor6_DaiDaoGan->id=6;
	ms->motor6_DaiDaoGan->GPIOx=GPIOF;
	ms->motor6_DaiDaoGan->GPIO_Pin_x1=GPIO_Pin_14;
	ms->motor6_DaiDaoGan->GPIO_Pin_x2=GPIO_Pin_15;
	ms->motor6_DaiDaoGan->stopFunc=NULL;
	motorInit(ms->motor6_DaiDaoGan);
	
	ms->motor7_DongLiGunTong->id=7;
	ms->motor7_DongLiGunTong->GPIOx=GPIOG;
	ms->motor7_DongLiGunTong->GPIO_Pin_x1=GPIO_Pin_2;
	ms->motor7_DongLiGunTong->GPIO_Pin_x2=GPIO_Pin_3;
	ms->motor7_DongLiGunTong->stopFunc=NULL;
	motorInit(ms->motor7_DongLiGunTong);
	
	ms->motor8_ChuanSongDai->id=8;
	ms->motor8_ChuanSongDai->GPIOx=GPIOG;
	ms->motor8_ChuanSongDai->GPIO_Pin_x1=GPIO_Pin_4;
	ms->motor8_ChuanSongDai->GPIO_Pin_x2=GPIO_Pin_5;
	ms->motor8_ChuanSongDai->stopFunc=NULL;
	motorInit(ms->motor8_ChuanSongDai);
	
	ms->motor9_WuGan->id=9;
	ms->motor9_WuGan->GPIOx=GPIOG;
	ms->motor9_WuGan->GPIO_Pin_x1=GPIO_Pin_6;
	ms->motor9_WuGan->GPIO_Pin_x2=GPIO_Pin_7;
	ms->motor9_WuGan->stopFunc=NULL;
	motorInit(ms->motor9_WuGan);
	
#ifdef _DEBUG
	SerialPrintf("电机配置完成\n");
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
		#ifdef _DEBUG
			SerialPrintf("卸料板调零完成\n");
		#endif
	}
	else 
	{
		#ifdef _DEBUG
			SerialPrintf("卸料板当前处于零位,无需调零\n");
		#endif
	}
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
	flags->runningFlag8_ChuanSongDai=off;
	flags->runningFlag9_WuGan=off;
	flags->runningFlagy=off;
	/*
	flags->positionFlag0_TieBiZhuangZhi=close;
	flags->positionFlag1_JuanYangJi=close;
	flags->positionFlag2_DangLiaoBanChuiZhi=close;
	flags->positionFlag3_TuiBanChuiZhi=close;
	flags->positionFlag4_CeDangBan=close;
	flags->positionFlag5_TuiLiaoShenSuoJi=close;
	*/
	flags->count=0;
	flags->column=0;
	flags->height=0;
	flags->localM=0;
#ifdef _DEBUG
	SerialPrintf("state内容初始化\n");
#endif
}
