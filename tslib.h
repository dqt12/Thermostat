


/*
 *  tslib/tests/ts_calibrate.c
 *
 *  Copyright (C) 2001 Russell King.
 *
 * This file is placed under the GPL.  Please see the file
 * COPYING for more details.
 *
 * $Id: ts_calibrate.c,v 1.8 2004/10/19 22:01:27 dlowder Exp $
 *
 * Basic test program for touchscreen library.
 */
 
#ifndef __TSLIB_H_  
#define __TSLIB_H_  

#include "ht32.h"
#include "lcm.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#define LCD_XSIZE          (480)       /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE          (272)       /* Y-resolution of LCD, Logical coor. */
	
#define MIN_AD_X  40
#define MAX_AD_X  4000
#define MIN_AD_Y  40
#define MAX_AD_Y  4000

#define TOUCH_X_MeasureX   	ADC_READ_X
#define TOUCH_X_MeasureY		ADC_READ_Y	
	
typedef struct 
{
    u16 x[5];
    u16 y[5];
    u16 xfb[5];
    u16 yfb[5];
		int cal[8];// cal:0~5 ; saclling:6 ;checksum:7
} TOUCH_CALIBRATION_TypeDef;
	
typedef struct
{
    u16 xPhys;
		u16 yPhys;
    u16 x;
		u16 y;
} TOUCH_XY_TypeDef;



extern	TOUCH_CALIBRATION_TypeDef  mCurrentSettings;	
extern	TOUCH_XY_TypeDef  Tocuh;	

bool perform_calibration(TOUCH_CALIBRATION_TypeDef *pcal);
bool ExecCalibration(TOUCH_CALIBRATION_TypeDef *pcal);

void get_sample(TOUCH_CALIBRATION_TypeDef *cal, u8 group,u16 x,u16 y,bool isRight);

bool check_touch_pressed(u16 xPhys,u16 yPhys);
bool TOUCH_MeasureXY(u16 *x, u16 *y);
void DelayT(u32 count);
void TOUCH_Exec(TOUCH_XY_TypeDef *tocuh);
//void _StoreUnstable(int x, int y);


#ifdef __cplusplus
}
#endif



#endif  










