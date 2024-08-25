#include "stm32f10x.h"
#include "MyUsart.h"
#include "Delay.h"
#include "string.h"
#include "LED.h"
#include "stdio.h"
#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"vivoS10e\",\"12345678\"\r\n"
#define MQTTUSERCFG             "AT+MQTTUSERCFG=0,1,\"NULL\",\"D001&k0cwzCsF9GJ\",\"6431a93c1ef3579c757012a1deffcf56144af68f8c85b0d1d78afafce07fd556\",0,0,\"\"\r\n"
#define MQTTCLIENTID           "AT+MQTTCLIENTID=0,\"k0cwzCsF9GJ.D001|securemode=2\\,signmethod=hmacsha256\\,timestamp=1711372810539|\"\r\n"   
const char* MQTTCONN				="iot-06z00hheac252d2.mqtt.iothub.aliyuncs.com";
const char* MQTTSUB="/sys/k0cwzCsF9GJ/D001/thing/event/property/post_reply";   
uint16_t DataPointer=0;
#define DataSize 512
char RxData[DataSize]={0};
uint8_t CompeteRx=0;


void Trail_Rxed(void)
{
	uint16_t Temp=DataPointer;
	Delay_ms(1);
	if(Temp==DataPointer)
	{
		Delay_ms(5);
		CompeteRx=1;
	}
}

char* atemp;
uint8_t ESP8266_SendCmd(char *cmd, char *res)
{
	
	Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	while(CompeteRx==0)Trail_Rxed();
	if(strstr(RxData, res) != NULL)
	{
		if(strstr(RxData,"switch")!=NULL)
		{
			Delay_ms(2);
			atemp=strstr(RxData,"switch");
			if(atemp[8]=='1')	  	LED_State(1);
			else if(atemp[8]=='0')	LED_State(0);
				
		}
		UsartPrintf(USART1, RxData);
		UsartPrintf(USART1, "\r\n");		

		DataPointer=0;memset(RxData,0,DataSize);CompeteRx=0;
		return 0;
	}
	else
	{
		DataPointer=0;memset(RxData,0,DataSize);CompeteRx=0;
		Delay_ms(10);	
		return 1;	
	}
	

}
void PUB_Data(int Hum,int Tem)
{
	char TxData[512]={0};
	sprintf(TxData,"AT+MQTTPUB=0,\"/sys/k0cwzCsF9GJ/D001/thing/event/property/post\",\"{\\\"params\\\": {\\\"CurrentHumidity\\\":%d\\,\\\"CurrentTemperature\\\":%d}}\",0,0\r\n",Hum,Tem);
	while(ESP8266_SendCmd(TxData, "OK"));	
}
void ESP01S_Start(void)
{
	UsartPrintf(USART1, " RST\r\n");
	while(ESP8266_SendCmd("AT+RESTORE\r\n", "ready"));
	Delay_ms(1500);


	UsartPrintf(USART1, "1.AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"));
	Delay_ms(1500);
	UsartPrintf(USART1, "1.ATE0\r\n");
	while(ESP8266_SendCmd("ATE0\r\n", "OK"));
	Delay_ms(1500);
	UsartPrintf(USART1, "2.CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n","OK"));
	Delay_ms(1500);
	UsartPrintf(USART1, "2.CWQAP\r\n");
	while(ESP8266_SendCmd("AT+CWQAP\r\n","OK"));
	Delay_ms(1500);
	UsartPrintf(USART1, "3.CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"));
	Delay_ms(1500);
	UsartPrintf(USART1, "4.MQTTUSERCFG\r\n");
	while(ESP8266_SendCmd(MQTTUSERCFG, "OK"));
	Delay_ms(1500);	
	UsartPrintf(USART1, "5.MQTTCLIENTID\r\n");
	while(ESP8266_SendCmd(MQTTCLIENTID, "OK"));
	Delay_ms(10000);	
	UsartPrintf(USART1, "6.MQTTCONN\r\n");
	while(ESP8266_SendCmd("AT+MQTTCONN=0,\"iot-06z00hheac252d2.mqtt.iothub.aliyuncs.com\",1883,1\r\n", "OK"));
	Delay_ms(500);	

	
}
void ESP01S_Init(void)
{
	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_2;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	//PA3	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_3;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	usartInitStruct.USART_BaudRate = 115200;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usartInitStruct.USART_Parity = USART_Parity_No;									//无校验
	usartInitStruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usartInitStruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &usartInitStruct);
	
	USART_Cmd(USART2, ENABLE);														//使能串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvicInitStruct.NVIC_IRQChannel = USART2_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicInitStruct);
	Delay_ms(3000);
	ESP01S_Start();
}



void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)==SET)
	{
		if(DataPointer>=DataSize) {DataPointer=0;memset(RxData,0,DataSize);}
		RxData[DataPointer++]=USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}
