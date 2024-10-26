/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口1功能的源文件               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "usart1.h"     //包含需要的头文件


volatile unsigned char USART1_States = 0;


volatile unsigned char USART1_RX_Tim = 0;

volatile unsigned char USART1_RXD[USART1_RXD_SIZE];
volatile unsigned short USART1_RXD_Index;


/*-------------------------------------------------*/
/*函数名：初始化串口1发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART1_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量

	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量



	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3

      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //使能串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //准备设置PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口1的发送
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA9
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //准备设置PA10 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口1的接收
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA10
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
  
  USART_Init(USART1, &USART_InitStructure);                                      //设置串口1	




	USART_ClearFlag(USART1, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启接收中断
	
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口1中断
 

	USART_Cmd(USART1, ENABLE);                              //使能串口1
}



void Send_Hex(unsigned char *Data,unsigned short Length)
{
	
	unsigned short i;
	
	
	for(i=0;i<Length;i++)
	{
		/* Place your implementation of fputc here */
		/* e.g. write a character to the USART */
		USART_SendData(USART1, *Data++);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{}
	}
		
}



//下面这一块是编写的命令数据，单片机与音乐模块的通信是串口通信，发送对应的命令就可以控制模块播放音乐



//上面发送的命令都是固定的，但是对于调节音量，数据是变化的所以就要计算校验和
void DoSum(unsigned char *Str,unsigned char len)//校验位计算
{
	unsigned int xorsum = 0;
	unsigned char i;

	for(i=1;i<len;i++)
	{
		xorsum = xorsum + Str[i];
	}
	xorsum = 0 - xorsum;
	*(Str+i)     = (unsigned char)(xorsum >> 8);
	*(Str+i+1)   = (unsigned char)(xorsum & 0x00ff);
}
void Music_Vol(unsigned char Vol)
{
	unsigned char Table[10];
	Table[0] = 0x7E;
	Table[1] = 0xFF;
	Table[2] = 0x06;

	Table[3] = 0x06; //指令
	Table[4] = 0x00;
	Table[5] = 0x00;
	Table[6] = Vol;//音量
	DoSum(Table,7);//计算校验码	
	Table[9] = 0xEF;//结束码
	
	Send_Hex(Table,10);//发送指令数据
}


void Music_DOWN(void)	   //下一首命令
{
	unsigned char Table[10];   //定义一个数组
	Table[0]= 0x7E;
	Table[1]= 0xFF;
	Table[2]= 0x06;
	Table[3]= 0x01;
	Table[4]= 0x00;
	Table[5]= 0x00;
	Table[6]= 0x00;
	Table[7]= 0xFE;
	Table[8]= 0xFA;
	Table[9]= 0xEF;	   //将命令赋值给数组

	Send_Hex(Table,10);	 //扔给发送函数
}
void Music_UP(void)	    //上一首
{
	unsigned char Table[10];
	Table[0]= 0x7E;
	Table[1]= 0xFF;
	Table[2]= 0x06;
	Table[3]= 0x02;
	Table[4]= 0x00;
	Table[5]= 0x00;
	Table[6]= 0x00;
	Table[7]= 0xFE;
	Table[8]= 0xF9;
	Table[9]= 0xEF;

	Send_Hex(Table,10);
}


void Music_STOP(void)   //停止播放命令
{
	unsigned char Table[10];
	Table[0]= 0x7E;
	Table[1]= 0xFF;
	Table[2]= 0x06;
	Table[3]= 0x0E;
	Table[4]= 0x00;
	Table[5]= 0x00;
	Table[6]= 0x00;
	Table[7]= 0xFE;
	Table[8]= 0xED;
	Table[9]= 0xEF;

	Send_Hex(Table,10);
}
void Music_Play(void)	//开始播放命令
{
	unsigned char Table[10];
	Table[0]= 0x7E;
	Table[1]= 0xFF;
	Table[2]= 0x06;
	Table[3]= 0x0D;
	Table[4]= 0x00;
	Table[5]= 0x00;
	Table[6]= 0x00;
	Table[7]= 0xFE;
	Table[8]= 0xEE;
	Table[9]= 0xEF;

	Send_Hex(Table,10);
}


void USART1_IRQHandler(void)
{
	unsigned char a;
	if(USART_GetFlagStatus(USART1,USART_FLAG_TC))  //发送完成入口
	{
		USART_ClearFlag(USART1,USART_FLAG_TC); //清楚发送完成标志
		
		
		
		
	}	
	else if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))//接收数据中断
	{       
		  USART_ClearFlag(USART1,USART_FLAG_RXNE);  //清除接收数据中断标志
		
			 a = USART_ReceiveData(USART1);	  //接收数据
		
		   USART1_RX_Tim = USART1_RX_TIM_CONST;
		
			if(USART1_States &(1 << USART1_RXD_END))
			{
				//上一帧数据没有处理完，不接受新数据
			}
			else
			{
				
				USART1_RXD[USART1_RXD_Index++ % USART1_RXD_SIZE] = a;
			}	
		}
		else
		{
			 USART_ClearFlag(USART1,USART_FLAG_RXNE); //清空接收完成中断
						
			 USART_ClearFlag(USART1,USART_FLAG_TC);	  //清空发送完成中断
			
			 if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  //串口接收 溢出
			 {
				 USART_ClearFlag(USART1,USART_FLAG_ORE);
	 
			 }	

	  }
						
			
}









