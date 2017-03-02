/*****************************************************

Project : 
Version : 
Date    : 2016/12/10
Author  : yanglei zhusiyao zengshiyuan
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>
#include <stdlib.h>
#include <string.h>
#include <delay.h>
#include <string.h>
#include <1wire.h>
// Alphanumeric LCD Module functions
#include <alcd.h>
// DS1820 Temperature Sensor functions
#include <ds18b20.h>
#include <stdlib.h>
/*MAIN*/
int mode=0;
int debug=2;
/*sec time*/
char rec_i=0;
bit flag_a=1;
bit flag_time=1;
bit flag_time1=0;
int time_counter=0;
unsigned char t=0,sec=0,mins=0,hour=0;
unsigned char str[30]={0};
unsigned char rec[10][4]={0};
unsigned char dis_mode=1;
unsigned char rec_num=0;
/*hong wai*/
unsigned char IrValue[20];
unsigned char Time;
/*day*/
flash unsigned char NUM[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
unsigned char y,mon,d,h,m,s,iWeek,c0 = 0,y0 = 0,mon0 = 0,d0 = 0;
unsigned char yh,yl,monh,monl,dh,dl,hh,hl,mh,ml,sh,sl;                   //时间的高低位
int i=0,j=0;
//temp
float temp,fa,the;

flash int ToneTable[]={0,                               
//    1            3             5      6             8           10            12
//低音1    #1      2      #2     3      4      #4     5     #5    6      #6      7      
   0X0EE0,0X0E1A,0X0D49,0X0C8F,0X0BD6,0X0B31,0X0A8E,0X09F7,0X0969,0X08E0,0X0861,0X07E8, 
//   13           15             17    18            20          22             24
//中音1    #1      2      #2     3      4      #4     5     #5    6      #6      7
   0X0778,0X070D,0X06A7,0X0647,0X05ED,0X0598,0X0547,0X04FB,0X04B3,0X0470,0X0430,0X03F4,
//   25           27           29     30             32          34             36
//高音1    #1      2      #2     3      4      #4     5     #5    6      #6      7
   0X03BC,0X0385,0X0353,0X0323,0X02F6,0X02CB,0X02A3,0X027D,0X025A,0X0238,0X0218,0X01FC            
};  
       
       
/*------------------两只老虎-------------*/
flash int MusicTable1[77]={13,2,  15,2,    17,2, 13,1, 0,1,  
                     13,2,  15,2,  17,2,   13,1,  0,1, 
                     17,2,  18,2,  20,2,    0,2,
                     17,2,  18,2,  20,2,    0,2,
                     20,1,  22,1,  20,1,   18,1,  17,2,  13,2,  
                     20,1,  22,1,  20,1,   18,1,  17,2,  13,2, 
                     15,2,   8,2,  13,2,    0,2,
                     15,2,   8,2,  13,2,    0,2,
                     0xff};  
/*-----------------新年好------------------*/
flash int MusicTable2[]={ 13,1,  13,1,  13,2,   8,2,//音符,拍数,  
                      17,1,  17,1,  17,2,  13,2,
                      13,1,  17,1,  20,2,  20,2,
                      18,1,  17,1,  15,2,   0,2,
                      15,1,  17,1,  18,2,  18,2,
                      17,1,  15,1,  17,2,  13,2,
                      13,1,  17,1,  15,2,   8,2,
                      12,1,  15,1,  13,2,   0,2,
                     
                      13,1,  13,1,  13,2,   8,2,//音符,拍数,  
                      17,1,  17,1,  17,2,  13,2,
                      13,1,  17,1,  20,2,  20,2,
                      18,1,  17,1,  15,2,   0,2,
                      15,1,  17,1,  18,2,  18,2,
                      17,1,  15,1,  17,2,  13,2,
                      13,1,  17,1,  15,2,   8,2,
                      12,1,  15,1,  13,2,   0,2,
                      0xff};  
/*-----------------菊花台-------------------------*/  
flash int MusicTable3[]={
		 0,2,   17,4,  17,2,  15,2,  17,4,  0,4,         //音符,拍数,  
 		 17,2,  20,2,  17,2,  15,2,  17,4,  0,4,         //每行为一小节
   13,4,  13,2,  15,2,  17,2,  20,2,  17,2, 0,2,
 				15,4,  15,2,  13,2,  15,4,  0,4,
 		 17,3,  20,1,  17,0,  22,1,  20,1,  0,1,
    22,1,  20,1,  20,1,  17,1,  20,3,  8,1, 0,1,
   17,2,  15,1,  15,1,  20,2,  17,1,  15,1, 0,1,
                15,2,  15,1,  13,1,  15,2,  0,2,
                17,2,  17,1,  15,1,  17,2,  0,2,
         17,1,  20,1,  17,1,  15,1,  17,2,  0,2,
    13,2,  13,1,  15,1,  17,1,  20,1,  17,2,0,1,
                15,2,  15,1,  13,1,  15,2,  0,2,
         17,2,  20,0,  17,1,  22,2,  20,3,  0,1,
         22,1,  20,1,  20,1,  17,1,  20,2,  0,1,
0,1,   17,1,  15,1,  17,1,  20,2,  17,1, 15,1, 0,1,  
 0xFF
};
/*---------------------------------------------------*/
#asm
   .equ __ds1302_port=0x1B ;PORTA
   .equ __ds1302_io=6
   .equ __ds1302_sclk=7
   .equ __ds1302_rst=5
   .equ __w1_port=0x15
   .equ __w1_bit=0x07
#endasm
#include <ds1302.h>

#define twinkle_time 200

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    unsigned char j,k;
    unsigned int err;
    Time=0;                     
    delay_us(9800);

    if(PIND.2==0)        //确认是否真的接收到正确的信号
    {     
        
        err=1000;                //1000*10us=10ms,超过说明接收到错误的信号
        /*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
        侯，程序死在这里*/    
        while((PIND.2==0)&&(err>0))    //等待前面9ms的低电平过去          
        {            
            delay_us(140);
            err--;
        } 
        if(PIND.2==1)            //如果正确等到9ms低电平
        {
            err=500;
            while((PIND.2==1)&&(err>0))		 //等待4.5ms的起始高电平过去
			{
				delay_us(140);
				err--;
			}
			for(k=0;k<4;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					err=60;		
					while((PIND.2==0)&&(err>0))//等待信号前面的560us低电平过去
					{
						delay_us(140);
						err--;
					}
					err=500;
					while((PIND.2==1)&&(err>0))	 //计算高电平的时间长度。
					{
						delay_us(140);//0.14ms
						Time++;
						err--;
						if(Time>30)
						{
							//EX0=1;
							return;
						}
					}
					IrValue[k]>>=1;	 //k表示第几组数据
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//用完时间要重新赋值							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}	   
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
// Place your code here

}

// Timer 0 output compare interrupt service routine
interrupt [TIM0_COMP] void timer0_comp_isr(void)
{
  time_counter++;
    if(time_counter==10)
      {
       time_counter=0;
       t=t+1;
       if(t==100)   {t=0;sec++;}
       if(sec==60)  {sec=0;mins++;flag_a=1;}
       if(mins==60)  {mins=0;hour++;}
       if(hour==24)  hour=0;
       flag_time=1; 
      }

}


void normal_display(unsigned char tmp_t,unsigned char tmp_sec,unsigned char tmp_mins,unsigned char tmp_hour)
{

 if(flag_time)
 {   
    if((tmp_t==0)||dis_mode||flag_time1)
    {
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_puts("h:m:s");
        lcd_gotoxy(7,0);
        lcd_puts("REC:");
        if(rec_i<10) lcd_putchar(rec_i+48);
        else lcd_puts("FULL");
        flag_time1=0;
    }
    ftoa(tmp_hour,0,str);
    lcd_gotoxy(0,1);
    lcd_puts(str);
    lcd_putchar(':');

    ftoa(tmp_mins,0,str);
    lcd_puts(str);
 
    lcd_putchar(':');
                         
    ftoa(tmp_sec,0,str);
    lcd_puts(str);
      
    lcd_putchar(':');    
    
    ftoa(tmp_t,0,str);
    lcd_puts(str);
    
    flag_time=0;
    if(dis_mode)dis_mode=2;  
 }

}
 
void sec_display(void)
{
        if(IrValue[1]==0x21)     
        {
          TIMSK^=0x02;
          IrValue[1]=0x00; 
          dis_mode=0;
           
        }
        
        if(IrValue[1]==0x04) 
         { 
         
           if(rec_i<10)
           {
              rec[rec_i][0]=hour;
              rec[rec_i][1]=mins;
              rec[rec_i][2]=sec;
              rec[rec_i][3]=t;
              rec_i++;                
           }  
           flag_time=1;
           flag_time1=1;
           IrValue[1]=0x00; 
         }
         
         if(IrValue[1]==0x8b){flag_time=1;rec_num=0;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0x86){flag_time=1;rec_num=1;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0x8c){flag_time=1;rec_num=2;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0xaf){flag_time=1;rec_num=3;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0x84){flag_time=1;rec_num=4;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0x8e){flag_time=1;rec_num=5;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0xad){flag_time=1;rec_num=6;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0xa1){flag_time=1;rec_num=7;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0xa9){flag_time=1;rec_num=8;dis_mode=1;IrValue[1]=0x00;}
         else if(IrValue[1]==0xa5){flag_time=1;rec_num=9;dis_mode=1;IrValue[1]=0x00;}   
         
         if(IrValue[1]==0xa0)
         {
           
            dis_mode=!dis_mode;
            flag_time=1;
            IrValue[1]=0x00;        
         }
         if(IrValue[1]==0xa2)
         {
            t=sec=mins=hour=0;
            flag_time=1;
            TIMSK=0x00;
            memset(rec,0,sizeof(rec));
            dis_mode=1;
            rec_i=0;
            IrValue[1]=0x00;
         }
         
         switch(dis_mode) 
         {  
            case 0: normal_display(t,sec,mins,hour);break; 
            case 1: normal_display(rec[rec_num][3],rec[rec_num][2],rec[rec_num][1],rec[rec_num][0]);break;
            case 2: break;
         }  
}

unsigned char Caculate(unsigned char y,unsigned char mon,unsigned char d)
{
  if (mon == 1 || mon == 2)
      {
          c0 = (y - 1) / 100;
          y0 = (y % 100) % 100; 
          mon0 = mon + 12;
          d0 = d;
      }
      else
      {
      c0 = y / 100;
      y0 = y % 100;
      mon0 = mon;
      d0 = d;
      }      
      
      iWeek = y + y / 4 + c0 / 4 - 2 * c0 + 26 * (mon0 + 1) / 10 + d0 - 1; //蔡勒公式
      iWeek = iWeek > 0 ? (iWeek % 7) : (iWeek % 7 + 7);   
      if (iWeek == 0)
      {
       iWeek = 7;
      }
      
      return iWeek;




}

void year_display(void)
{
      rtc_get_date(&d,&mon,&y); //显示时间   
      iWeek = Caculate(y,mon,d);
      if (iWeek == 1) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Mon.");
      }     
      if (iWeek == 2) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Tue.");
      } 
      if (iWeek == 3) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Wed.");
      } 
      if (iWeek == 4) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Thu.");
      } 
      if (iWeek == 5) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Fri.");
      } 
      if (iWeek == 6) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sat.");
      }      
      if (iWeek == 7) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sun.");
      }                           
      yh=y/10;
      yl=y%10;
      monh=mon/10;
      monl=mon%10;
      dh=d/10;
      dl=d%10;
      lcd_gotoxy(0,0);
      lcd_putsf("20");
      lcd_putchar(NUM[yh]);
      lcd_putchar(NUM[yl]);
      lcd_putchar('-');
      lcd_putchar(NUM[monh]);
      lcd_putchar(NUM[monl]);
      lcd_putchar('-');
      lcd_putchar(NUM[dh]);
      lcd_putchar(NUM[dl]);
      
      rtc_get_time(&h,&m,&s);
      hh=h/10;
      hl=h%10;
      mh=m/10;
      ml=m%10;
      sh=s/10;
      sl=s%10;
      lcd_gotoxy(1,1);
      lcd_putchar(NUM[hh]);
      lcd_putchar(NUM[hl]);
      lcd_putchar(':');
      lcd_putchar(NUM[mh]);
      lcd_putchar(NUM[ml]);
      lcd_putchar(':');
      lcd_putchar(NUM[sh]);
      lcd_putchar(NUM[sl]); 
      
      if(i==1)                                 //设置年
    {          
    iWeek = Caculate(y,mon,d);
    lcd_gotoxy(11,0);
      iWeek = Caculate(y,mon,d);
      if (iWeek == 1) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Mon.");
      }     
      if (iWeek == 2) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Tue.");
      } 
      if (iWeek == 3) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Wed.");
      } 
      if (iWeek == 4) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Thu.");
      } 
      if (iWeek == 5) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Fri.");
      } 
      if (iWeek == 6) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sat.");
      }      
      if (iWeek == 7) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sun.");
      } 
    lcd_gotoxy(0,0);
    lcd_putsf("    ");                         //闪烁
    delay_ms(twinkle_time);
    lcd_gotoxy(0,0);
    lcd_putsf("20");
    lcd_putchar(NUM[yh]);
    lcd_putchar(NUM[yl]);
    delay_ms(twinkle_time);  
    y=y+j;
    if(y>20)
    {
         y=15;
    } 
    rtc_set_date(d,mon,y);   
    j=0;                                     //按一次pd3加一次时间,否则时间不改变
    }
     if(i==2)
    { 
    iWeek = Caculate(y,mon,d);
    lcd_gotoxy(11,0);
      iWeek = Caculate(y,mon,d);
      if (iWeek == 1) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Mon.");
      }     
      if (iWeek == 2) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Tue.");
      } 
      if (iWeek == 3) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Wed.");
      } 
      if (iWeek == 4) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Thu.");
      } 
      if (iWeek == 5) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Fri.");
      } 
      if (iWeek == 6) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sat.");
      }      
      if (iWeek == 7) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sun.");
      } 
    lcd_gotoxy(5,0);
    lcd_putsf("  "); 
    delay_ms(twinkle_time);
    lcd_gotoxy(5,0);
    lcd_putchar(NUM[monh]);
    lcd_putchar(NUM[monl]);
    delay_ms(twinkle_time);  
    mon=mon+j;
    if(mon>12)
    {
         mon=1;
    }
    if(mon==0)
    {
         mon=12;
    }
    rtc_set_date(d,mon,y);  
    j=0;
    }
    if(i==3)
    {      
    iWeek = Caculate(y,mon,d);
    lcd_gotoxy(11,0);
      iWeek = Caculate(y,mon,d);
      if (iWeek == 1) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Mon.");
      }     
      if (iWeek == 2) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Tue.");
      } 
      if (iWeek == 3) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Wed.");
      } 
      if (iWeek == 4) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Thu.");
      } 
      if (iWeek == 5) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Fri.");
      } 
      if (iWeek == 6) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sat.");
      }      
      if (iWeek == 7) 
      {        
       lcd_gotoxy(11,0);    
       lcd_putsf(" Sun.");
      } 
    lcd_gotoxy(9,0);
    lcd_putsf("  "); 
    delay_ms(twinkle_time);
    lcd_gotoxy(8,0);
    lcd_putchar(NUM[dh]);
    lcd_putchar(NUM[dl]);
    delay_ms(twinkle_time);   
    d=d+j;
     if(d>31)
       d=1;
     if(d==0)
       d=31;
    rtc_set_date(d,mon,y);   
    j=0;
    }
     if(i==4)
    {  
    lcd_gotoxy(1,1);
    lcd_putsf("  "); 
    delay_ms(twinkle_time);
    lcd_gotoxy(1,1);
    lcd_putchar(NUM[hh]);
    lcd_putchar(NUM[hl]);
    delay_ms(twinkle_time);
    if(h==0&&j==-1)
    {
         h=23;
         j=0;
    }
    h=h+j;
    if(h>23)
    {
         h=0;
    }
    rtc_set_time(h,m,s);  
    j=0;
    }
    if(i==5)
    { 
    lcd_gotoxy(4,1);
    lcd_putsf("  "); 
    delay_ms(twinkle_time);
    lcd_gotoxy(4,1);
    lcd_putchar(NUM[mh]);
    lcd_putchar(NUM[ml]);
    delay_ms(twinkle_time);
    if(m==0&&j==-1)
    {
         m=59;
         j=0;
    }
    m=m+j;
    if(m>59)
    {
         m=0;
    }
    
    rtc_set_time(h,m,s);  
    j=0;
    }
    if(i==6)
    {  
    lcd_gotoxy(7,1);
    lcd_putsf("  "); 
    delay_ms(twinkle_time);
    lcd_gotoxy(7,1);
    lcd_putchar(NUM[sh]);
    lcd_putchar(NUM[sl]);
    delay_ms(twinkle_time);
    if(s==0&&j==-1)
    {
         s=59;
         j=0;
    }    
    s=s+j;
    if(s>59)
    {
     s=0;
    }
    rtc_set_time(h,m,s);  
    j=0;
    } 
     if(i==0)
     {
       lcd_gotoxy(12,1);
      lcd_putsf("   ");
     }

}
      
