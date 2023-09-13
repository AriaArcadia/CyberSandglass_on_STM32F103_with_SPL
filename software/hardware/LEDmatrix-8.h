#ifndef _LED_M
#define _LED_M
#include "includes.h"



//这几个函数可以操作LED
void Write_7219byte(u8 x);
void Write_MAX7219(u8 addr,u8 val);
void Init_MAX7219(void);
void draw_matrix(u8 data[][length],u8 clear);
//这几个函数只操作数组
u8 ispixelon(u8 data[][length],u8 row,u8 col,u8 whichpiece);
void moveapixel(u8 data[][length],u8 row,u8 col,u8 whichpiece,u8 direct,u8 spin);
void updateapixel(u8 data[][length],u8 row,u8 col,u8 whichpiece,u8 b);
u8 whethertomove(u8 data[][length],u8 *row,u8 *col,u8 *updown, u8 direct);
void iteratepixels(u8 data[][length],u8 direct,u8 spin);

extern u8  drop_delay;
extern u16  frame_delay;
extern u8 matrix0[][length];
#endif
