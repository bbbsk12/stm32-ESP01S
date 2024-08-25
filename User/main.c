#include "stm32f10x.h"                  // Device header

#include "OLED.h"
#include "MyUsart.h"
#include "DH11.h"
#include "stdio.h"
#include "LED.h"
extern char RxData[512];
extern uint16_t DataPointer;
char* athemp;
#include "Delay.h"
#include "ESP01s.h"
#include "string.h"
#include "stm32f10x_it.h"
int i=0;
char Temp[30];
extern uint8_t DHT11_Data[5];

int main(void)
{
	//TIM2_Init();
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	Serial_Init();
	ESP01S_Init();

	while(1)
	{		
		
		i++;
		if(i==72000)
		{     
			
			Check_Digit( );
			UsartPrintf(USART1,"Hum:%d.%d,Tem:%d.%d\r\n",DHT11_Data[0],DHT11_Data[1],DHT11_Data[2],DHT11_Data[3]);
			sprintf(Temp,"Hum:%d.%d",DHT11_Data[0],DHT11_Data[1]);
			OLED_ShowString(1,1,Temp);
			sprintf(Temp,"Tem:%d.%d",DHT11_Data[2],DHT11_Data[3]);
			OLED_ShowString(2,1,Temp);
			PUB_Data(DHT11_Data[0],DHT11_Data[2]);i=0;
		}
		if(strstr(RxData,"switch")!=NULL)
		{
			Delay_ms(2);
			athemp=strstr(RxData,"switch");
			if(athemp[8]=='1')	  	LED_State(1);
			else if(athemp[8]=='0')	LED_State(0);
			UsartPrintf(USART1,RxData);
			UsartPrintf(USART1,"\r\n");
			DataPointer=0;memset(RxData,0,512);memset(athemp,0,30);	
		}
	}
}
