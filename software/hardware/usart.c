#include "usart.h"		 
#include "OLED.h"
#include "beep.h"
#include "LEDmatrix.h"
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART_bt_Init(u32 bound)
{
   //GPIO端口设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);	
}
void copewith(char a,u8 b,char c){
	if(a=='S'){//SET
		toRESET=1;
		frame_delay=b;
		if(c=='M')frame_delay*=5;
		if(c=='S')frame_delay*=300;
		frame_delay0=frame_delay;
	}else if (a=='B'){//BEGIN
		STOP=0;
	}else if(a=='L'){//LOCK
		STOP=1;
	}else if(a=='R'){//RESET
		toRESET=1;
	}else if(a=='A'){//ALARM
		Beep(50);
	}
}
/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART1_IRQHandler(void) {
    uint8_t data;
    static char buffer[32];
    static uint8_t index = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(USART1);
        //if (data == 0XA5) {
				if(data=='+'){
            index = 0;//得到字符串不含+
        //} else if (data == 0X5A) {
				}else if(data=='-'){
            buffer[index] = '\0';//得到字符串不含-
						OLED_ShowString(4,1,buffer);
						copewith(buffer[0],(buffer[4]-'0')*10+buffer[5]-'0',buffer[7]);
            // Do something with the buffer
						//int i;
						//u8 capcha=0;
						//USART_SendData(USART2,0XA5);
						//for(i=0;i<index;i++){
							//capcha+=buffer[i];
							//USART_SendData(USART2,buffer[i]);
							//while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
							//OLED_ShowChar(4,i+1,buffer[i]);
						//}
						USART_SendData(USART1,'+');
						Beep(50);
        } else {
            buffer[index++] = data;
						//buffer[index]=data;
						//index++;
        }
    }
}


void USART_asrpro_Init(u32 bound){
	 //GPIO端口设置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void USART2_IRQHandler(void) {
    uint8_t data;
    static char buffer[32];
    static uint8_t index = 0;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(USART2);
        //if (data == 0XA5) {
				if(data=='+'){
            index = 0;//得到字符串不含+
        //} else if (data == 0X5A) {
				}else if(data=='-'){
            buffer[index] = '\0';//得到字符串不含-
						OLED_ShowString(4,1,buffer);
						copewith(buffer[0],(buffer[4]-'0')*10+buffer[5]-'0',buffer[7]);
            // Do something with the buffer
						//int i;
						//u8 capcha=0;
						//USART_SendData(USART2,0XA5);
						//for(i=0;i<index;i++){
							//capcha+=buffer[i];
							//USART_SendData(USART2,buffer[i]);
							//while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
							//OLED_ShowChar(4,i+1,buffer[i]);
						//}
						USART_SendData(USART2,'+');
						Beep(50);
        } else {
            buffer[index++] = data;
						//buffer[index]=data;
						//index++;
        }
    }
}