void year_set(void)
{       
        year_display(); 
        if(IrValue[1]==0xa0)     
        {
          i++;  
          if (i>6) i=1; 
          IrValue[1]=0x00;            
        }
        if(IrValue[1]==0xa2)     
        {
          i--;  
          if (i==0) i=6; 
          IrValue[1]=0x00;            
        }
        if(IrValue[1]==0x0a)     
        {
          if(i!=0)
          j=1;
          IrValue[1]=0x00;            
        }
        if(IrValue[1]==0x03)     
        {
          if(i!=0)
          j=-1;
          IrValue[1]=0x00;            
        }
        if(IrValue[1]==0x21)     
        {
          i=0;
          IrValue[1]=0x00; 
           
        }
}

void OC1_INT(void)
{
    DDRD|=0X30;
    PORTD=0X00;
    TCCR1A|=0X50;
    TCCR1B|=0X0A; //8分频
    TCNT1=0X0000;
    OCR1A=0x0000;
}
 
void temp_display()
{
     temp=ds18b20_temperature(0);
     lcd_gotoxy(2,0);
     lcd_puts("TEMP");    
     lcd_gotoxy(9,0);
     ftoa(temp,2,str);
     lcd_puts(str);
     lcd_puts("C");
     lcd_gotoxy(1,1);
     fa=temp*9/5+32;
     ftoa(fa,2,str);
     lcd_puts(str);
     lcd_puts("F");
     the=temp+273.15;
     lcd_gotoxy(9,1);
     ftoa(the,2,str);
     lcd_puts(str);
     lcd_puts("K");
     
}

