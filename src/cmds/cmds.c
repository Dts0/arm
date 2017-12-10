#include "cmds.h"
#include "../main/defines.h"
#include "../threads/threads.h"

extern sysState state;
extern Motors motors;
extern IDs ids;
extern osMutexId  uart_mutex_id; // Mutex ID
extern char *printfBuf;
const char* cmds[]={"STOP","RST","CONT","RSD"};
//stop,reset,continue,resend
void cmdFunction0()
{
			if(state.flag_running==true) 
			{
				osMutexWait(uart_mutex_id, osWaitForever);
				os_serialPrintf("OK,stop\n");
				state.flag_running=false;
			}
			else os_serialPrintf("OK,but error\n");
}
void cmdFunction1()
{
	os_serialPrintf("OK,reset\n");
	NVIC_SystemReset();// 复位
}
void cmdFunction2()
{
	if(state.flag_running==false) 
		{
			state.flag_running=true;
			osMutexRelease(uart_mutex_id);
			os_serialPrintf("OK,continue\n");}
		else os_serialPrintf("OK,but error\n");
}
void cmdFunction3()
{
	os_serialPrintf(printfBuf);
}

void cmdNotFind()
{
	os_serialPrintf("ERR,unknown cmd\n");
}

