/*********************************************************************************************************//**
 * @file    ebi_lcd_7.h
 * @version V1.0
 * @date    09/03/2013 ...
 * @brief   The header file of ebi_lcd.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __EBI_LCD_H
#define __EBI_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/

/* LCD Registers */
#define REG0             0x00
#define REG1             0x01
#define REG2             0x02
#define REG3             0x03
#define REG4             0x04
#define REG5             0x05
#define REG6             0x06
#define REG7             0x07
#define REG8             0x08
#define REG9             0x09
#define REG10            0x0A
#define REG12            0x0C
#define REG13            0x0D
#define REG14            0x0E
#define REG15            0x0F
#define REG16            0x10
#define REG17            0x11
#define REG18            0x12
#define REG19            0x13
#define REG20            0x14
#define REG21            0x15
#define REG22            0x16
#define REG23            0x17
#define REG24            0x18
#define REG25            0x19
#define REG26            0x1A
#define REG27            0x1B
#define REG28            0x1C
#define REG29            0x1D
#define REG30            0x1E
#define REG31            0x1F
#define REG32            0x20
#define REG33            0x21
#define REG34            0x22
#define REG36            0x24
#define REG37            0x25
#define REG40            0x28
#define REG41            0x29
#define REG43            0x2B
#define REG45            0x2D
#define REG48            0x30
#define REG49            0x31
#define REG50            0x32
#define REG51            0x33
#define REG52            0x34
#define REG53            0x35
#define REG54            0x36
#define REG55            0x37
#define REG56            0x38
#define REG57            0x39
#define REG59            0x3B
#define REG60            0x3C
#define REG61            0x3D
#define REG62            0x3E
#define REG63            0x3F
#define REG64            0x40
#define REG65            0x41
#define REG66            0x42
#define REG67            0x43
#define REG68            0x44
#define REG69            0x45
#define REG70            0x46
#define REG71            0x47
#define REG72            0x48
#define REG73            0x49
#define REG74            0x4A
#define REG75            0x4B
#define REG76            0x4C
#define REG77            0x4D
#define REG78            0x4E
#define REG79            0x4F
#define REG80            0x50
#define REG81            0x51
#define REG82            0x52
#define REG83            0x53
#define REG96            0x60
#define REG97            0x61
#define REG106           0x6A
#define REG118           0x76
#define REG128           0x80
#define REG129           0x81
#define REG130           0x82
#define REG131           0x83
#define REG132           0x84
#define REG133           0x85
#define REG134           0x86
#define REG135           0x87
#define REG136           0x88
#define REG137           0x89
#define REG139           0x8B
#define REG140           0x8C
#define REG141           0x8D
#define REG143           0x8F
#define REG144           0x90
#define REG145           0x91
#define REG146           0x92
#define REG147           0x93
#define REG148           0x94
#define REG149           0x95
#define REG150           0x96
#define REG151           0x97
#define REG152           0x98
#define REG153           0x99
#define REG154           0x9A
#define REG157           0x9D
#define REG192           0xC0
#define REG193           0xC1
#define REG227           0xE3
#define REG229           0xE5
#define REG231           0xE7
#define REG239           0xEF

/* LCD color (RGB565) */
#define LCD_COLOR_WHITE				0xFFFF
#define LCD_COLOR_BLACK				0x0000
#define LCD_COLOR_GREY				0xF7DE
#define LCD_COLOR_RED				0xF800
#define LCD_COLOR_GREEN				0x07E0
#define LCD_COLOR_BLUE				0x001F
#define LCD_COLOR_MAGENTA			0xF81F
#define LCD_COLOR_CYAN				0x7FFF
#define LCD_COLOR_YELLOW			0xFFE0
#define LCD_COLOR(R, G, B)			((((R)& 0x00F8) << 8) | \
									(((G) & 0x00FC) << 3) | \
									(((B) & 0x00F8) >> 3))  


/* Exported functions --------------------------------------------------------------------------------------*/
void LCD_Init(void);
void LCD_Config(void);
void LCD_Clear(u16 Color);
void LCD_FillRect(u16 x, u16 y, u16 width, u16 height, u16 color);
void LCD_BL_DutySet(u8 num);
u16 LCD_GetScanLine(void);
void LCD_ShowString(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, uc8 *Sptr);
void LCD_BackColorSet(u16 Color);
void LCD_TextColorSet(u16 Color);
void LCD_DrawCheckerPattern(short blue, short green, short red);

#endif /* __EBI_LCD_H ---------------------------------------------------------------------------------*/

