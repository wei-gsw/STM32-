/******************************************************************
**	  红龙开发板（V1.0）
**	  TIM配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.0
**	  作    者：FXL
**	  完成日期:	2012.7.25
********************************************************************/
#include "TimTest.h"



/********************************************************************************************
*函数名称：void GpioLed_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：led灯初始化配置
*******************************************************************************************/
static void Gpio_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOC, ENABLE);
  //  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  //  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);   
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
		
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;              //选择beep
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 	 //管脚频率为50MHZ
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //模式为推挽输出
  	GPIO_Init(GPIOC, &GPIO_InitStructure);           //初始化led3寄存器
		
	  GPIOC->BSRR = 0;
    GPIOC->ODR |= GPIO_Pin_15; //熄灭LED
}

/********************************************************************************************
*函数名称：void TimCounterInit(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM计数初始化配置
*	       TIMCLK = 72 MHz, Prescaler = TIM_PSC, TIM counter clock = TIMCLK/TIM_COUNTER MHz
*******************************************************************************************/
void TimCounterInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	Gpio_Init();               //IO初始化

	/* TIM时钟使能 */
    if ((TIM == TIM1) || (TIM == TIM8))
	    RCC_APB2PeriphClockCmd(RCC_APB_TIM, ENABLE);
    else if((TIM == TIM2) || (TIM == TIM3) || (TIM == TIM4) || (TIM == TIM5))
	    RCC_APB1PeriphClockCmd(RCC_APB_TIM, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 5;					  //计数个数
	TIM_TimeBaseStructure.TIM_Prescaler = 36000;                     //分频系�
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//????:???(??)??????????
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//????
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数值
	
	TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);				//初始化TIM
	
	TIM_ITConfig(TIM,TIM_IT_Update,ENABLE);						//打开 更新事件 中断
	TIM_Cmd(TIM, ENABLE);										//使能TIM
}



volatile unsigned char Time_10ms_Flag = 0;
volatile unsigned char Time_100ms_Flag = 0;
volatile unsigned char Time_1s_Flag = 0;

volatile unsigned char Time_10s_Flag = 0;


unsigned char Time_Counter = 0;



//void LED_Connect_Processing(void)
//{
//if(LED_100ms_Flag)
//{
//	LED_100ms_Flag = 0;
//	
//	if(Connect_flag)
//	{
//		LED_TURN;   //LED翻转;
//	}
//	else
//	{
//		LED_ON;
//	}

//}
//}


/********************************************************************************************
*函数名称：void Tim_IRQ(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM中断处理函数
*******************************************************************************************/
void Tim_IRQ(void)
{
	static unsigned char i = 0;
	static volatile unsigned char Time_100ms_Cnt = 0;
	static volatile unsigned char Time_1s_Cnt = 0;
	static volatile unsigned char Time_10s_Cnt = 0;
	

   if(TIM_GetITStatus(TIM, TIM_IT_Update) != RESET)					//判断是否为 更新事件 标志位
	{
	   TIM_ClearITPendingBit(TIM, TIM_IT_Update);						//清除 	更新事件 标志

		
		 Time_10ms_Flag = 1 ;
		 
		KEY_Scan();
		
		if(++Time_100ms_Cnt >= 40)
		 {
			 Time_100ms_Cnt = 0;
			 Time_100ms_Flag = 1;
//			 
//			 if(EEPROM_Delay)
//			 {
//				 EEPROM_Delay--;
//				 
//				 if(EEPROM_Delay == 0)
//				 {
//					 EEPROM_Enable = 1;
//					 
//				 }
//				 
//			 }
			// ADC_Enable = 1;
			 
			 //GPIOC->ODR ^= GPIO_Pin_15;
			 
			 if(++Time_1s_Cnt >= 10)
			 {
				 Time_1s_Cnt = 0;
				 Time_1s_Flag = 1;
				 
				// GPIOB->ODR ^= GPIO_Pin_12;
				 
				 if(++Time_10s_Cnt >= 5)
				 {
					 Time_10s_Cnt = 0;
					 Time_10s_Flag = 1;
					 
				 }
			 }
			 
		 }
		 
		
	}
}
