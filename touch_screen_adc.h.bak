/*
 *
 *  Copyright (C) 2018/1/22

 * Basic program for touchscreen library.
 */
 
#ifndef __TSLIB_H_  
#define __TSLIB_H_  

#include "ht32.h"
#include "lcm.h"
#include "spi_flash.h"
#include "main.h"

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#define LCD_XSIZE          (480)       /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE          (272)       /* Y-resolution of LCD, Logical coor. */
	
	
#define TOUCH_USE						(1)	
#define TOUCH_NOUSE					(0)	
	
//Setting the touch calibration var for save	 
#define USE_TOUCH_CAL_SAVE			(TOUCH_USE)
#define Cal_Var_address   			0X3FFF000
#define Touch_Cal_Var_Save(X)		SPI_FLASH_BufferWrite((u8*)&X, Cal_Var_address, sizeof(X));
#define Touch_Cal_Var_Read(X)		SPI_FLASH_BufferRead((u8*)&X, Cal_Var_address, sizeof(X));
#define	Touch_Cal_Var_Erase()		SPI_FLASH_SectorErase(Cal_Var_address);
	
//Setting the touch screen interrupt ,if no use the interrupt flag , lib use for measure data;	
#define USE_TOUCH_PRESS_INT			(TOUCH_NOUSE)
#define	TOUCH_PRESS							(1)
#define	TOUCH_NOPRESS						(0)
#define	TOUCH_PRESS_INT 			  ADC_READ_INT()

//Setting the touch screen ADC MAX&MIN data,and ADC measure data;	
#define MIN_AD_X  40
#define MAX_AD_X  4080
#define MIN_AD_Y  40
#define MAX_AD_Y  4080

#define TOUCH_X_MeasureX   	ADC_READ_X
#define TOUCH_X_MeasureY		ADC_READ_Y	

//Setting the Display functions 
#define	Touch_Display_Piont(X,Y)			LCD_DrawFillRect(x-3, y-3, 6, 6,Red)
#define	Touch_Display_MSG(X,Y,MSG)		LCD_TextColorSet(Red);\
																			LCD_DrawString(X, Y, 16, 8, 0,MSG)
#define	Touch_Display_Clear()					LCD_Clear(White)


typedef struct 
{
		int cal[6];
		int scaling;
		int checksum;
	
} TOUCH_CAL_TypeDef;
	
	
typedef struct 
{
    u16 x[5];
    u16 y[5];
    u16 xfb[5];
    u16 yfb[5];
	  TOUCH_CAL_TypeDef Var;
} TOUCH_Calibration_TypeDef;
	
typedef struct
{
		bool isPress;
    u16 xPhys;
		u16 yPhys;
    u16 x;
		u16 y;
} TOUCH_XY_TypeDef;


extern	TOUCH_CAL_TypeDef	Touch_Cal;
extern	TOUCH_XY_TypeDef  Tocuh;	

bool TOUCH_SCREEN_INIT(ControlStatus force_cal);
bool TOUCH_Calibration(TOUCH_Calibration_TypeDef *pcal);
bool Calculation_calibration(TOUCH_Calibration_TypeDef *pcal);


bool TOUCH_MeasureXY(u16 *x, u16 *y);
//bool TOUCH_CheckPressed(u16 xPhys,u16 yPhys);
bool TOUCH_CheckPressed(void);

void TOUCH_Logical_Coor_Get(TOUCH_XY_TypeDef *tocuh);

void DelayT(u32 count);




#ifdef __cplusplus
}
#endif



#endif  










