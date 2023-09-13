#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"
#include "includes.h"
#include "MPU6050.h"
void KEY_Init(void)
{
	/*RCC������䣬�����ⲿ�����ʱ��
	����EXTI��NVIC���ں˵ġ��ڲ����衱������ҪRCC����
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//ʱ��ʹ�����
	
	/*�ܽ�ʹ����䣬ͬʱ�����
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1|KEY2|KEY3|KEY4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_GPIO_PAN, &GPIO_InitStructure);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	/*AFIO�������
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//Ĭ�Ͻ�Pin4��EXTI4����������������Ҫ��ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);//ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);//ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	
	
	/*NVIC�������
	*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//1λ��ռ���ȼ���3λ�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	//4��Ϊ����ֹͣ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//5��Ϊ��λ
	//6��Ϊframe++
	//7��Ϊdelay++
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
