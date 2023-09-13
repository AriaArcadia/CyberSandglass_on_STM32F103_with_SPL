#ifndef __usart_H
#define __usart_H
#define BUF_SIZE 40
#include "system.h" 
#include "stdio.h" 
#include "string.h"
void USART_bt_Init(u32 bound);
void USART_asrpro_Init(u32 bound);
void copewith(char a,u8 b,char c);
extern u8  drop_delay;
extern u16  frame_delay;
extern u16 frame_delay0;
extern u8 STOP;
extern u8 toRESET;
#endif


