#include "LEDmatrix.h"
#include "stm32f10x.h"


void Write_7219byte(u8 x){
	u8 i;
	if(count==1)GPIO_ResetBits(GPIOA,MAX_CS);//有一个的时候
	for(i=8;i>0;i--){
		GPIO_ResetBits(GPIOA,MAX_CLK);
		GPIO_WriteBit(GPIOA,MAX_DIN,x&0x80);
		x=x<<1;
		GPIO_SetBits(GPIOA,MAX_CLK);
	}
}
void Write_MAX7219(u8 addr,u8 val){
	GPIO_ResetBits(GPIOA, MAX_CS);
	Write_7219byte(addr);
	Write_7219byte(val);
	GPIO_SetBits(GPIOA, MAX_CS);
}
void Init_MAX7219(){
	u8 i=1;
	//端口设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MAX_CLK|MAX_CS|MAX_DIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_LEDM, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//使能JTAGDisable，即禁用JTAG接口
	//关闭JTAG的PB3 PB4 PA15的功能，作为普通I/O口使用，不影响SWD的下载调试。
	
	//适合级联的情况
	GPIO_ResetBits(GPIO_LEDM, MAX_CS);
	for (i=0;i<count;i++){Write_7219byte(0x09);Write_7219byte(0x00);}
	GPIO_SetBits(GPIO_LEDM, MAX_CS);
	GPIO_ResetBits(GPIO_LEDM, MAX_CS);
	for (i=0;i<count;i++){Write_7219byte(0x0a);Write_7219byte(0x03);}
	GPIO_SetBits(GPIO_LEDM, MAX_CS);
	GPIO_ResetBits(GPIO_LEDM, MAX_CS);
	for (i=0;i<count;i++){Write_7219byte(0x0b);Write_7219byte(0x07);}
	GPIO_SetBits(GPIO_LEDM, MAX_CS);
	GPIO_ResetBits(GPIO_LEDM, MAX_CS);
	for (i=0;i<count;i++){Write_7219byte(0x0c);Write_7219byte(0x01);}
	GPIO_SetBits(GPIO_LEDM, MAX_CS);
	GPIO_ResetBits(GPIO_LEDM, MAX_CS);
	for (i=0;i<count;i++){Write_7219byte(0x0f);Write_7219byte(0x00);}
	GPIO_SetBits(GPIO_LEDM, MAX_CS);
	/*
	//适合单个的情况
	Write_MAX7219(0x09, 0x00);       //译码方式：BCD码
	Write_MAX7219(0x0a, 0x03);       //亮度 
	Write_MAX7219(0x0b, 0x07);       //扫描界限；8个数码管显示
	Write_MAX7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
	Write_MAX7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
	*/
}

void draw_matrix(u8 data[][length],u8 clear){
	u8 i,j;
	for (i = 1; i < 9; i++)
    {
      GPIO_ResetBits(GPIO_LEDM,MAX_CS);
      for (j = 0; j < count; j++)
      {
        Write_7219byte(i);                           // 写入地址，即数码管编号
        if(!clear)
					Write_7219byte(0);
				else Write_7219byte(data[count - 1 - j][i - 1]); // 写入数据，即数码管显示数字
      }
      GPIO_SetBits(GPIO_LEDM,MAX_CS);
    }
}

u8 ispixelon(u8 data[][length],u8 row,u8 col,u8 whichpiece){//只有一句，判断该点亮不亮
	return bitRead(data[whichpiece][row-1],8-col);
}



void moveapixel(u8 data[][length],u8 row,u8 col,u8 whichpiece,u8 direct,u8 spin){
	if(ispixelon(data,row,col,whichpiece)){
		u8 r=row;
		u8 c=col;
		u8 w=whichpiece;
		u8 z=0;
		if(whethertomove(data,&r,&c,&w,direct)){
			z=1;
		}
		else if (!spin){
			if(whethertomove(data,&r,&c,&w,mod7(direct))){
				z=1;
			}
			else if(whethertomove(data,&r,&c,&w,rmod7(direct))){
				z=1;
			}	
		}
		else{
			if(whethertomove(data,&r,&c,&w,rmod7(direct)))z=1;
			else if(whethertomove(data,&r,&c,&w,mod7(direct)))z=1;
		}
		if(z){
			updateapixel(data,r,c,w,1);
			updateapixel(data,row,col,whichpiece,0);
		}
	}
}
void updateapixel(u8 data[][length],u8 row,u8 col,u8 whichpiece,u8 b){//只有一句，改变当前点
	bitWrite(data[whichpiece][row-1],8-col,b);
}
u8 whethertomove(u8 data[][length],u8 *row,u8 *col,u8 *updown, u8 direct){
	//0 1
/*
1     2     3

0     ·     4

7     6     5
*/
	u8 r=*row;
	u8 c=*col;
	u8 u=*updown;
	
	static u8 tick_goup=0;
	static u8 tick_godown=0;
	if(!(direct==0||direct==1||direct==2))tick_goup=0;
	if(!(direct==4||direct==5||direct==6))tick_godown=0;
	
	switch (direct){//按照方向，获得目标点（移动一格后下一个点的位置）
    case 0:
      (*col) --;
      break;
    case 1:
      (*row) --;
      (*col) --;
      break;
    case 2:
      (*row) --;
      break;
    case 3:
      (*row) --;
      (*col) ++;
      break;
    case 4:
      (*col) ++;
      break;
    case 5:;
      (*row) ++;
      (*col) ++;
      break;
    case 6:
      (*row) ++;
      break;
    case 7:
      (*row) ++;
      (*col) --;
      break;
  }
	
	if(*row>8||*col>8||*row<1||*col<1){
		if (*row > 8 && *col > 8 && *updown == 1){
			*row -= 8;
			*col -= 8;
			*updown = 0;
			tick_godown ++;
			if (tick_godown < drop_delay || ispixelon(data, *row, *col, *updown)){
				*row=r;
				*col=c;
				*updown=u;
				return 0;
			}
			else {
				tick_godown = 0;
				return 1;
			}
		}
		else if(*row < 1 && *col < 1 && *updown == 0){
			*row += 8;
      *col += 8;
      *updown = 1;
      tick_goup ++;
			if (tick_goup < drop_delay || ispixelon(data, *row, *col, *updown)){
        *row = r;
        *col = c;
        *updown = u;//保持原位置不变
        return 0;
      }
      else{
        tick_goup = 0;//清零
        return 1;//移动
      }
		}
		else {
			*row=r;
			*col=c;
			*updown=u;
			return 0;
		}
	}
	else if(ispixelon(data,*row,*col,*updown)){
		*row=r;
		*col=c;
		*updown=u;
		return 0;
	}
	else{
		return 1;
	}
	
}

