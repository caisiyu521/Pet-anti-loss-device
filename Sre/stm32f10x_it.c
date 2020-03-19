#include "stm32f10x_it.h"
#include "stdio.h"

extern uchar  GPRS_Delay_1S ;

void TIM2_IRQHandler(void){	//TIM2中断服务函数
	static int count1 ,count4;
	static uint8_t flag;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){//判断是否进入TIM2更新中断
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除TIM2更新中断
		if(++count1 == 200){
			count1 = 0;
			if(flag == 0) { flag = 1; PAout(8) = 0;}
			else if(flag == 1) { flag = 0; PAout(8) = 1;}
		}
		if(GPRS_Delay_1S == 1){					//精确延时1S
			if(++count4 == 1000){
				count4 = 0;
				GPRS_Delay_1S = 0;
			}
		}
	}
}

