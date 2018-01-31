
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
void Display_WIFI(void);
void LCD_TEST(void);
void FLASH_TEST(void);
void Display_State(char *Str);
 
 
 //////////////////////////////////////////////////////////////////////////

typedef struct
{
	u8 Now_sta;
	u8 Set_sta;
}UI_Sta_TypeDef;

 
 typedef struct
{
	bool updata;
	UI_Sta_TypeDef wifi;
	UI_Sta_TypeDef page;
	bool time_updata;
	bool date_updata;
	UI_Sta_TypeDef tmpset;
	UI_Sta_TypeDef tmpsub;
	UI_Sta_TypeDef tmpadd;
	bool tempset_updata;
	bool tempnow_updata;
	
}UI_DEMO_TypeDef;

extern UI_DEMO_TypeDef gUI_DataBase;

void DUI_DEMO_INIT(void);
void DUI_TS_Scan(void);
void DUI_DISPLAY(void);

void DISPLAY_part(LCD_DISPLAY_FrameInfoTypeDef *qq,u8 PicID);
void DISPLAY_ADD(u8 PicID);
void DISPLAY_full(u8 PicID);
 //////////////////////////////////////////////////////////////////////////

/* Setting -----------------------------------------------------------------------------------------------*/
   
/* Exported constants --------------------------------------------------------------------------------------*/

extern u8 Thermostat_DATA[12];

/* Exported types ------------------------------------------------------------------------------------------*/

//typedef struct
//{
//  u16 Kold;
//  u16 Know;
//}KeyCmd_TypeDef;
//extern KeyCmd_TypeDef KeyCmd;
//---------------------------------------------------------------------

typedef struct TEMP
{
	bool 	En;
	bool  CorEn;
	bool 	SetEn;
	bool 	SubEn;
	bool 	AddEn;
	u16 Now;
	u16	Set;
	u16 Time;
	u16 TimeBuf;
}TEMP_TypeDef;

extern TEMP_TypeDef Temp;

typedef struct 
{
	bool flag;
	u16 cnt;
}Time_Slice_TypeDef;

typedef struct 
{
	Time_Slice_TypeDef _10ms;
	Time_Slice_TypeDef _20ms;
	Time_Slice_TypeDef _100ms;
	Time_Slice_TypeDef _500ms;
	Time_Slice_TypeDef _1s;
}TIME_SLICE_TypeDef;

extern TIME_SLICE_TypeDef TimeSlice;

extern u8 KEY_STATE;
extern vu16 ADC_DATA[3];


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H -----------------------------------------------------------------------------------*/
