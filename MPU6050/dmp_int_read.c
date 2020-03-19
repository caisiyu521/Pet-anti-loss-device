#include "dmp_int_read.h"

void DMP_ReadData_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;  
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//初始化 GPIOC4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	
	//GPIOC4 中断线以及中断初始化配置   上升沿触发  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure);     //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器  
}











