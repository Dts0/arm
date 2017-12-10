#ifndef __THREADS_H
#define  __THREADS_H

#include "cmsis_os.h" 
#include "../main/defines.h"


typedef struct
{//用到的话再添加
	osThreadId id_main;
	osThreadId id_serial_send;
	osThreadId id_serial_receive;
	osThreadId id_LED;
	osThreadId id_motor0_TieBiZhuangZhi;
	osThreadId id_motor4_CeDangBan;
} IDs;

typedef struct
{
	motor *motory;
	motor *motor0_TieBiZhuangZhi;
	motor *motor1_JuanYangJi;
	motor *motor2_DangLiaoBanChuiZhi;
	motor *motor3_TuiBanChuiZhi;
	motor *motor4_CeDangBan;
	motor *motor5_DangLiaoBanTuiChu;
	motor *motor6_DaiDaoGan;
	motor *motor7_DongLiGunTong;
	motor *motor8_ChuanSongDai;
	motor *motor9_WuGan;
} Motors;

void thread_main(void *p);
void thread_serial_send(void *p);//从串口发送系统状态信息以检测运行状态
void thread_serial_receive(void *p);//接收从串口发送来的命令并执行
void thread_LED(void *p);

void thread_turnOnMotor(void *p);//打开一个电机或汽缸,并按照其stopFunc函数自行停止

void os_serialPrintf(char *str);//加了进程锁的串口输出,防止在传数据过程中被打断
char *os_serialReceivedString(unsigned int num);

#endif
