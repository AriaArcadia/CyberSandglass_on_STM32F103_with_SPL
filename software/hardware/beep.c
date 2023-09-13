#include "beep.h"
#include "Delay.h"
void Beep_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
void Beep(uint8_t times){
	for(;times>0;times--){
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		Delay_ms(10);
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		Delay_ms(10);
	}
}
