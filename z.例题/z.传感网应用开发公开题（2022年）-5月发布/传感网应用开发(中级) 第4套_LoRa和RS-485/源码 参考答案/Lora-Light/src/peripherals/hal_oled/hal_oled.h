/***********************************************************************************************************
*
*
*
*
*
*
***********************************************************************************************************/
#ifndef _HAL_OLED_H_
#define _HAL_OLED_H_

#include "board.h"
#include "user_define.h"

#ifndef USE_OLED_RST
#define USE_OLED_RST 				false 	//û��ʹ�õ�OLED��ʾ����RST��λ��
#endif


//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  

#define OLED_CS 		PB_12
#if (USE_OLED_RST == true)
#define OLED_RST 		PB_13
#endif //(USE_OLED_RST == true)
#define OLED_DC			PB_14
#define OLED_SCLK 	PB_13
#define OLED_SDIN 	PB_15

//�ⲿ��������
extern Gpio_t OLED_CS_PIN;
#if (USE_OLED_RST == true)
extern Gpio_t OLED_RST_PIN;
#endif //(USE_OLED_RST == true)
extern Gpio_t OLED_DC_PIN;
extern Gpio_t OLED_SCLK_PIN;
extern Gpio_t OLED_SDIN_PIN;


#define OLED_CS_Clr()  GpioWrite( &OLED_CS_PIN, 0 )//CS
#define OLED_CS_Set()  GpioWrite( &OLED_CS_PIN, 1 )

#if (USE_OLED_RST == true)
#define OLED_RST_Clr() GpioWrite( &OLED_RST_PIN, 0 )//RES
#define OLED_RST_Set() GpioWrite( &OLED_RST_PIN, 1 )
#endif //(USE_OLED_RST == true)

#define OLED_DC_Clr() GpioWrite( &OLED_DC_PIN, 0 )//DC
#define OLED_DC_Set() GpioWrite( &OLED_DC_PIN, 1 )

#define OLED_SCLK_Clr() GpioWrite( &OLED_SCLK_PIN, 0 )//CLK
#define OLED_SCLK_Set() GpioWrite( &OLED_SCLK_PIN, 1 )

#define OLED_SDIN_Clr() GpioWrite( &OLED_SDIN_PIN, 0 )//DIN
#define OLED_SDIN_Set() GpioWrite( &OLED_SDIN_PIN, 1 )
 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_Clear_Page(uint8_t page);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowNumInvertColor(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowHex(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_ShowStringInvertColor(uint8_t x,uint8_t y,uint8_t *chr);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  //_OLED_H_
	 



