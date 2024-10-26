#include "key.h"

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_0;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
 
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
  GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	
	
}


volatile unsigned char KEY_Value = KEY_NOP;



unsigned char KEY_Read(void)
{
	unsigned char a;
	
	a  = (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) <<4)+ (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) <<3)+ (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)<<2)+ (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)<<1)+ (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) );//(GPIO_ReadInputData( GPIOA)&0xC0) +
	
	return (~a)&0x1F;
}



volatile unsigned char Relay_States = 0;


void KEY_Scan(void)
{
	
	static volatile unsigned char Trg = 0;
	static volatile unsigned char Cnt = 0;
	unsigned char KeyRead = 0;

		
		KeyRead = KEY_Read();
		Trg = KeyRead ^ (KeyRead & Cnt);
		Cnt =	KeyRead;
		
		if(Trg)
		{
			switch(Trg)
			{
				case 0x10:KEY_Value = KEY_A; break;
				case 0x08:KEY_Value = KEY_B; break;
				case 0x01:KEY_Value = KEY_D; break;
				case 0x02:KEY_Value = KEY_C; break;
				case 0x04:KEY_Value = KEY_E; break;
					
				default:   KEY_Value = KEY_NOP;break;

				
			}
		}		
	
	
}

uint8_t flag = 0;

void KEY_Processing(void)
{	 

	if(KEY_Value == KEY_A)
	{
		 KEY_Value = KEY_NOP;
		
	 
		 Music_UP();
		//播放
		OLED_ShowGBK(64,16,14,16,1);
		OLED_ShowGBK(80,16,17,16,1);
		OLED_ShowGBK(96,16,16,16,1);
			OLED_Refresh_Gram();
		
		delay_ms(500);//delay_ms(1000);
		//播放
		OLED_ShowGBK(64,16,10,16,1);
		OLED_ShowGBK(80,16,11,16,1);
		OLED_ShowGBK(96,16,18,16,1);
		OLED_Refresh_Gram();
		
		
	}
	
	if(KEY_Value == KEY_B)
	{
		KEY_Value = KEY_NOP;
		
    Music_DOWN();
		OLED_ShowGBK(64,16,15,16,1);
		OLED_ShowGBK(80,16,17,16,1);
		OLED_ShowGBK(96,16,16,16,1);
		
		OLED_Refresh_Gram();
		
		delay_ms(500);//delay_ms(1000);
		//播放
		OLED_ShowGBK(64,16,10,16,1);
		OLED_ShowGBK(80,16,11,16,1);
		OLED_ShowGBK(96,16,18,16,1);
		OLED_Refresh_Gram();
		
	}	
	
	if(KEY_Value == KEY_C)
	{
		 KEY_Value = KEY_NOP;
		
			M_VOL++;			  //音量数据加
			if(M_VOL>30)M_VOL = 30;	//音量数据的限制
			Music_Vol(M_VOL);	 //发送音量数据
		
		
			sprintf(buf,"%d",(short)M_VOL);
			OLED_ShowString(64,32,buf);
			
			OLED_Refresh_Gram();
		
	}		
	
	
	if(KEY_Value == KEY_D)
	{
		 KEY_Value = KEY_NOP;
			if(M_VOL<1)M_VOL = 1;
			M_VOL--;
			Music_Vol(M_VOL);
		
			sprintf(buf,"%d",(short)M_VOL);
			OLED_ShowString(64,32,buf);
			
			OLED_Refresh_Gram();	
 
	}	
	
	
	if(KEY_Value == KEY_E)
	{
		    KEY_Value = KEY_NOP;
		
				flag++;
				 
				//Delay_ms(20);
				if(flag%2==0)	   //当等于0，发送停止命令，显示更新为停止显示
				{
					Music_STOP();//停止		   //  Mode();    //循环播放
					//停止
					OLED_ShowGBK(64,16,8,16,1);
					OLED_ShowGBK(80,16,9,16,1);
					OLED_ShowGBK(96,16,18,16,1);
					OLED_Refresh_Gram();	
				}
				else
				{

				 
					Music_Play(); 	
				//	Delay_ms(20);
				
					//播放
					OLED_ShowGBK(64,16,10,16,1);
					OLED_ShowGBK(80,16,11,16,1);
					OLED_ShowGBK(96,16,18,16,1);
					OLED_Refresh_Gram();	
					
				};
 
			OLED_Refresh_Gram();	
	}		

}
