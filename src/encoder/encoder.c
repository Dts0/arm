#include "stm32f10x.h"
#include "encoder.h"
#include "misc.h"
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

static volatile int turn1=0;
static volatile int turn2=0;

static volatile int dis1=0;
void Encoder1Init()
{
	
}
uint32_t Encoder1GetCount(void)//获得计数数
{
	return 0;
}
uint32_t Encoder1GetDeg(void)//获得当前位置的度数,保留到整数
{
	return 0;
}
int Encoder1GetTurn(void)//获得圈数
{
	return turn1;
}
void Encoder1SetTurn(int turn)//设置圈数
{
	turn1=turn;
}
void Encoder1SetCount(int count)//设置计数
{
	int count0;
	count0++;//单纯不喜欢看到warning,为了消除warning
	count0=count;
}
int Encoder1GetDistance(int d)//获得距离
{
	//dis1=(int)(turn1*3.14*d+3.14*d*Encoder1GetDeg()/360)
	return dis1;
}
void Encoder1SetDistance(int dis,int d)//设置距离
{
	dis1=dis;
}

void Encoder2Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		//NVIC
	NVIC_InitTypeDef   NVIC_InitStructure;
	if(GPIO_Pin_1)
	{
	//EXTI,对应Z
	EXTI_InitTypeDef EXTI_InitStructure;
	//* config the extiline(PA1) clock and AFIO clock */
	//如果Z与AB不在同一GPIOx,这里需要使能Z的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	/* config the NVIC(PA1) */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI line gpio config(PB1) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PB1) mode config */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	}
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	//GPIO,对应TIM4的两个通道 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	//TIM4溢出
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	//TIM4,对应A,B
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 400*4-1; //计数器重装值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入;
	TIM_ICInitStructure.TIM_ICFilter = 6;   //选择输入比较滤波器 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
//	TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM4的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	TIM4->CNT = 0;//

	TIM_Cmd(TIM4, ENABLE);   //启动

}


uint32_t Encoder2GetCount(void)
{
	return ((TIM4->CNT)/4);
}
uint32_t Encoder2GetDeg(void)
{
	return ((TIM4->CNT/4)*360/400);
}
int Encoder2GetTurn(void)
{
	return turn2;
}
void Encoder2SetTurn(int turn)
{
	turn2=turn;
}
void Encoder2SetCount(int count)
{
	TIM4->CNT=count*4;
}
int Encoder2GetDistance(int d)
{
	return (int)(turn2*3.14*d+3.14*d*Encoder2GetCount()/400);
}
void Encoder2SetDistance(int dis,int d)
{
	Encoder2SetTurn(dis*d/3.14);
	Encoder2SetCount((400*(dis/3.14-turn2*d)));
}

void EXTI1_IRQHandler(void)
{
	TIM4->CNT = 0;
	TIM_Cmd(TIM4, ENABLE);
	EXTI_ClearITPendingBit(EXTI_Line1);
 											 
}

void TIM4_IRQHandler(void)//TIM4溢出
{ 	
	if(TIM4->SR&0x0001)
	{
		;
	}	
	TIM4->SR&=~(1<<0);
	if(TIM4->CNT==0)
		turn1++;
	else turn1--;	
}
void Encoder1LEDInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_All);
}