void iteratepixels(u8 data[][length],u8 direct,u8 spin){//update
	switch (direct){//direct 0-7
    case 0://V
      for (int k = 1;k >= 0;k --){//上下片
        for (int i = 1;i <= 8;i ++){
          for (int j = 1;j <= 8;j ++){
            if (!spin)//重力方向发生反转
              moveapixel(data, j, i, k, direct, spin);//
            else
              moveapixel(data, 9-j, i, k, direct, spin);
          }
        }
      }
      break;

    case 1://V
      for (int k = 1;k >= 0;k --){
        for (int i = 2;i <= 16;i ++){
          if (i <= 9){
            for (int j = 0;j < i-1;j ++){
              if (!spin)
                moveapixel(data, j+1, i-j-1, k, direct, spin);
              else
                moveapixel(data, i-j-1, j+1, k, direct, spin);
            }
          }
          else{
            for (int j = 0;j < 17-i;j ++){
              if (!spin)
                moveapixel(data, i+j-8, 8-j, k, direct, spin);
              else
                moveapixel(data, 8-j, i+j-8, k, direct, spin);
            }
          }
        }
      }
      break;

    case 2://V
      for (int k = 1;k >= 0;k --){
        for (int i = 1;i <= 8;i ++){
          for (int j = 1;j <= 8;j ++){
            if (!spin)
              moveapixel(data, i, 9-j, k, direct, spin);
            else
              moveapixel(data, i, j, k, direct, spin);
          }
        }
      }
      break;

    case 3://V
      for (int i = 1;i <= 8;i ++){
        if (!spin){
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, i-j, 8-j, k, direct, spin);
          }
        }
        else{
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 1+j, 9-i+j, k, direct, spin);
          }
        }
      }
      for (int i = 7;i >= 1;i --){
        if (!spin){
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 8-j, i-j, k, direct, spin);
            }
        }
        else{
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 9-i+j, 1+j, k, direct, spin);
            }
        }
      }
      break;

    case 4://V
      for (int k = 0;k < 2;k ++){
        for (int i = 8;i > 0;i --){
          for (int j = 1;j <= 8;j ++){
            if (!spin)
              moveapixel(data, 9-j, i, k, direct, spin);
            else
              moveapixel(data, j, i, k, direct, spin);
          }
        }
      }
      break;

    case 5://V
      for (int k = 0;k < 2;k ++){
        for (int i = 16;i > 1;i --){
          if (i > 8){
            for (int j = 0;j < 17-i;j ++){
              if (!spin)
                moveapixel(data, 8-j, i+j-8, k, direct, spin);
              else
                moveapixel(data, i+j-8, 8-j, k, direct, spin);
            }
          }
          else{
            for (int j = 0;j < i-1;j ++){
              if (!spin)
                moveapixel(data, i-j-1, 1+j, k, direct, spin);
              else
                moveapixel(data, 1+j, i-j-1, k, direct, spin);
            }
          }
        }
      }
      break;
    case 6://V
      for (int k = 0;k < 2;k ++){
        for (int i = 8;i > 0;i --){
          for (int j = 1;j <= 8;j ++){
            if (!spin)
              moveapixel(data, i, j, k, direct, spin);
            else
              moveapixel(data, i, 9-j, k, direct, spin);
          }
        }
      }
      break;

    case 7:
      for (int i = 1;i <= 8;i ++){
        if (!spin){
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 9-i+j, 1+j, k, direct, spin);
          }
        }
        else{
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 8-j, i-j, k, direct, spin);
          }
        }
      }
      for (int i = 7;i >= 1;i --){
        if (!spin){
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 1+j, 9-i+j, k, direct, spin);
            }
        }
        else{
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, i-j, 8-j, k, direct, spin);
            }
        }
      }
      break;
  }
}
