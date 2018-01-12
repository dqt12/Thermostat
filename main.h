
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
//#include "board.h"
#include "lcd_display.h"
/* Setting -----------------------------------------------------------------------------------------------*/

#define DEMO_BOARD_HT32DVB    0
#define DEMO_BOARD_ELSONIC    1   
#define DEMO_BOARD_SEL        DEMO_BOARD_HT32DVB

#define EEPROM_ADDRESS        0x51
   
/* Exported constants --------------------------------------------------------------------------------------*/
	 extern vu8  URRxFin ;

//void Display_State_Clear(void);
//void Display_State(char *Str);

//extern u8 Thermostat_DATA[12];

	 
	 
	 
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

typedef struct
{
  u16 Kold;
  u16 Know;
}KeyCmd_TypeDef;

typedef enum
{
	READ_X,
	READ_Y,
	READ_Z,
	READ_FINE,
} Touch_Screen_Enum;


extern KeyCmd_TypeDef KeyCmd;

void Read_X(void);
void Read_Y(void);
void Read_Z(void);
extern Touch_Screen_Enum Flag_LINE;
/* Exported variables --------------------------------------------------------------------------------------*/
extern UI_T gUI;

/* Exported macro ------------------------------------------------------------------------------------------*/

//#define KEY1_STATE_READ()             IN_BB(GPIOD_ID, 1)//D
//#define KEY2_STATE_READ()             IN_BB(GPIOD_ID, 2)//D

/* Exported functions --------------------------------------------------------------------------------------*/
//void TempertureInit(void);
//void TempertureHandler(u16 adr);
//void TouchKeyHandler(void);
void Demo_part(LCD_DISPLAY_FrameInfoTypeDef* qq);
//void Demo_full(void);
//void Demo0(void);
//void Demo1(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H -----------------------------------------------------------------------------------*/
