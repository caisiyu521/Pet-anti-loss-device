#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1 GPIO_Pin_8
#define LED2 GPIO_Pin_9

void LED_Init(void);

#endif
