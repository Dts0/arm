#include "cmds.h"
#include "../main/defines.h"
#include "../threads/threads.h"
#include "../config/config.h"
#include "../encoder/encoder.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);//断开
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
			GPIO_SetBits(GPIOA,GPIO_Pin_5);//闭合
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
	
	char cmd2[3],cmd3[19];
	cmd2[0]=cmd[4];
	cmd2[1]=cmd[5];
	cmd2[2]='\0';
	int i=7;
	while(cmd[i]!='\0')
	{
		cmd3[i-7]=cmd[i];
		i++;
	}
	cmd3[i]='\0';
	if(strcmp(cmd2,"01")==0)
	{
		if(atoi(cmd3)==1)
		{//os_serialPrintf("set 1\n");
		SetHasEnteredBox(true);
		}
		else 
		{//os_serialPrintf("set 0\n");
			SetHasEnteredBox(false);
		}
		os_serialPrintf("SET 01\n");
	}
	else if(strcmp(cmd2,"02")==0)
	{
		if(atoi(cmd3)==1)
		SetHasItems(true);
		else SetHasItems(false);
		os_serialPrintf("SET 02\n");
	}
	else if(strcmp(cmd2,"03")==0)
	{
		if(atoi(cmd3)==1)
		SetHasArrivedAtExtremePosition(true);
		else SetHasArrivedAtExtremePosition(false);
		os_serialPrintf("SET 03\n");
	}
	else if(strcmp(cmd2,"04")==0)
	{
		if(atoi(cmd3)==1)
		SetHasArrivedAtB(true);
		else SetHasArrivedAtB(false);
		os_serialPrintf("SET 04\n");
	}
	else if(strcmp(cmd2,"05")==0)
	{
		if(atoi(cmd3)==1)
		SetHasArrivedAtC(true);
		else SetHasArrivedAtC(false);
		os_serialPrintf("SET 05\n");
	}
	else if(strcmp(cmd2,"06")==0)
	{
		if(atoi(cmd3)==1)
		SetHasArrivedAtD(true);
		else SetHasArrivedAtD(false);
		os_serialPrintf("SET 06\n");
	}
	else if(strcmp(cmd2,"07")==0)
	{
		if(atoi(cmd3)==1)
		SetHasArrivedAtE(true);
		else SetHasArrivedAtE(false);
		os_serialPrintf("SET 07\n");
	}
	else if(strcmp(cmd2,"13")==0)
	{
		if(atoi(cmd3)==1)
		setTieBiPosition(true);
		else setTieBiPosition(false);
		os_serialPrintf("SET 13\n");
	}
	else if(strcmp(cmd2,"14")==0)
	{
		if(atoi(cmd3)==1)
		setDangLiaoBanChuiZhiPosition(true);
		else setDangLiaoBanChuiZhiPosition(false);
		os_serialPrintf("SET 14\n");
	}
	else if(strcmp(cmd2,"15")==0)
	{
		if(atoi(cmd3)==1)
		setTuiBanPosition(true);
		else setTuiBanPosition(false);
		os_serialPrintf("SET 15\n");
	}
	else if(strcmp(cmd2,"16")==0)
	{
		if(atoi(cmd3)==1)
		setCeDangBanPosition(true);
		else setCeDangBanPosition(false);
		os_serialPrintf("SET 16\n");
	}
	else if(strcmp(cmd2,"17")==0)
	{
		if(atoi(cmd3)==1)
		setDangLiaoBanTuiChuPosition(true);
		else setDangLiaoBanTuiChuPosition(false);
		os_serialPrintf("SET 17\n");
	}
	else if(strcmp(cmd2,"18")==0)
	{
		if(atoi(cmd3)==1)
		setTuiBanTuiChuPosition(true);
		else setTuiBanTuiChuPosition(false);
		os_serialPrintf("SET 18\n");
	}
	else if(strcmp(cmd2,"E1")==0)
	{
		/*
		char c[10]="";
		sprintf(c,"%d\n",atoi(cmd3));
		os_serialPrintf(c);
		*/
		Encoder1SetDistance(atoi(cmd3),D_JuanYangJi);
		os_serialPrintf("OK,SET E1\n");
	}
	else if(strcmp(cmd2,"E2")==0)
	{
				
		int val=atoi(cmd3);
		char c[10]="";
		sprintf(c,"%d\n",val);
		os_serialPrintf(c);

		Encoder2SetTurn(val);
		os_serialPrintf("OK,SET E2 turn\n");
	}
	else if(strcmp(cmd2,"C2")==0)
	{
		int val=atoi(cmd3);
		char c[10]="";
		sprintf(c,"%d\n",val);
		os_serialPrintf(c);
		
		Encoder2SetCount(val);
		os_serialPrintf("OK,SET E2 count\n");
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

