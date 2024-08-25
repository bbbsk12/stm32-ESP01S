#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"
uint8_t ReceviceData;
uint8_t ReceviceFlag;
void Serial_Init(void)
{  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);

	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate=43000;
	USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART1_InitStruct.USART_Parity=USART_Parity_No;
	USART1_InitStruct.USART_StopBits=USART_StopBits_1;
	USART1_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitStruct);

	USART_Cmd(USART1,ENABLE);

	

}
//void Usart_SendString(USART_TypeDef *USARTx, char *str, unsigned short len)
//{

//	unsigned short count = 0;
//	
//	for(; count < len; count++)
//	{
//		USART_SendData(USARTx, *str++);									//发送数据
//		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
//	}

//}
void Usart_SendString(USART_TypeDef *USARTx,char* String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		USART_SendData(USARTx,String[i]);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==0);				
	}
}
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
	
//	char String[100];
//	va_list arg;
//	va_start(arg, format);
//	vsprintf(String, format, arg);
//	va_end(arg);
//	Usart_SendString(USARTx,String);

}
//void USART1_IRQHandler(void)
//{
//	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
//	{
//		USART_ClearFlag(USART1, USART_FLAG_RXNE);
//	}

//}