void Music(flash int *p_Music) 
{ 
  while(*p_Music!=0xFF)         //0xFF为音乐结尾符 
    { 
      
      TCCR1A|=0X50;                    
      OCR1A=ToneTable[*p_Music];         //取音调频率 
                     //将频率值对应的计数值写入计时器,开始发声 
      p_Music++;            //乐谱音符指针+1 ,取拍数
      delay_ms((*p_Music)*80);          //按拍数延时
      delay_ms((*p_Music)*80);
      TCCR1A=0x00;                  //发声结束           
      p_Music++;                  //乐谱音符指针+1 ，取下一音符 
      if(IrValue[1]==0xa3)     
        { 
          
         
          IrValue[1]=0x00;
          break;           
        }
    }
    delay_ms(1000);                  //曲谱结束,等待 
                                               
}
  
void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: CTC top=OCR0
// OC0 output: Disconnected
TCCR0=0x0B;
TCNT0=0x05;
OCR0=0xFA;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: On
// INT1 Mode: Falling Edge
// INT2: Off
GICR|=0xC0;
MCUCR=0x0A;
MCUCSR=0x00;
GIFR=0xC0;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// USART disabled
UCSRB=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// DS1302 Real Time Clock initialization
// Trickle charger: On
// Trickle charge resistor: 8K
// Trickle charge diode(s): 1
rtc_init(1,1,3);

