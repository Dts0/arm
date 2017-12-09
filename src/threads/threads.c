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
int distanceBuf;

void os_serialPrintf(char *str);//加了进程锁的串口输出,防止在传数据过程中被打断
char *os_serialReceivedString(unsigned int num);
void setStateRunFlag(int id,motorStatus flag);
void setStatePosFlag(int id,positionStatus flag);


void thread_main(void *p)
{
#ifdef _DEBUG
	os_serialPrintf("thread_main start\n");
#endif
	
//step1://step1:进入箱体
	motorTurn(motors.motory,on);//伸缩机电机正转,小车前进
	setStateRunFlag(-1,on);
	while(!HasEnteredBox());//在没有进箱子之前等待,检测光电传感器
	/*多余内容
	motorTurn(motors.motory,off);//伸缩机停
	setStateRunFlag(-1,off);
	*/
	
	/*//新线程,内容等价于
	motorTurn(motors.motor0_TieBiZhuangZhi,on);//贴壁汽缸向两侧各伸出90mm
	while(getTieBiPosition()!=open);//等待汽缸达到90mm位置
	motorTurn(motors.motor0_TieBiZhuangZhi,off);//停止汽缸的伸出
	*/
	os_pthread thread_motor0=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor0,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor0_TieBiZhuangZhi=osThreadCreate (osThread (thread_motor0), NULL);
	
	/*//新线程,内容等价于
	motorTurn(motors.motor4_CeDangBan,on);//侧挡板汽缸向两侧各伸出120mm
	while(getCeDangBanPosition()!=open);//等待汽缸达到120mm位置
	motorTurn(motors.motor4_CeDangBan,off);//停止汽缸的伸出
	*/
	os_pthread thread_motor4=(os_pthread)thread_turnOnMotor;
	osThreadDef(thread_motor4,osPriorityNormal, 1, motorThreadStackSize);
	ids.id_motor4_CeDangBan=osThreadCreate (osThread (thread_motor4), NULL);
	
	/*多余内容
	motorTurn(motors.motory,on);//伸缩机电机正转
	*/
	//小车继续前行至距离箱底10mm处,小车离集装箱底的距离=集装箱长度-小车长度-编码器计数距离
	while(BoxLength-CarLength-Encoder2GetDistance(D_ShenSuoJiDianJi)>10);
	//小于10毫米,伸缩机电机关闭
	motorTurn(motors.motory,off);
	
	while(getCeDangBanPosition()!=open);//检测侧挡板是否打开到位,在侧挡板到位前一直等待
	
	while(!HasItems());//检测动力滚筒上是否有料,无料时等待
	
	step2://装料
	//计数器为state.num,记录正在装载的物料的编号1,2,3,4
	motorTurn(motors.motor8_ChuanSongDai,on);//伸缩机传送带电机工作，开始输料
	motorTurn(motors.motor7_DongLiGunTong,on);//动力滚筒开始运行
	
	//M11:物料1
	while(!HasArrivedAtExtremePosition());//传感器检测料到达动力滚筒极限位置,达到极限位置前一直等待
	
	motorTurn(motors.motor3_TuiBanChuiZhi,on);//推板两个垂直气缸下行
	while(getTuiBanPosition()!=open);//下行到位前一直等待
	motorTurn(motors.motor3_TuiBanChuiZhi,off);//停止推板垂直汽缸下行
	
	motorTurn(motors.motor6_DaiDaoGan,on);//带导杆气缸前行
	while(getDaiDaoGanPosition()!=open);//带导杆汽缸前行到位前一直等待
	motorTurn(motors.motor6_DaiDaoGan,off);//停止带导杆气缸前行
	
	//M11 main:
	motorTurn(motors.motor9_WuGan,on);//无杆气缸左行，料至1处
	while(!HasArrivedAtB());//B传感器检测位置推板
	motorTurn(motors.motor9_WuGan,off);//先停
	motorTurn(motors.motor9_WuGan,rev);//再反转,无杆汽缸右行,返回
	while(!HasArrivedAtD());//D传感器检测位置推板,等待返回初始位置
	motorTurn(motors.motor9_WuGan,off);
	//END M11
	
	//这里是按照红字修正后的
	motorTurn(motors.motor6_DaiDaoGan,rev);//带导杆气缸退回原位
	while(getDaiDaoGanPosition()!=close);
	motorTurn(motors.motor6_DaiDaoGan,off);
	
	motorTurn(motors.motor3_TuiBanChuiZhi,rev);//推板气缸上行归位
	while(getTuiBanPosition()!=close);
	motorTurn(motors.motor3_TuiBanChuiZhi,off);
	//end
	
	//M12
	while(!HasArrivedAtExtremePosition());//传感器检测料到达动力滚筒极限位置,达到极限位置前一直等待
	
	motorTurn(motors.motor3_TuiBanChuiZhi,on);//推板两个垂直气缸下行
	while(getTuiBanPosition()!=open);//下行到位前一直等待
	motorTurn(motors.motor3_TuiBanChuiZhi,off);//停止推板垂直汽缸下行
	
	motorTurn(motors.motor6_DaiDaoGan,on);//带导杆气缸前行
	while(getDaiDaoGanPosition()!=open);//带导杆汽缸前行到位前一直等待
	motorTurn(motors.motor6_DaiDaoGan,off);//停止带导杆气缸前行
	
	//M12 main:
	motorTurn(motors.motor9_WuGan,on);//无杆气缸左行，料至2处
	while(!HasArrivedAtC());//C传感器检测位置推板
	motorTurn(motors.motor9_WuGan,off);//先停
	motorTurn(motors.motor9_WuGan,rev);//再反转,无杆汽缸右行,返回
	while(!HasArrivedAtD());//D传感器检测位置推板,等待返回初始位置
	motorTurn(motors.motor9_WuGan,off);
	//END M12
	
	//这里是按照红字修正后的
	motorTurn(motors.motor6_DaiDaoGan,rev);//带导杆气缸退回原位
	while(getDaiDaoGanPosition()!=close);
	motorTurn(motors.motor6_DaiDaoGan,off);
	
	motorTurn(motors.motor3_TuiBanChuiZhi,rev);//推板气缸上行归位
	while(getTuiBanPosition()!=close);
	motorTurn(motors.motor3_TuiBanChuiZhi,off);
	//end
	
	//M13
	while(!HasArrivedAtExtremePosition());//传感器检测料到达动力滚筒极限位置,达到极限位置前一直等待
	
	motorTurn(motors.motor3_TuiBanChuiZhi,on);//推板两个垂直气缸下行
	while(getTuiBanPosition()!=open);//下行到位前一直等待
	motorTurn(motors.motor3_TuiBanChuiZhi,off);//停止推板垂直汽缸下行
	
	motorTurn(motors.motor6_DaiDaoGan,on);//带导杆气缸前行
	while(getDaiDaoGanPosition()!=open);//带导杆汽缸前行到位前一直等待
	motorTurn(motors.motor6_DaiDaoGan,off);//停止带导杆气缸前行
	
	//M13 main:
	motorTurn(motors.motor9_WuGan,on);//无杆气缸左行，料至4处
	while(!HasArrivedAtE());//C传感器检测位置推板
	motorTurn(motors.motor9_WuGan,off);//先停
	motorTurn(motors.motor9_WuGan,rev);//再反转,无杆汽缸右行,返回
	while(!HasArrivedAtD());//D传感器检测位置推板,等待返回初始位置
	motorTurn(motors.motor9_WuGan,off);
	//END M13
	
	//这里是按照红字修正后的
	motorTurn(motors.motor6_DaiDaoGan,rev);//带导杆气缸退回原位
	while(getDaiDaoGanPosition()!=close);
	motorTurn(motors.motor6_DaiDaoGan,off);
	
	motorTurn(motors.motor3_TuiBanChuiZhi,rev);//推板气缸上行归位
	while(getTuiBanPosition()!=close);
	motorTurn(motors.motor3_TuiBanChuiZhi,off);
	//end
	
	
	//M14
	while(!HasArrivedAtExtremePosition());//传感器检测料到达动力滚筒极限位置,达到极限位置前一直等待
	
	motorTurn(motors.motor3_TuiBanChuiZhi,on);//推板两个垂直气缸下行
	while(getTuiBanPosition()!=open);//下行到位前一直等待
	motorTurn(motors.motor3_TuiBanChuiZhi,off);//停止推板垂直汽缸下行
	
	motorTurn(motors.motor6_DaiDaoGan,on);//带导杆气缸前行
	while(getDaiDaoGanPosition()!=open);//带导杆汽缸前行到位前一直等待
	motorTurn(motors.motor6_DaiDaoGan,off);//停止带导杆气缸前行
	
	//M14 main:
	state.count+=4;//计数器计四个数
	//END M14
	
	//这里是按照红字修正后的
	motorTurn(motors.motor6_DaiDaoGan,rev);//带导杆气缸退回原位
	while(getDaiDaoGanPosition()!=close);
	motorTurn(motors.motor6_DaiDaoGan,off);
	
	motorTurn(motors.motor3_TuiBanChuiZhi,rev);//推板气缸上行归位
	while(getTuiBanPosition()!=close);
	motorTurn(motors.motor3_TuiBanChuiZhi,off);
	//end
	
	motorTurn(motors.motor8_ChuanSongDai,off);//传送带停止
	
	//step3://step3
	motorTurn(motors.motor2_DangLiaoBanChuiZhi,on);//挡料板下行
	while(getDangLiaoBanChuiZhiPosition()!=open);//等待挡料板下行到位
	motorTurn(motors.motor2_DangLiaoBanChuiZhi,off);//停止挡料板下行
	
	if(state.height<0)//当1-6层时,编码器计数（下行960mm）
	{
		motorTurn(motors.motor1_JuanYangJi,on);//卷扬机下行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(distanceBuf-Encoder1GetDistance(D_JuanYangJi)<ItemHeight);//当移动的距离小于960(一个物料高)时等待
	}
	else if(state.height==0)//第7层时,下行50mm
	{
		motorTurn(motors.motor1_JuanYangJi,on);//卷扬机下行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(distanceBuf-Encoder1GetDistance(D_JuanYangJi)<50);//当移动的距离小于50时等待
	}
	else if(state.height>0 && state.height<8)//上行100,150,200,250,300,350mm,50+state.height*50
	{
		motorTurn(motors.motor1_JuanYangJi,rev);//卷扬机上行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(distanceBuf-Encoder1GetDistance(D_JuanYangJi)<50+state.height*50);//当移动的距离小于50+state.height*50时等待
	}
	else ;
	motorTurn(motors.motor1_JuanYangJi,off);//卷扬机停
	
	motorTurn(motors.motor5_DangLiaoBanTuiChu,on);//挡料板推出气缸伸出
	
	motorTurn(motors.motory,rev);//伸缩机电机反转，小车后退
	distanceBuf=Encoder2GetDistance(D_ShenSuoJiDianJi);//获得当前编码器距离读数并暂存
	while(distanceBuf-Encoder2GetDistance(D_ShenSuoJiDianJi)<ItemLength);//等待后退到位（编码器计数退910mm）
	motorTurn(motors.motory,off);//伸缩机电机停,小车停止
	
	while(getDangLiaoBanTuiChuPosition()!=open);//等待推出到位
	motorTurn(motors.motor5_DangLiaoBanTuiChu,off);//推出到位,停止挡料板推出
	
	motorTurn(motors.motor1_JuanYangJi,rev);//卷扬机工作，卸料盘上升
	
	motorTurn(motors.motor2_DangLiaoBanChuiZhi,rev);//挡料板复位(上行)//注:无法做到快速
	while(getDangLiaoBanChuiZhiPosition()!=open);//等待挡料板上行到位
	motorTurn(motors.motor2_DangLiaoBanChuiZhi,off);//停止挡料板上行
	
	motorTurn(motors.motor5_DangLiaoBanTuiChu,rev);//挡料板推出气缸缩回
	while(getDangLiaoBanTuiChuPosition()!=close);//等待缩回到位
	motorTurn(motors.motor5_DangLiaoBanTuiChu,off);//缩回到位,停止挡料板缩回
	
	osDelay(10000);//延时10秒
	
	motorTurn(motors.motory,on);//伸缩机电机正转，前行910mm
	distanceBuf=Encoder2GetDistance(D_ShenSuoJiDianJi);//获得当前编码器距离读数并暂存
	while(distanceBuf-Encoder2GetDistance(D_ShenSuoJiDianJi)<ItemLength);//等待前行到位（编码器计数进910mm）
	motorTurn(motors.motory,off);//伸缩机电机停,小车停止
	
	
	if(state.height<0)//当1-7层时,编码器计数（上行960mm）
	{
		motorTurn(motors.motor1_JuanYangJi,rev);//卷扬机上行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(Encoder1GetDistance(D_JuanYangJi)-distanceBuf<ItemHeight);//当移动的距离小于960(一个物料高)时等待
	}
	else if(state.height==0)
	{
		motorTurn(motors.motor1_JuanYangJi,rev);//卷扬机上行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(Encoder1GetDistance(D_JuanYangJi)-distanceBuf<50);//当移动的距离小于50时等待
	} 
	else if(state.height>0 && state.height<8)
	{
		motorTurn(motors.motor1_JuanYangJi,on);//卷扬机下行
		distanceBuf=Encoder1GetDistance(D_JuanYangJi);//获得当前编码器距离读数并暂存
		while(distanceBuf-Encoder1GetDistance(D_JuanYangJi)<50+state.height*50);//当移动的距离小于50+state.height*50时等待
	
	}
	motorTurn(motors.motor1_JuanYangJi,off);//卷扬机停
	
	//step4://step4重复上述步骤
	if(state.height<MaxHeight)//小于14层时,返回step2,码下一层
	{
		state.height++;
		goto step2;
	}
	//step5://重复
	if(state.column<MaxColumn)//小于最大码的列数时,返回step2,码下一列
	{
		state.column++;
		state.height=0;
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
	sprintf(msg,"%s%d ",msg,state->runningFlag2_DangLiaoBanChuiZhi);
	sprintf(msg,"%s%u ",msg,state->runningFlag3_TuiBanChuiZhi);
	sprintf(msg,"%s%d ",msg,state->runningFlag4_CeDangBan);
	sprintf(msg,"%s%d ",msg,state->runningFlag5_DangLiaoBanTuiChu);
	sprintf(msg,"%s%d ",msg,state->runningFlag6_DaiDaoGan);
	sprintf(msg,"%s%d ",msg,state->runningFlag7_DongLiGunTong);
	sprintf(msg,"%s%d ",msg,state->positionFlag0_TieBiZhuangZhi);
	sprintf(msg,"%s%d ",msg,state->positionFlag1_JuanYangJi);
	sprintf(msg,"%s%d ",msg,state->positionFlag2_DangLiaoBanChuiZhi);
	sprintf(msg,"%s%u ",msg,state->positionFlag3_TuiBanChuiZhi);
	sprintf(msg,"%s%d ",msg,state->positionFlag4_CeDangBan);
	sprintf(msg,"%s%d ",msg,state->positionFlag5_TuiLiaoShenSuoJi);
	sprintf(msg,"%s%u ",msg,state->c);
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
	sprintf(s,"motor%d turn off,position open\n",motorPointer->id);
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
		case 2:state.runningFlag2_DangLiaoBanChuiZhi=flag;break;
		case 3:state.runningFlag3_TuiBanChuiZhi=flag;break;
		case 4:state.runningFlag4_CeDangBan=flag;break;
		case 5:state.runningFlag5_DangLiaoBanTuiChu=flag;break;
		case 6:state.runningFlag6_DaiDaoGan=flag;break;
		case 7:state.runningFlag7_DongLiGunTong=flag;break;
		case -1:state.runningFlagy=flag;break;
		default:;
	}
}
void setStatePosFlag(int id,positionStatus flag)
{
	switch(id)
	{
		case 0:state.positionFlag0_TieBiZhuangZhi=flag;break;
		case 1:state.positionFlag1_JuanYangJi=flag;break;
		case 2:state.positionFlag2_DangLiaoBanChuiZhi=flag;break;
		case 3:state.positionFlag3_TuiBanChuiZhi=flag;break;
		case 4:state.positionFlag4_CeDangBan=flag;break;
		case 5:state.positionFlag5_TuiLiaoShenSuoJi=flag;break;
		default:;
	}
}

