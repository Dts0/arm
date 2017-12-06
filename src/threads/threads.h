#ifndef __THREADS_H
#define  __THREADS_H

#include "cmsis_os.h" 
#include "../main/defines.h"


typedef struct
{
	osThreadId id_main;
	osThreadId id_serial_send;
	osThreadId id_serial_receive;
	osThreadId id_LED;
	osThreadId id_motor0_TieBiZhuangZhi;
	osThreadId id_motor1_JuanYangJi;
	osThreadId id_motor2_DangLiaoBan;
	osThreadId id_motor3_TuiBan;
	osThreadId id_motor4_CeDangBan;
	osThreadId id_motor5_TuiLiaoShenSuoJi;
} IDs;

typedef struct
{
	motor *motory;
	motor *motor0_TieBiZhuangZhi;
	motor *motor1_JuanYangJi;
	motor *motor2_DangLiaoBan;
	motor *motor3_TuiBan;
	motor *motor4_CeDangBan;
	motor *motor5_TuiLiaoShenSuoJi;
} Motors;

void thread_main(void *p);
void thread_serial_send(void *p);//从串口发送系统状态信息以打印log
void thread_serial_receive(void *p);//接收从串口发送来的命令并执行
void thread_LED(void *p);

void thread_turnOnMotor(void *p);//打开一个机器


#endif
