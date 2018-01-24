
/*********************************************************************************************************//**
 * @file   main.h
 * @version V1.00
 * @date    03/04/2014
 * @brief   The header file of main.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include <string.h>
#include "lcm.h"
#include "spi_flash.h"
#include "ESP8266.h"
#include "touch_screen.h"

// FOR USB VCP
#include "ht32_usbd_core.h"
#include "ht32_usbd_class.h"
#include "ht32_usbd_descriptor.h"
#include "iap_handler.h"
#include "_ht32_project_source.h"

//USE FOR LCD DISPLAY
#include "lcd_display.h"	 
	 
//USE FOR DEMO
extern u16 Display_State_Line ;
 void Display_Temp(void);
 void LCD_TEST(void);
 void FLASH_TEST(void);
 void Display_State(char *Str);
/* Setting -----------------------------------------------------------------------------------------------*/
   
/* Exported constants --------------------------------------------------------------------------------------*/

extern u8 Thermostat_DATA[12];

/* Exported types ------------------------------------------------------------------------------------------*/

typedef struct 
{
  u16 FilterBuffer[18];
  u8 FilterWriteIndex;
  u8 FilterReadIndex;
  u16 DebounceCount;
  u16 DebounceTime;
  u16 PreviousTemperture;
  s16 Temperture;
  s16 TempertureCompensation;
  u16 TempertureCompensationTimebaseDivider;
  vu16 TempertureCompensationTimebaseCount;
  u16 SetTemperture;
  u32 SetTempertureTimeoutCount;
  u16 RangeHigh;
  u16 RangeLow;  
  volatile bool IsTempertureChange;
  volatile bool IsSettingTemperture;
} TEMPERTURE_TypeDef;

#define UI_STATUS_DEMO1   0
#define UI_STATUS_DEMO2   1

#define UI_DEMO_STATUS_INIT       0
#define UI_DEMO_STATUS_PROCESS    1

typedef struct 
{
  u8 MachineStatus;
  u8 Demo1_Status;
  u8 Demo2_Status;
  u8 Demo1_ShowPicID;
	
	u16 num;
	u16 num1;
	char MVC[3];
	
  volatile bool IsDemo1Update;
  volatile bool IsDemo2Update;
	
  vu32 Demo1_AutoChangeCounter;

} UI_T;

extern UI_T gUI;
//typedef struct
//{
//  u16 Kold;
//  u16 Know;
//}KeyCmd_TypeDef;
//extern KeyCmd_TypeDef KeyCmd;
//---------------------------------------------------------------------

struct TEMP
{
	bool 	En;
	bool 	CorEn;
	bool 	SetEn;
	bool 	SubEn;
	bool 	AddEn;
	u16 Now;
	u16	Set;
	u16 Time;
};

extern struct TEMP Temp;

typedef struct 
{
	bool flag;
	u16 cnt;
}Time_Slice_TypeDef;

struct TIME_SLICE
{
	Time_Slice_TypeDef _10ms;
	Time_Slice_TypeDef _20ms;
	Time_Slice_TypeDef _100ms;
	Time_Slice_TypeDef _500ms;
	Time_Slice_TypeDef _1s;
};

extern struct TIME_SLICE TimeSlice;
extern u8 KEY_STATE;
extern vu16 ADC_DATA[3];

/* Exported variables --------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/

//#define KEY1_STATE_READ()             IN_BB(GPIOD_ID, 1)//D
//#define KEY2_STATE_READ()             IN_BB(GPIOD_ID, 2)//D

/* Exported functions --------------------------------------------------------------------------------------*/
//void TempertureInit(void);
//void TempertureHandler(u16 adr);
//void TouchKeyHandler(void);
//void Demo_part(LCD_DISPLAY_FrameInfoTypeDef* qq);
//void Demo_full(void);
//void Demo0(void);
//void Demo1(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H -----------------------------------------------------------------------------------*/
