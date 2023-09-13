#include "LEDmatrix.h"
#include "stm32f10x.h"


void Write_7219byte(u8 x,u8 select){
	u8 i;
	if(select==1){
		if(count==1)GPIO_ResetBits(GPIO_LEDM1,MAX_CS1);//��һ����ʱ��
		for(i=8;i>0;i--){
			GPIO_ResetBits(GPIO_LEDM1,MAX_CLK1);
			GPIO_WriteBit(GPIO_LEDM1,MAX_DIN1,x&0x80);
			x=x<<1;
			GPIO_SetBits(GPIO_LEDM1,MAX_CLK1);
		}
	}
	else{
		if(count==1)GPIO_ResetBits(GPIO_LEDM2,MAX_CS2);//��һ����ʱ��
		for(i=8;i>0;i--){
			GPIO_ResetBits(GPIO_LEDM2,MAX_CLK2);
			GPIO_WriteBit(GPIO_LEDM2,MAX_DIN2,x&0x80);
			x=x<<1;
			GPIO_SetBits(GPIO_LEDM2,MAX_CLK2);
			}
	}
}
void Init_MAX7219(){
	u8 i=1;
	//�˿�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MAX_CLK1|MAX_CS1|MAX_DIN1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_LEDM1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MAX_CLK2|MAX_CS2|MAX_DIN2;
	GPIO_Init(GPIO_LEDM2, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//ʹ��JTAGDisable��������JTAG�ӿ�
	//�ر�JTAG��PB3 PB4 PA15�Ĺ��ܣ���Ϊ��ͨI/O��ʹ�ã���Ӱ��SWD�����ص��ԡ�
	
	//�ʺϼ��������
	GPIO_ResetBits(GPIO_LEDM1, MAX_CS1);
	for (i=0;i<count;i++){Write_7219byte(0x09,1);Write_7219byte(0x00,1);}
	GPIO_SetBits(GPIO_LEDM1, MAX_CS1);
	GPIO_ResetBits(GPIO_LEDM1, MAX_CS1);
	for (i=0;i<count;i++){Write_7219byte(0x0a,1);Write_7219byte(0x03,1);}
	GPIO_SetBits(GPIO_LEDM1, MAX_CS1);
	GPIO_ResetBits(GPIO_LEDM1, MAX_CS1);
	for (i=0;i<count;i++){Write_7219byte(0x0b,1);Write_7219byte(0x07,1);}
	GPIO_SetBits(GPIO_LEDM1, MAX_CS1);
	GPIO_ResetBits(GPIO_LEDM1, MAX_CS1);
	for (i=0;i<count;i++){Write_7219byte(0x0c,1);Write_7219byte(0x01,1);}
	GPIO_SetBits(GPIO_LEDM1, MAX_CS1);
	GPIO_ResetBits(GPIO_LEDM1, MAX_CS1);
	for (i=0;i<count;i++){Write_7219byte(0x0f,1);Write_7219byte(0x00,1);}
	GPIO_SetBits(GPIO_LEDM1, MAX_CS1);
	
	GPIO_ResetBits(GPIO_LEDM2, MAX_CS2);
	for (i=0;i<count;i++){Write_7219byte(0x09,2);Write_7219byte(0x00,2);}
	GPIO_SetBits(GPIO_LEDM2, MAX_CS2);
	GPIO_ResetBits(GPIO_LEDM2, MAX_CS2);
	for (i=0;i<count;i++){Write_7219byte(0x0a,2);Write_7219byte(0x03,2);}
	GPIO_SetBits(GPIO_LEDM2, MAX_CS2);
	GPIO_ResetBits(GPIO_LEDM2, MAX_CS2);
	for (i=0;i<count;i++){Write_7219byte(0x0b,2);Write_7219byte(0x07,2);}
	GPIO_SetBits(GPIO_LEDM2, MAX_CS2);
	GPIO_ResetBits(GPIO_LEDM2, MAX_CS2);
	for (i=0;i<count;i++){Write_7219byte(0x0c,2);Write_7219byte(0x01,2);}
	GPIO_SetBits(GPIO_LEDM2, MAX_CS2);
	GPIO_ResetBits(GPIO_LEDM2, MAX_CS2);
	for (i=0;i<count;i++){Write_7219byte(0x0f,2);Write_7219byte(0x00,2);}
	GPIO_SetBits(GPIO_LEDM2, MAX_CS2);
	
	/*
	//�ʺϵ��������
	Write_MAX7219(0x09, 0x00);       //���뷽ʽ��BCD��
	Write_MAX7219(0x0a, 0x03);       //���� 
	Write_MAX7219(0x0b, 0x07);       //ɨ����ޣ�8���������ʾ
	Write_MAX7219(0x0c, 0x01);       //����ģʽ��0����ͨģʽ��1
	Write_MAX7219(0x0f, 0x00);       //��ʾ���ԣ�1�����Խ�����������ʾ��0
	*/
}

void draw_matrix(u16 data[][length],u8 clear){
	u8 i,j;
	for (i = 1; i < 9; i++)
    {
      GPIO_ResetBits(GPIO_LEDM1,MAX_CS1);
      for (j = 0; j <=1; j++)
      {
        Write_7219byte(i,1);                           // д���ַ��������ܱ��
				Write_7219byte((data[0][i - 1+8*j]>>8)& 0xff,1); // д�����ݣ����������ʾ����
				Write_7219byte(i,1);
				Write_7219byte((data[0][i - 1+8*j])& 0xff,1);
      }
      GPIO_SetBits(GPIO_LEDM1,MAX_CS1);
			
			GPIO_ResetBits(GPIO_LEDM2,MAX_CS2);
      for (j = 0; j <=1; j++)
      {
        Write_7219byte(i,2);                           // д���ַ��������ܱ��
				Write_7219byte((data[1][i - 1+8*j]>>8)& 0xff,2);
				Write_7219byte(i,2);
				Write_7219byte((data[1][i - 1+8*j])& 0xff,2);				
      }
      GPIO_SetBits(GPIO_LEDM2,MAX_CS2);
			
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//������������������漰��ʾ
//////////////////////////////////////////////////////////////////////////////////////////
u8 ispixelon(u16 data[][length],u8 row,u8 col,u8 whichpiece){//ֻ��һ�䣬�жϸõ�������
	return bitRead(data[whichpiece][row-1],length-col);
	//if(whichpiece==1)GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
//row 1-16
//col 1-16

void moveapixel(u16 data[][length],u8 row,u8 col,u8 whichpiece,u8 direct,u8 spin){
	if(ispixelon(data,row,col,whichpiece)){
		u8 r=row;
		u8 c=col;
		u8 w=whichpiece;
		u8 z=0;
		if(whethertomove(data,&r,&c,&w,direct)){
			z=1;
		}
		else if (!spin){
			if(whethertomove(data,&r,&c,&w,mod7(direct)))z=1;
			else if(whethertomove(data,&r,&c,&w,rmod7(direct)))z=1;
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
void updateapixel(u16 data[][length],u8 row,u8 col,u8 whichpiece,u8 b){//ֻ��һ�䣬�ı䵱ǰ��
	bitWrite(data[whichpiece][row-1],length-col,b);
}
u8 whethertomove(u16 data[][length],u8 *row,u8 *col,u8 *whichpiece, u8 direct){
	//0 1
/*
1     2     3

0     ��     4

7     6     5
*/
	u8 r=*row;
	u8 c=*col;
	u8 u=*whichpiece;
	
	static u8 tick_goup=0;
	static u8 tick_godown=0;
	if(!(direct==0||direct==1||direct==2))tick_goup=0;
	if(!(direct==4||direct==5||direct==6))tick_godown=0;
	
	switch (direct){//���շ��򣬻��Ŀ��㣨�ƶ�һ�����һ�����λ�ã�
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
	
	if(*row>length||*col>length||*row<1||*col<1){//����ܳ��ÿ���ӵı߽�
		if(isstable){
			if (*row > length && *col > length && *whichpiece == 1){
				*row -= length;
				*col -= length;
				*whichpiece = 0;
				tick_godown ++;
				if (tick_godown < drop_delay || ispixelon(data, *row, *col, *whichpiece)){
					*row=r;
					*col=c;
					*whichpiece=u;
					return 0;
				}
				else {
					tick_godown = 0;
					return 1;
				}
			}
			else if(*row < 1 && *col < 1 && *whichpiece == 0){
				*row += length;
				*col += length;
				*whichpiece = 1;
				tick_goup ++;
				if (tick_goup < drop_delay || ispixelon(data, *row, *col, *whichpiece)){
					*row = r;
					*col = c;
					*whichpiece = u;//����ԭλ�ò���
					return 0;
				}
				else{
					tick_goup = 0;//����
					return 1;//�ƶ�
				}
			}
			else {
				*row=r;
				*col=c;
				*whichpiece=u;
				return 0;
			}
		}
		else {
			*row=r;
			*col=c;
			*whichpiece=u;
			return 0;
		}
	}
	else if(ispixelon(data,*row,*col,*whichpiece)){//�����һ����������
		*row=r;
		*col=c;
		*whichpiece=u;
		return 0;
	}
	else{//�����������һ���㲻��
		return 1;
	}
	
}

void iteratepixels(u16 data[][length],u8 direct,u8 spin){//update
	switch (direct){//direct 0-7
    case 0://V
      for (int k = 1;k >= 0;k --){//����Ƭ
        for (int i = 1;i <= length;i ++){
          for (int j = 1;j <= length;j ++){
            if (!spin)//������������ת
              moveapixel(data, j, i, k, direct, spin);//
            else
              moveapixel(data, length+1-j, i, k, direct, spin);
          }
        }
      }
      break;

    case 1://V
      for (int k = 1;k >= 0;k --){
        for (int i = 2;i <= 2*length;i ++){
          if (i <= length+1){
            for (int j = 0;j < i-1;j ++){
              if (!spin)
                moveapixel(data, j+1, i-j-1, k, direct, spin);
              else
                moveapixel(data, i-j-1, j+1, k, direct, spin);
            }
          }
          else{
            for (int j = 0;j < 2*length+1-i;j ++){
              if (!spin)
                moveapixel(data, i+j-length, length-j, k, direct, spin);
              else
                moveapixel(data, length-j, i+j-length, k, direct, spin);
            }
          }
        }
      }
      break;

    case 2://V
      for (int k = 1;k >= 0;k --){
        for (int i = 1;i <= length;i ++){
          for (int j = 1;j <= length;j ++){
            if (!spin)
              moveapixel(data, i, length+1-j, k, direct, spin);
            else
              moveapixel(data, i, j, k, direct, spin);
          }
        }
      }
      break;

    case 3://V
      for (int i = 1;i <= length;i ++){
        if (!spin){
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, i-j, length-j, k, direct, spin);
          }
        }
        else{
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 1+j, length+1-i+j, k, direct, spin);
          }
        }
      }
      for (int i = length-1;i >= 1;i --){
        if (!spin){
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, length-j, i-j, k, direct, spin);
            }
        }
        else{
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, length+1-i+j, 1+j, k, direct, spin);
            }
        }
      }
      break;

    case 4://V
      for (int k = 0;k < 2;k ++){
        for (int i = length;i > 0;i --){
          for (int j = 1;j <= length;j ++){
            if (!spin)
              moveapixel(data, length+1-j, i, k, direct, spin);
            else
              moveapixel(data, j, i, k, direct, spin);
          }
        }
      }
      break;

    case 5://V
      for (int k = 0;k < 2;k ++){
        for (int i = 2*length;i > 1;i --){
          if (i > length){
            for (int j = 0;j < 2*length+1-i;j ++){
              if (!spin)
                moveapixel(data, length-j, i+j-length, k, direct, spin);
              else
                moveapixel(data, i+j-length, length-j, k, direct, spin);
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
        for (int i = length;i > 0;i --){
          for (int j = 1;j <= length;j ++){
            if (!spin)
              moveapixel(data, i, j, k, direct, spin);
            else
              moveapixel(data, i, length+1-j, k, direct, spin);
          }
        }
      }
      break;

    case 7:
      for (int i = 1;i <= length;i ++){
        if (!spin){
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, length+1-i+j, 1+j, k, direct, spin);
          }
        }
        else{
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, length-j, i-j, k, direct, spin);
          }
        }
      }
      for (int i = length-1;i >= 1;i --){
        if (!spin){
          for (int k = 1;k >= 0;k --){
            for (int j = 0;j < i;j ++)
              moveapixel(data, 1+j, length+1-i+j, k, direct, spin);
            }
        }
        else{
          for (int k = 0;k < 2;k ++){
            for (int j = 0;j < i;j ++)
              moveapixel(data, i-j, length-j, k, direct, spin);
            }
        }
      }
      break;
  }
}
