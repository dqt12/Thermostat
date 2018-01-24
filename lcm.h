/*********************************************************************************************************//**
 * @file    ebi_lcd.h
 * @version V1.00
 * @date    10/01/2013
 * @brief   The header file of ebi_lcd.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __LCM_H
#define __LCM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"	 
#include "ht32_board.h"
#include "spi_flash.h"
/*----------------------------------------------------------------------------------------------------------*/
/* LCD Module Setting                                                                                       */
/*----------------------------------------------------------------------------------------------------------*/
//  <h> LCD Module Configuration
//    <o0> LCD Interface ID
//      <0=> EBI Channel 0
//      <1=> EBI Channel 1
//      <2=> EBI Channel 2
//      <3=> EBI Channel 3
#define LCM_IF_ID                 0
//    <h> LCM Reset Pin Configuration
//      <o0> GPIO PORT  <0=> GPIOA      <1=> GPIOB      <2=> GPIOC      <3=> GPIOD      <4=> GPIOE
//      <o1> GPIO PIN
//        <0=> PIN0   <1=> PIN1   <2=> PIN2   <3=> PIN3   <4=> PIN4   <5=> PIN5   <6=> PIN6   <7=> PIN7
//        <8=> PIN8   <9=> PIN9   <10=> PIN10 <11=> PIN11 <12=> PIN12 <13=> PIN13 <14=> PIN14 <15=> PIN15
//      <o2> SLEEP AFIO MODE  <0-15> 
//#define LCM_RST_GPIO_PORT_ID        1
//#define LCM_RST_GPIO_PIN_ID         11
//#define LCM_RST_AFIO_MODE_ID_SLP    0
//    </h>
//    <o0> Backlight Mode <0=> No BL Control <1=> IO Output Control <2=> PWM Control (TBD)
//    <o1> Backlight PWM Channel (TBD)
//      <0=> PWM0(GT0CH0)   <1=>  PWM1(GT0CH1)   <2=>  PWM2(GT0CH2)   <3=>  PWM3(GT0CH3)
//      <4=> PWM4(GT1CH0)   <5=>  PWM5(GT1CH1)   <6=>  PWM6(GT1CH2)   <7=>  PWM7(GT1CH3)
//      <8=> PWM8(MT0CH0)   <9=>  PWM9(MT0CH1)   <10=> PWM10(MT0CH2)  <11=> PWM11(MT0CH3)
//      <12=> PWM12(MT1CH0) <13=> PWM13(MT1CH1)  <14=> PWM14(MT1CH2)  <15=> PWM15(MT1CH3)
//    <o2> Backlight PWM Default Duty <0-1000>
//    <o3> LCD Modual Rotete 
//        <0=> Normal 
//        <1=> Y-invert 
//        <2=> X-invert
//        <3=> Y-invert & X-invert 
//        <4=> X-Y Change 
//        <5=> X-Y Change & Y-invert 
//        <6=> X-Y Change & X-invert 
//        <7=> X-Y Change & Y-invert & X-invert 


//#define LCM_BL_MODE               2
//#define LCM_BL_PWM_CH             4
//#define LCM_BL_PWM_DEFAULT_DUTY   800
//#define LCM_DIRECTION             6
//#define LCM_HP										480
//#define LCM_WP										272
//  </h>

//#define LCM_BL_DUTY_SET(duty) (HT_GPTM1->CH0CCR = duty)
//#define LCM_BL_DUTY_SET(duty) (HT_MCTM1->CH2CCR = duty)



/* Exported types ------------------------------------------------------------------------------------------*/
typedef struct
{
  vu16 EBI_LCD_REG;
	vu16 EBI_LCD_Reserved;
  vu16 EBI_LCD_RAM;
}EBI_LCD_TypeDef;


/* Exported constants --------------------------------------------------------------------------------------*/
#define LCM_IF_EBI_0            0
#define LCM_IF_EBI_1            1
#define LCM_IF_EBI_2            2
#define LCM_IF_EBI_3            3

#define LCM_NORMAL                0
#define LCM_Y_INVERT              1
#define LCM_X_INVERT              2
#define LCM_YX_INVERT             3
#define LCM_XY_EXCHANGE           4
#define LCM_XY_EXCHANGE_Y_INVERT  5
#define LCM_XY_EXCHANGE_X_INVERT  6
#define LCM_XY_EXCHANGE_YX_INVERT 7

#if   (LCM_IF_ID == LCM_IF_EBI_0)
  #define LCD_Bank_ADDR             0x60000000
  #define LCM_EBI_BANK              EBI_BANK_0
#elif (LCM_IF_ID == LCM_IF_EBI_1)
  #define LCD_Bank_ADDR             0x64000000
  #define LCM_EBI_BANK              EBI_BANK_1
