#include "stm32f10x.h"
#include "LED.h"
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIOA_InitStructure);
	LED_State(0);
}
