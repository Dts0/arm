
#include "serial.h"

void SerialInit(void)
{
	//发送配置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* 配置串口1 USART1 Tx (PA.09)）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* 配置串口1 USART1 Rx (PA.10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure); 
	
	//接收配置
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      //优先组为2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //打开USART中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //中断通道使能
	NVIC_Init(&NVIC_InitStructure);
 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //串口接收中断使能
 
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void SerialPrintf(char *str)
{
	for(int i=0;str[i]!='\0'&&str[i]!=0x0;i++)
	{USART_SendData(USART1,str[i]);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);}
}
char SerialReceivedChar(void)
{
	return (char)USART_ReceiveData(USART1);
}

FlagStatus RX_status,RX_reponse,RX_save;  //定义标志位
char receivedString[1024];
char receivedChar;
unsigned int Num_receivedChar=0,Num_max=0;

char* SerialReceivedString(unsigned int maxChar)
{
	Num_receivedChar=0;
	Num_max=maxChar;
	for(int i;i<maxChar;i++)
		receivedString[i]='\0';
	RX_save=SET;
	while(RX_save==SET);
	return receivedString;
}
void SerialEnableReponse(FlagStatus isEnable)//定义是否响应(按原样返回输入的值)
{
	RX_reponse=isEnable;
}
void USART1_IRQHandler(void)
{  	
	RX_status = USART_GetFlagStatus(USART1, USART_FLAG_RXNE);//读取接收数据标志位，如果装好了一帧数据则硬件将其置一。
	if(RX_status == SET) 
	{
		receivedChar=USART_ReceiveData(USART1);
		if(RX_reponse == SET)
		{
			USART_SendData(USART1 , receivedChar);//将收到的数据再由STM32发送给PC机。
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);//等待发送完成。
		}
		if(RX_save == SET)
		{
			if(receivedChar=='\0'||Num_receivedChar>=Num_max)
			{
				receivedString[Num_receivedChar]='\0';
				RX_save = RESET;
			}
			else 
			{
				receivedString[Num_receivedChar]=receivedChar;
				Num_receivedChar++;
			}
		}
	}
}
