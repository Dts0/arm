#include "cmds.h"
#include "../main/defines.h"
#include "../threads/threads.h"
#include "../config/config.h"
#include <string.h>


extern sysState state;
extern Motors motors;
extern IDs ids;
extern osMutexId  uart_mutex_id; // Mutex ID
extern char *printfBuf;
const char* cmds[]={"STOP","RST","CONT","RSD","SET"};
//stop,reset,continue,resend,set
void cmd0_STOP()
{
			if(state.flag_running==true) 
			{
				osMutexWait(uart_mutex_id, osWaitForever);
				os_serialPrintf("OK,stop \n");
				state.flag_running=false;
			}
			else os_serialPrintf("OK,but error \n");
}
void cmd1_RST()
{
	os_serialPrintf("OK,reset \n");
	NVIC_SystemReset();// 复位
}
void cmd2_CONT()
{
	if(state.flag_running==false) 
		{
			state.flag_running=true;
			osMutexRelease(uart_mutex_id);
			os_serialPrintf("OK,continue \n");}
		else os_serialPrintf("OK,but error \n");
}
void cmd3_RSD()
{
	os_serialPrintf(printfBuf);
}
void cmd4_SET(char *cmd)
{
	#ifdef _DEBUG
	
	char cmd2[3],cmd3[2];
	cmd2[0]=cmd[4];
	cmd2[1]=cmd[5];
	cmd2[2]='\0';
	cmd3[0]=cmd[7];
	cmd3[1]='\0';

	if(strcmp(cmd2,"01")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasEnteredBox(true);
		else SetHasEnteredBox(false);
	}
	else if(strcmp(cmd2,"02")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasItems(true);
		else SetHasItems(false);
	}
	else if(strcmp(cmd2,"03")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasArrivedAtExtremePosition(true);
		else SetHasArrivedAtExtremePosition(false);
	}
	else if(strcmp(cmd2,"04")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasArrivedAtB(true);
		else SetHasArrivedAtB(false);
	}
	else if(strcmp(cmd2,"05")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasArrivedAtC(true);
		else SetHasArrivedAtC(false);
	}
	else if(strcmp(cmd2,"06")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasArrivedAtD(true);
		else SetHasArrivedAtD(false);
	}
	else if(strcmp(cmd2,"07")==0)
	{
		if(strcmp(cmd3,"1")==0)
		SetHasArrivedAtE(true);
		else SetHasArrivedAtE(false);
	}
	else if(strcmp(cmd2,"13")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setTieBiPosition(true);
		else setTieBiPosition(false);
	}
	else if(strcmp(cmd2,"14")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setDangLiaoBanChuiZhiPosition(true);
		else setDangLiaoBanChuiZhiPosition(false);
	}
	else if(strcmp(cmd2,"15")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setTuiBanPosition(true);
		else setTuiBanPosition(false);
	}
	else if(strcmp(cmd2,"16")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setCeDangBanPosition(true);
		else setCeDangBanPosition(false);
	}
	else if(strcmp(cmd2,"17")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setDangLiaoBanTuiChuPosition(true);
		else setDangLiaoBanTuiChuPosition(false);
	}
	else if(strcmp(cmd2,"18")==0)
	{
		if(strcmp(cmd3,"1")==0)
		setTuiBanTuiChuPosition(true);
		else setTuiBanTuiChuPosition(false);
	}
	else os_serialPrintf("ERR,unknown args \n");

	#else
	os_serialPrintf("ERR,you didn't define _DEBUG \n");
	#endif
}

void cmdNotFind()
{
	os_serialPrintf("ERR,unknown cmd \n");
}

