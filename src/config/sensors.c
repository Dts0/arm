#include "config.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

//传感器函数的实现

void sensorsInit(void)//传感器初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|
																GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置为输入上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置为输入上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

bool HasEnteredBox(void)//光电传感器返回是否已经进箱子
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==SET)//读PD2管脚判断是否为高电位
		return true;
	else return false;
}
bool HasItems(void)//检查动力滚筒上是否有料
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)==SET)//读PD3管脚判断是否为高电位
		return true;
	else return false;
}
bool HasArrivedAtExtremePosition(void)//检测料到达动力滚筒极限位置
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==SET)//读PD6管脚判断是否为高电位
		return true;
	else return false;
}
bool HasArrivedAtB(void)//B传感器检测位置推板
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==SET)//读PD7管脚判断是否为高电位
		return true;
	else return false;
}
bool HasArrivedAtC(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==SET)//读PD11管脚判断是否为高电位
		return true;
	else return false;
}
bool HasArrivedAtD(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==SET)//读PD12管脚判断是否为高电位
		return true;
	else return false;
}
bool HasArrivedAtE(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==SET)//读PD13管脚判断是否为高电位
		return true;
	else return false;
}

positionStatus getTieBiPosition(void)//返回贴壁汽缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==SET)//读PE0管脚判断是否为高电位
		return open;
	else return close;
}
positionStatus getCeDangBanPosition(void)//返回侧挡板汽缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==SET)//读PE1管脚判断是否为高电位
		return open;
	else return close;
}
positionStatus getTuiBanPosition(void)//返回推板垂直汽缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==SET)//读PE2管脚判断是否为高电位
		return open;
	else return close;
}
positionStatus getDaiDaoGanPosition(void)//返回带导杆汽缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==SET)//读PE3管脚判断是否为高电位
		return open;
	else return close;
}
positionStatus getDangLiaoBanChuiZhiPosition(void)//返回挡料板垂直汽缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==SET)//读PE4管脚判断是否为高电位
		return open;
	else return close;
}
positionStatus getDangLiaoBanTuiChuPosition(void)//返回挡料板推出气缸的汽缸电磁传感器值
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==SET)//读PE5管脚判断是否为高电位
		return open;
	else return close;
}
