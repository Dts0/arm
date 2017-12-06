#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "cmsis_os.h"                  // ::CMSIS:RTOS2

#include <string.h>
#include <stdio.h>
#include "../threads/threads.h"
#include "../serial/serial.h"
#include "../main/defines.h"
#include "../encoder/encoder.h"

extern sysState state;
extern Motors motors;
extern IDs ids;
extern osMutexId  uart_mutex_id; // Mutex ID

char *printfBuf;

void os_serialPrintf(char *str);//加了进程锁的串口输出,防止在传数据过程中被打断
char *os_serialReceivedString(unsigned int num);
void setStateRunFlag(int id,motorStatus flag);
void setStatePosFlag(int id,postionStatus flag);


void thread_main(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_main start\n");
#endif
	
//step1://step1:进入箱体
	
	//驱动全向轮移动
	motorTurn(motors.motory,on);
	
	//贴壁装置打开
	os_pthread thread_motor0_TieBiZhuangZhi=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor0_TieBiZhuangZhi,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor0_TieBiZhuangZhi=osThreadCreate(osThread(thread_motor0_TieBiZhuangZhi),motors.motor0_TieBiZhuangZhi);
	//卷扬机启动
	os_pthread thread_motor1_JuanYangJi=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor1_JuanYangJi,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor1_JuanYangJi=osThreadCreate(osThread(thread_motor1_JuanYangJi),motors.motor1_JuanYangJi);
	//挡料板抬起
	os_pthread thread_motor2_DangLiaoBan=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor2_DangLiaoBan,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor2_DangLiaoBan=osThreadCreate(osThread(thread_motor2_DangLiaoBan),motors.motor2_DangLiaoBan);
	//驱动挡料板
	os_pthread thread_motor3_TuiBan=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor3_TuiBan,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor3_TuiBan=osThreadCreate(osThread(thread_motor3_TuiBan),motors.motor3_TuiBan);
	
	osDelay(8000);//假设8秒到最里
	motorTurn(motors.motory,off);
	
	//step2:装料循环
	step2:
	//侧挡板向外推出
	motorTurn(motors.motor4_CeDangBan,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行1秒为从右到中
	osDelay(1000);
	motorTurn(motors.motor4_CeDangBan,off);
//step2_1://物料1
	//伸缩机推出以及返回
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行2秒为伸缩机推料所需时间
	osDelay(2000);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,off);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,rev);
	osDelay(2000);
	
	//推板
	motorTurn(motors.motor3_TuiBan,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行1秒为从右到中
	osDelay(2000);
	motorTurn(motors.motor3_TuiBan,off);
	motorTurn(motors.motor3_TuiBan,rev);
	osDelay(2000);
//step2_2://物料2
	//伸缩机推出以及返回
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行2秒为伸缩机推料所需时间
	osDelay(2000);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,off);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,rev);
	osDelay(2000);
	
	//推板
	motorTurn(motors.motor3_TuiBan,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行1秒为从右到中
	osDelay(1000);
	motorTurn(motors.motor3_TuiBan,off);
	
//step2_3://物料3
	//伸缩机推出以及返回
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行2秒为伸缩机推料所需时间
	osDelay(2000);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,off);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,rev);
	osDelay(2000);
	
	//推板将物料3推至最右侧
	motorTurn(motors.motor3_TuiBan,rev);
	osDelay(1000);
	
//step2_4://物料4
	//伸缩机推出以及返回
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,on);
	//注意:这里应当修改判断条件判断是否已经到达条件,此处假定运行2秒为伸缩机推料所需时间
	osDelay(2000);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,off);
	motorTurn(motors.motor5_TuiLiaoShenSuoJi,rev);
	osDelay(2000);
	
//step3://判断是否码够N层,不够时返回step2,码下一层
	if(state.height<MaxHeight)
	{
		state.height++;
		goto step2;
	}
