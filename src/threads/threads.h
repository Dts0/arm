#ifndef __THREADS_H
#define  __THREADS_H

#include "cmsis_os.h" 

typedef struct
{
	osThreadId id_main,id_serial_send,id_serial_receive,id_LED;
} ids;

void thread_main(void *p);
void thread_serial_send(void *p);//从串口发送系统状态信息以打印log
void thread_serial_receive(void *p);//接收从串口发送来的命令并执行
void thread_LED(void *p);

void thread_open0(void *p);//打开贴壁装置
void thread_open1(void *p);//打开卷扬机
void thread_open2(void *p);//抬起挡料板
void thread_open3(void *p);//驱动推板

#endif
