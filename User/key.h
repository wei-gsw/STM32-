#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#include "TimTest.h"




#define Flash_Address 0x08004F00


extern uint8_t flag;

extern volatile unsigned char KEY_Value;
#define KEY_NOP       0
#define KEY_A     		1
#define KEY_B      		2
#define KEY_C   			3
#define KEY_D   			4
#define KEY_E   			5





void KEY_Init(void);//IO≥ı ºªØ
void KEY_Scan(void);

void KEY_Processing(void);


#endif
