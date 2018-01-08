/*********************************************************************************************************//**
 * @file   picture_index.h
 * @version V1.00
 * @date    03/04/2014
 * @brief   The header file of picture index.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "ht32.h" 
//#include "board.h"

/* Image information related typedef */
typedef __PACKED_H struct
{
  u32 ImageStartAddr; 
  u16 ImageWidth;
  u16 ImageHigh;
  u8  ImageFormat;
  u8  RESERVE[1];
} __PACKED_F LCD_DISPLAY_ImageTypeDef;

typedef __PACKED_H struct
{
  u16 Count;
  LCD_DISPLAY_ImageTypeDef *Table;
} __PACKED_F LCD_DISPLAY_ImageInfoTypeDef;

__PACKED_H typedef struct
{
  u16 Column;
  u16 Page;
} __PACKED_F LCD_DISPLAY_PositionTypeDef;

typedef struct
{
  u8 ImageCountPerFrame;
  LCD_DISPLAY_PositionTypeDef* ImagePositionTable;
} LCD_DISPLAY_FrameInfoTypeDef;

typedef __PACKED_H struct
{
  LCD_DISPLAY_FrameInfoTypeDef* pFrameInfo;
  u16 ImageStartIndex;
  u16 ImageLength;
  u16* pImageRemapTable;
  u8 FrameRate;
  u8 Mode;
  u16 TransparentColor;  
} __PACKED_F LCD_DISPLAY_InitTypedef;

typedef __PACKED_H struct
{
  /* Play Information */
  u32 ImageCounter;
} __PACKED_F LCD_DISPLAY_InfoTypedef;

#define LCD_DISPLAY_MODE_NORMAL           0
#define LCD_DISPLAY_MODE_TRANSPARENT_1    1
#define LCD_DISPLAY_MODE_TRANSPARENT_2    2

extern LCD_DISPLAY_InfoTypedef gLCD_DISPLAY;

void LCD_DISPLAY_Init(LCD_DISPLAY_InitTypedef* pDisplay);
void LCD_DISPLAY_Process(void);
void LCD_DISPLAY_TimebaseHandler(void);
void LCD_DISPLAY_GetImageInfo(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_DISPLAY_H -----------------------------------------------------------------------------------*/
