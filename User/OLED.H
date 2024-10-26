
#ifndef _OLED_H
#define _OLED_H


#include <main.h>








  
#define SCL3_0	GPIO_ResetBits(GPIOB,GPIO_Pin_8)			// Serial Clock Input
#define SDA3_0	GPIO_ResetBits(GPIOB,GPIO_Pin_9)				// Serial Data Input


#define SCL3_1	GPIO_SetBits(GPIOB,GPIO_Pin_8)					// Serial Clock Input
#define SDA3_1	GPIO_SetBits(GPIOB,GPIO_Pin_9)					// Serial Data Input





extern volatile unsigned char Error_Flag;

void OLED_ShowGBK(u8 x, u8 y,  u8 num, u8 size,u8 mode);
void LED_12_show_font(uint16_t x,uint16_t y,const uint8_t *font,uint8_t mode);

void	LCD_Init3(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_Processing(void);

void LED_DrawPoint(unsigned short x,unsigned short y,u8 t);

void OLED_ShowGBK12x8(u8 x, u8 y,  u8 num, u8 size,u8 mode);

#endif