// 1 Wire Bus initialization
// 1 Wire Data port: PORTC
// 1 Wire Data bit: 7
// Note: 1 Wire port settings must be specified in the
// Project|Configure|C Compiler|Libraries|1 Wire IDE menu.
w1_init();

// Alphanumeric LCD initialization
// Connections specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 2
// D4 - PORTD Bit 4
// D5 - PORTD Bit 5
// D6 - PORTD Bit 6
// D7 - PORTD Bit 7
// Characters/line: 16
lcd_init(16);

// Global enable interrupts
#asm("sei")
DDRB=0xff;
IrValue[1]=0;
lcd_gotoxy(0,0);
lcd_puts("Music~~");
while(debug!=-1)
{   
    DDRD.5=1;
    OC1_INT();
    switch(debug) 
    {
        case 0:  Music(MusicTable3);break;
        case 1:  Music(MusicTable2);break;
        case 2:  Music(MusicTable1);break;
    }
    debug--;               
}
while (1)
      {
        
            switch(mode) 
            {
            case 0:  sec_display();break;
            case 1:  year_set();break;
            case 2:  temp_display();break;
            //case 2:  clock_set();break;
            }
            if(IrValue[1]==0x23)
            {   
                i=0;
                dis_mode=0;
                flag_time1=1;
                flag_time=1;
                mode++;
                lcd_clear();                      
                IrValue[1]=0x00;        
            }
            if(IrValue[1]==0x22)
            {   
                i=0;
                dis_mode=0;                
                flag_time1=1;
                flag_time=1;
                mode--; 
                lcd_clear();                     
                IrValue[1]=0x00;        
            } 



            if(mode==3)mode=0;
            if(mode==-1)mode=2;
            
      }
}
