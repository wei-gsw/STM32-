#ifndef _TIM1TEST_H_
#define _TIM1TEST_H_


#include "main.h"

/*TIM使用声明*/
//#define TIM             TIM1					  //TIM声明
//#define RCC_APB_TIM     RCC_APB2Periph_TIM1	      //TIM时钟使能
#define TIM             TIM2					    //TIM声明
#define RCC_APB_TIM     RCC_APB1Periph_TIM2	    //TIM时钟使能




#define LED_OFF   GPIOC->ODR |= GPIO_Pin_13;
#define LED_ON    GPIOC->ODR &= ~GPIO_Pin_13;


extern volatile unsigned char Time_10ms_Flag;
extern volatile unsigned char Time_100ms_Flag;
extern volatile unsigned char Time_1s_Flag;
extern volatile unsigned char Time_10s_Flag;



void TimCounterInit(void);
void Tim_IRQ(void);

#endif


