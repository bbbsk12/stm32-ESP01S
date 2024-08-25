#ifndef __Serial_H
#define __Serial_H
#include <stdio.h>
void Serial_Init(void);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
uint8_t USART_RxFlag(void);
uint8_t USART_GetRxData(void);
#endif
