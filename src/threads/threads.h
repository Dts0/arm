#ifndef __THREADS_H
#define  __THREADS_H

#include "cmsis_os.h" 
#include "../main/defines.h"

#define motorThreadStackSize 100

typedef struct
{
	osThreadId id_main;
	osThreadId id_serial_send;
	osThreadId id_serial_receive;
	osThreadId id_LED;
	osThreadId id_motor0;
	osThreadId id_motor1;
	osThreadId id_motor2;
	osThreadId id_motor3;
	osThreadId id_motor4;
	osThreadId id_motor5;
} IDs;

typedef struct
{
	motor *motory;
	motor *motor0;
	motor *motor1;
	motor *motor2;
	motor *motor3;
	motor *motor4;
	motor *motor5;
} Motors;

void thread_main(void *p);
void thread_serial_send(void *p);//从串口发送系统状态信息以打印log
void thread_serial_receive(void *p);//接收从串口发送来的命令并执行
void thread_LED(void *p);

void thread_turnOnMotor(void *p);//打开一个机器


#endif
