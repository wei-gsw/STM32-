/******************************************************************
**	  ºìÁú¿ª·¢°å£¨V1.0£©
**	  TIMÅäÖÃÎÄ¼þ
**
**	  ÂÛ    Ì³£ºbbs.openmcu.com
**	  Íú    ±¦£ºwww.openmcu.com
**	  ÓÊ    Ïä£ºsupport@openmcu.com
**
**    °æ    ±¾£ºV1.0
**	  ×÷    Õß£ºFXL
**	  Íê³ÉÈÕÆÚ:	2012.7.25
********************************************************************/
#include "TimTest.h"



/********************************************************************************************
*º¯ÊýÃû³Æ£ºvoid GpioLed_Init(void)
*
*Èë¿Ú²ÎÊý£ºÎÞ
*
*³ö¿Ú²ÎÊý£ºÎÞ
*
*¹¦ÄÜËµÃ÷£ºledµÆ³õÊ¼»¯ÅäÖÃ
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
	  GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;              //Ñ¡Ôñbeep
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 	 //¹Ü½ÅÆµÂÊÎª50MHZ
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //Ä£Ê½ÎªÍÆÍìÊä³ö
  	GPIO_Init(GPIOC, &GPIO_InitStructure);           //³õÊ¼»¯led3¼Ä´æÆ÷
		
	  GPIOC->BSRR = 0;
    GPIOC->ODR |= GPIO_Pin_15; //Ï¨ÃðLED
}

/********************************************************************************************
*º¯ÊýÃû³Æ£ºvoid TimCounterInit(void)
*
*Èë¿Ú²ÎÊý£ºÎÞ
*
*³ö¿Ú²ÎÊý£ºÎÞ
*
*¹¦ÄÜËµÃ÷£ºTIM¼ÆÊý³õÊ¼»¯ÅäÖÃ
*	       TIMCLK = 72 MHz, Prescaler = TIM_PSC, TIM counter clock = TIMCLK/TIM_COUNTER MHz
*******************************************************************************************/
void TimCounterInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	Gpio_Init();               //IO³õÊ¼»¯

	/* TIMÊ±ÖÓÊ¹ÄÜ */
    if ((TIM == TIM1) || (TIM == TIM8))
	    RCC_APB2PeriphClockCmd(RCC_APB_TIM, ENABLE);
    else if((TIM == TIM2) || (TIM == TIM3) || (TIM == TIM4) || (TIM == TIM5))
	    RCC_APB1PeriphClockCmd(RCC_APB_TIM, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 5;					  //¼ÆÊý¸öÊý
	TIM_TimeBaseStructure.TIM_Prescaler = 36000;                     //·ÖÆµÏµÊ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//????:???(??)??????????
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//????
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//ÖØ¸´¼ÆÊýÖµ
	
	TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);				//³õÊ¼»¯TIM
	
	TIM_ITConfig(TIM,TIM_IT_Update,ENABLE);						//´ò¿ª ¸üÐÂÊÂ¼þ ÖÐ¶Ï
	TIM_Cmd(TIM, ENABLE);										//Ê¹ÄÜTIM
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
//		LED_TURN;   //LED·­×ª;
//	}
//	else
//	{
//		LED_ON;
//	}

//}
//}


/********************************************************************************************
*º¯ÊýÃû³Æ£ºvoid Tim_IRQ(void)
*
*Èë¿Ú²ÎÊý£ºÎÞ
*
*³ö¿Ú²ÎÊý£ºÎÞ
*
*¹¦ÄÜËµÃ÷£ºTIMÖÐ¶Ï´¦Àíº¯Êý
*******************************************************************************************/
void Tim_IRQ(void)
{
	static unsigned char i = 0;
	static volatile unsigned char Time_100ms_Cnt = 0;
	static volatile unsigned char Time_1s_Cnt = 0;
	static volatile unsigned char Time_10s_Cnt = 0;
	

   if(TIM_GetITStatus(TIM, TIM_IT_Update) != RESET)					//ÅÐ¶ÏÊÇ·ñÎª ¸üÐÂÊÂ¼þ ±êÖ¾Î»
	{
	   TIM_ClearITPendingBit(TIM, TIM_IT_Update);						//Çå³ý 	¸üÐÂÊÂ¼þ ±êÖ¾

		
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
