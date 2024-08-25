#ifndef __BUZZER_H
#define __BUZZER_H

#define LED_State(x) GPIO_WriteBit(GPIOA,GPIO_Pin_12, (BitAction)x)

void LED_Init(void);

#endif