#elif (LCM_IF_ID == LCM_IF_EBI_2)
  #define LCD_Bank_ADDR             0x68000000
  #define LCM_EBI_BANK              EBI_BANK_2
#elif (LCM_IF_ID == LCM_IF_EBI_3)
  #define LCD_Bank_ADDR             0x6C000000
  #define LCM_EBI_BANK              EBI_BANK_3
#else
  #error "Erro LCD IF Selection !!! In board_hardware.h" 
#endif

#define EBI_LCD                  ((EBI_LCD_TypeDef *) LCD_Bank_ADDR)

#define LCM_RST_INIT()            GPIO_DirectionConfig(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN, GPIO_DIR_OUT);	
#define LCM_RST_ACTIVE()          GPIO_ClearOutBits(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN);
#define LCM_RST_INACTIVE()        GPIO_SetOutBits(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN);




#define RGB888_EBI8 				(1)
#define RGB565_EBI16				(2)

#define LCD_CONTROL_MODE     2

#if  (LCD_CONTROL_MODE == RGB888_EBI8)
	typedef u32		ColorType ;
	#define White          0xFFFFFF
	#define Black          0x000000
	#define Grey           0x808080
	#define Blue           0x0000FF
	#define Red            0xFF0000
	#define Green          0x00FF00
	#define Yellow         0xFFFF00
#elif (LCD_CONTROL_MODE == RGB565_EBI16)
	typedef u16		ColorType ;
	#define White          0xFFFF
	#define Black          0x0000
	#define Grey           0xF7DE
	#define Blue           0x001F
	#define Blue2          0x051F
	#define Red            0xF800
	#define Magenta        0xF81F
	#define Green          0x07E0
	#define Green2         0x4593
	#define Cyan           0x7FFF
	#define Yellow         0xFFE0
#else
  #error "Erro LCD RGB MODE !!! " 
#endif


#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216

#define Horizontal     0x00
#define Vertical       0x01

typedef struct
{
	u16 Xpos;
	u16 Ypos;
	u16 XposEnd;
	u16 YposEnd;
	u16 Height;
	u16 Width;
	u16 Mode;
}LCD_DrawSize;

extern LCD_DrawSize LCD_IData;

extern ColorType Color_Text; 
extern ColorType Color_Back;  

/* Exported functions --------------------------------------------------------------------------------------*/
void LCD_Init(void);//OK
void LCD_Config(void);//OK

void LCD_DisplayOff(void);
void LCD_DisplayOn(void);

void LCD_WriteReg(u16 LCD_Reg_Index, u16 LCD_Reg_Value);//OK

void LCD_StarterSet(u16 X_Location, u16 Y_Location);//OK
void LCD_SetDisplayArea(u16 Column, u16 Page, u16 Height, u16 Width);//OK

void LCD_WriteRAMPrior(void);//OK
void LCD_WriteRAM(u16 Data);//OK
void LCD_WritePixel(ColorType Color);
void LCD_WriteRAMWord(ColorType RGB_Set);//OK

void LCD_Clear(ColorType Color);//OK
void LCD_BackColorSet(ColorType Color);//OK
void LCD_TextColorSet(ColorType Color);//OK

void LCD_DrawPoint(u16 Xpos, u16 Ypos, ColorType Color);//OK
void LCD_DrawLine(u16 X_Location, u16 Y_Location, u16 Length,u8 Direction, ColorType Color);//OK
void LCD_DrawRect(u16 X_Location, u16 Y_Location, u16 Height, u16 Width,ColorType Color);//OK
void LCD_DrawFillRect(u16 X_Location, u16 Y_Location, u16 Height, u16 Width,ColorType Color);//OK
void LCD_DrawCircle(u16 X_Location, u16 Y_Location, u16 Radius,ColorType Color);//OK
void LCD_DrawFillCircle(u16 X_Location, u16 Y_Location, u16 Radius,ColorType Color);//OK

void LCD_DrawChar(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii);//OK
void LCD_DrawString(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, char *Sptr);//OK

void LCD_ShowTemp(u16 Xpos, u16 Ypos, u8 Mode, u16 Num);//OK
void LCD_ShowNum(u16 Xpos, u16 Ypos,u8 Font_Size,u8 Mode, u16 Num);//OK

void LCD_StringDisplay(char *Sptr);
void LCD_IData_ALL(u16 Xpos, u16 Ypos, u16 XposEnd, u16 YposEnd, u16 Height, u16 Width, u8 Mode);

//null
void LCD_SetPartialDisplayArea(u16 Start, u16 End);
void LCD_NormalDisplay(void);
void LCD_PartialDisplayOn(void);
void LCD_LowPowerCmd(ControlStatus cmd);
void LCD_PicDraw(u8 X_Location, u16 Y_Location, u16 Height, u16 Width, uc8 *Pptr);

#ifdef __cplusplus
}
#endif

#endif /* __LCM_H --------------------------------------------------------------------------------------*/