//step4://判断是否码够M列,不够时返回step3,码下一列(即判断是否码完整个集装箱)
	//松开贴壁装置
		motorTurn(motors.motor0_TieBiZhuangZhi,rev);
		osDelay(1000);
		motorTurn(motors.motor0_TieBiZhuangZhi,off);
	if(state.column<MaxColumn)
	{
		//机器在y方向后退一个单位(向集装箱外移动一个单位)
		motorTurn(motors.motory,rev);
		osDelay(1000);
		motorTurn(motors.motory,off);
		
		state.column++;
		goto step2;
	}
	
	while(1);//整个集装箱码完,待命
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
	sprintf(msg,"%s%d ",msg,state->runningFlagy);
	sprintf(msg,"%s%d ",msg,state->runningFlag0_TieBiZhuangZhi);
	sprintf(msg,"%s%d ",msg,state->runningFlag1_JuanYangJi);
	sprintf(msg,"%s%d ",msg,state->runningFlag2_DangLiaoBan);
	sprintf(msg,"%s%u ",msg,state->runningFlag3_TuiBan);
	sprintf(msg,"%s%d ",msg,state->runningFlag4_CeDangBan);
	sprintf(msg,"%s%d ",msg,state->runningFlag5_TuiLiaoShenSuoJi);
	sprintf(msg,"%s%d ",msg,state->postionFlag0_TieBiZhuangZhi);
	sprintf(msg,"%s%d ",msg,state->postionFlag1_JuanYangJi);
	sprintf(msg,"%s%d ",msg,state->postionFlag2_DangLiaoBan);
	sprintf(msg,"%s%u ",msg,state->postionFlag3_TuiBan);
	sprintf(msg,"%s%d ",msg,state->postionFlag4_CeDangBan);
	sprintf(msg,"%s%d ",msg,state->postionFlag5_TuiLiaoShenSuoJi);
	sprintf(msg,"%s%u ",msg,state->num);
	sprintf(msg,"%s%d ",msg,state->height);
	sprintf(msg,"%s%u ",msg,state->column);
	sprintf(msg,"%s%u ",msg,state->localStep);
	sprintf(msg,"%s   ",msg);
	os_serialPrintf(msg);
	sprintf(msg,"");//清空msg
	osDelay(_SEND_DELAY_TIME);//每秒发送一次消息
	}
}

extern const char* cmds[];
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
	if(motorPointer->id<=0) osThreadExit();
	
	motorTurn(motorPointer,on);
	setStateRunFlag(motorPointer->id,on);
#ifdef _DEBUG
	char *s;
	sprintf(s,"motor%d turn on\n",motorPointer->id);
	SerialPrintf(s);
#endif
	//执行motor中的stopFunc函数来等待停止,如果没有定义stopFunc,执行5秒后停止
	if(!motorPointer->stopFunc)
	motorPointer->stopFunc();
	else osDelay(5000);
	
	setStateRunFlag(motorPointer->id,off);
	setStatePosFlag(motorPointer->id,open);
#ifdef _DEBUG
	sprintf(s,"motor%d turn off,postion open\n",motorPointer->id);
	SerialPrintf(s);
#endif
		osThreadExit();
}


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

void setStateRunFlag(int id,motorStatus flag)
{
	switch(id)
	{
		case 0:state.runningFlag0_TieBiZhuangZhi=flag;break;
		case 1:state.runningFlag1_JuanYangJi=flag;break;
		case 2:state.runningFlag2_DangLiaoBan=flag;break;
		case 3:state.runningFlag3_TuiBan=flag;break;
		case 4:state.runningFlag4_CeDangBan=flag;break;
		case 5:state.runningFlag5_TuiLiaoShenSuoJi=flag;break;
		default:;
	}
}
void setStatePosFlag(int id,postionStatus flag)
{
	switch(id)
	{
		case 0:state.postionFlag0_TieBiZhuangZhi=flag;break;
		case 1:state.postionFlag1_JuanYangJi=flag;break;
		case 2:state.postionFlag2_DangLiaoBan=flag;break;
		case 3:state.postionFlag3_TuiBan=flag;break;
		case 4:state.postionFlag4_CeDangBan=flag;break;
		case 5:state.postionFlag5_TuiLiaoShenSuoJi=flag;break;
		default:;
	}
}

