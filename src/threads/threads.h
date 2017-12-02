#ifndef __THREADS_H
#define  __THREADS_H

#include "cmsis_os.h" 
#include "../main/defines.h"

typedef struct
{
	osThreadId id_main,id_serial_send,id_serial_receive,id_LED;
} ids;


void thread_main(void *p);
void thread_serial_send(void *p);//从串口发送系统状态信息以打印log
void thread_serial_receive(void *p);//接收从串口发送来的命令并执行
void thread_LED(void *p);

void thread_turnOnMotor(void *p);//打开一个机器


#endif
