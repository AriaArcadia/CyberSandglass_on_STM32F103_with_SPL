#ifndef _LED_M
#define _LED_M
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdint.h"
#include "includes.h"
extern u8 drop_delay;
extern u16 frame_delay;
extern u8 isstable;
#define MAX_DIN1 GPIO_Pin_11
#define MAX_CS1 GPIO_Pin_12
#define MAX_CLK1 GPIO_Pin_15
#define GPIO_LEDM1 GPIOA
#define MAX_DIN2 GPIO_Pin_12
#define MAX_CS2 GPIO_Pin_13
#define MAX_CLK2 GPIO_Pin_14
#define GPIO_LEDM2 GPIOB

//这几个函数可以操作LED
void Write_7219byte(u8 x,u8 select);
void Init_MAX7219(void);
void draw_matrix(u16 data[][length],u8 clear);
//这几个函数只操作数组
u8 ispixelon(u16 data[][length],u8 row,u8 col,u8 whichpiece);
void moveapixel(u16 data[][length],u8 row,u8 col,u8 whichpiece,u8 direct,u8 spin);
void updateapixel(u16 data[][length],u8 row,u8 col,u8 whichpiece,u8 b);
u8 whethertomove(u16 data[][length],u8 *row,u8 *col,u8 *updown, u8 direct);
void iteratepixels(u16 data[][length],u8 direct,u8 spin);


#endif
