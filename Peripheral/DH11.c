#include "stm32f10x.h"
#include "Delay.h"
uint8_t DHT11_Data[5];
void Pin_In()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);


	
}

void Pin_Out()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);	
	
}



void DHT11_Start(void)
{
	Pin_Out();
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	Delay_ms(20);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	Delay_us(30);
	Pin_In();

}

uint8_t DHT11_Recevice(void)
{
	uint8_t i,Data;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
		Delay_us(30);
		Data <<=1;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)Data|=1;
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1);
	}
	return Data;
}

void Check_Digit(void)
{
	uint8_t Temp[5];
	DHT11_Start();
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1);
		Temp[0]=DHT11_Recevice();
		Temp[1]=DHT11_Recevice();
		Temp[2]=DHT11_Recevice();
		Temp[3]=DHT11_Recevice();
		Temp[4]=DHT11_Recevice();

		GPIO_SetBits(GPIOA,GPIO_Pin_5);	
		Delay_us(10);
		if(Temp[4]==Temp[3]+Temp[2]+Temp[1]+Temp[0])
		{
			DHT11_Data[4]=Temp[4];
			DHT11_Data[3]=Temp[3];
			DHT11_Data[2]=Temp[2];
			DHT11_Data[1]=Temp[1];
			DHT11_Data[0]=Temp[0];
		}
	}



}
