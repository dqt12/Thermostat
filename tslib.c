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


#include "tslib.h"


#define ABS(v) ((v>0) ? v : -v)

TOUCH_CALIBRATION_TypeDef  mCurrentSettings;
TOUCH_XY_TypeDef  Tocuh;	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DelayT(u32 count)
{
	u32 i,j;
	for(i=0;i<count;i++)
		for(j=0;j<72000;j++);
}



bool perform_calibration(TOUCH_CALIBRATION_TypeDef *pcal)
{
    u8 j;
    float n, x, y, x2, y2, xy, z, zx, zy;
    float det, a, b, c, e, f, i;
    float scaling = 65536.0;
		//float scaling = 4096.0;
		int checksum = 0;
	
    // Get sums for matrix
    n = x = y = x2 = y2 = xy = 0;

    for (j = 0; j < 5; j++)
    {
        n  += 1.0;
        x  += (float)pcal->x[j];
        y  += (float)pcal->y[j];
        x2 += (float)(pcal->x[j] * pcal->x[j]);
        y2 += (float)(pcal->y[j] * pcal->y[j]);
        xy += (float)(pcal->x[j] * pcal->y[j]);
    }

    // Get determinant of matrix -- check if determinant is too small
    det = n * (x2 * y2 - xy * xy) + x * (xy * y - x * y2) + y * (x * xy - y * x2);

    if (det < 0.1 && det > -0.1)
    {
        return (FALSE);
    }
			
    // Get elements of inverse matrix
    a = (x2 * y2 - xy * xy) / det;
    b = (xy * y - x * y2) / det;
    c = (x * xy - y * x2) / det;
    e = (n * y2 - y * y) / det;
    f = (x * y - n * xy) / det;
    i = (n * x2 - x * x) / det;

    // Get sums for x calibration
    z = zx = zy = 0;

    for (j = 0; j < 5; j++)
    {
        z  += (float)pcal->xfb[j];
        zx += (float)(pcal->xfb[j] * pcal->x[j]);
        zy += (float)(pcal->xfb[j] * pcal->y[j]);
    }

    // Now multiply out to get the calibration for framebuffer x coord
    pcal->cal[0] = (int)((a * z + b * zx + c * zy) * (scaling));
    pcal->cal[1] = (int)((b * z + e * zx + f * zy) * (scaling));
    pcal->cal[2] = (int)((c * z + f * zx + i * zy) * (scaling));

    // Get sums for y calibration
    z = zx = zy = 0;

    for (j = 0; j < 5; j++)
    {
        z  += (float)pcal->yfb[j];
        zx += (float)(pcal->yfb[j] * pcal->x[j]);
        zy += (float)(pcal->yfb[j] * pcal->y[j]);
    }

    // Now multiply out to get the calibration for framebuffer y coord
    pcal->cal[3] = (int)((a * z + b * zx + c * zy) * (scaling));
    pcal->cal[4] = (int)((b * z + e * zx + f * zy) * (scaling));
    pcal->cal[5] = (int)((c * z + f * zx + i * zy) * (scaling));

    // If we got here, we're OK, so assign scaling to a[6] and return
    pcal->cal[6] = (int)scaling;

		//If we want to remmber the calibration,use for checksum
		for (j = 0; j < 7; j++)
		{
			checksum += pcal->cal[j];
		}
	
		pcal->cal[7] = checksum;
		
    return (TRUE);
}

//功能：读取触摸屏读坐标，坐标未做转换，不能直接使用
//返回：0=无效，1=有效坐标	
//说明：本函数连续采样2此，2次采样结果+-5范围才有效

bool TOUCH_MeasureXY(u16 *x, u16 *y)
{
    u16 x1, y1;
    u16 x2, y2;
    u16 i;

    x1 = TOUCH_X_MeasureX();
    y1 = TOUCH_X_MeasureY();

    for (i = 0; i < 1000; i++);

    x2 = TOUCH_X_MeasureX();
    y2 = TOUCH_X_MeasureY();

    if (ABS(x1 - x2) < 50 && ABS(y1 - y2) < 50)
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;

        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

bool check_touch_pressed(u16 xPhys,u16 yPhys)
{
	
#if 0
	LCD_ShowNum(40,180,16,0,xPhys);
	LCD_ShowNum(80,180,16,0,yPhys);
#endif
	
	if((xPhys > MIN_AD_X ) && (xPhys < MAX_AD_X)) 
	{	
		if((yPhys > MIN_AD_Y) &&(yPhys < MAX_AD_Y))
		{
			return (TRUE);
		}
	}
	
	return (FALSE); 
}

 void get_sample(TOUCH_CALIBRATION_TypeDef *cal, u8 group,u16 x,u16 y,bool isRight)
{
    u16 xPhys, yPhys;

	  LCD_DrawFillRect(x, y, 5,5,Blue);//
//		LCD_DrawFillRect(x-5, y-5, 5,5,Red);
	
		LCD_TextColorSet(Red);
	
	  if (isRight)
		{
				LCD_DrawString(x - 120, y - 8, 16, 8, 0,"PRESS_PIXEL");
		}
    else
		{
				LCD_DrawString(x + 20, y - 8, 16, 8, 0,"PRESS_PIXEL");
		}
		
    // Wait touch to release
    while (1)
    {
        if (TOUCH_MeasureXY(&xPhys, &yPhys))
        {
            if (!check_touch_pressed(xPhys, yPhys))
						{
							break;
						}
                 
        } 

				DelayT(100);
    }
    // Wait touch pressed
    while(1)
    {
        if (TOUCH_MeasureXY(&xPhys, &yPhys))
        {
            if (check_touch_pressed(xPhys, yPhys))
            {
                if (TOUCH_MeasureXY(&cal->x[group], &cal->y[group]))
								{
                    break;
								}
            }
        }
				DelayT(100);
    };

    /* Tell user to release */
		LCD_Clear(White);
	  if (isRight)
				LCD_DrawString(x - 80, y - 8, 16, 8, 0,"PRESS_PIXEL_OK");
    else
				LCD_DrawString(x + 20, y - 8, 16, 8, 0,"PRESS_PIXEL_OK");
		
		
    cal->xfb[group] = x;
    cal->yfb[group] = y;
}

/*********************************************************************
*
*       _ExecCalibration
*
**********************************************************************
*/

bool ExecCalibration(TOUCH_CALIBRATION_TypeDef *pcal)
{
    bool result;
    u16 xPhys,yPhys;
        
    /* _Calibrate upper left */
		LCD_TextColorSet(Red);
		LCD_Clear(White);
	
    // Top Left
    get_sample(pcal, 0, 50,             50,             FALSE);

    // Top Right
    get_sample(pcal, 1, LCD_XSIZE - 50, 50,             TRUE);

    // Bottom Right
    get_sample(pcal, 2, LCD_XSIZE - 50, LCD_YSIZE - 50, TRUE);

    // Bottom Left
    get_sample(pcal, 3, 50,             LCD_YSIZE - 50, FALSE);

    // Center
    get_sample(pcal, 4, LCD_XSIZE / 2,  LCD_YSIZE / 2,  FALSE);

		
    //GUI_Clear();
		LCD_Clear(White);
		
    result = perform_calibration(pcal);

    if (result)
		{
			LCD_DrawString(80, 8, 16, 8, 0,"MSG_CALIBRATION_SUCCESSFUL");
		}
    else
		{
			LCD_DrawString(80, 8, 16, 8, 0,"MSG_CALIBRATION_NOT_SUCCESSFUL");
		}
		
			LCD_DrawString(80, 30, 16, 8, 0,"MSG_PRESS_TOUCH_TO_CONTINUE");

		DelayT(200);
    // Wait the touch to continue
    while (1)
    {
        if (TOUCH_MeasureXY(&xPhys, &yPhys))
        {
            if (check_touch_pressed(xPhys, yPhys))
                break;
        }
				DelayT(10);
    }

    return (result);
}

/*********************************************************************
*
*       _StoreUnstable
*/
// void _StoreUnstable(int x, int y)
//{
//    static int _xLast = -1;
//    static int _yLast = -1;
//    int xOut, yOut;

//    if ((x != -1) &&
//        (y != -1) &&
//        (_xLast != -1) &&
//        (_yLast != -1))
//    {
//        xOut = _xLast;
//        yOut = _yLast;
//    }
//    else
//    {
//        xOut = -1;
//        yOut = -1;
//    }

//    _xLast = x;
//    _yLast = y;

//    //GUI_TOUCH_StoreUnstable(xOut, yOut); //去抖函数
//}


/*********************************************************************
*
*       TOUCH_Exec
*/
void TOUCH_Exec(TOUCH_XY_TypeDef *tocuh)
{
//    u16 xPhys, yPhys;
//    int x, y;

    if (TOUCH_MeasureXY(&tocuh->xPhys, &tocuh->yPhys))
    {
//		 if (check_touch_pressed(tocuh->xPhys, tocuh->yPhys))
     {       
			
        /* Convert values into logical values */
			 tocuh->x = (u16)((mCurrentSettings.cal[0] + mCurrentSettings.cal[1] * tocuh->xPhys +
                   mCurrentSettings.cal[2] * tocuh->yPhys) / mCurrentSettings.cal[6]);

       tocuh->y = (u16)((mCurrentSettings.cal[3] + mCurrentSettings.cal[4] * tocuh->xPhys +
                   mCurrentSettings.cal[5] * tocuh->yPhys) / mCurrentSettings.cal[6]);

        if ((tocuh->x  > LCD_XSIZE) || (tocuh->y > LCD_YSIZE))
        {
					tocuh->x = LCD_XSIZE;
					tocuh->y = LCD_YSIZE;
					
					
           // _StoreUnstable(-1, -1);
        }
        else 
        {
           // _StoreUnstable(x, y);
        }
				
				
				
			}
		 
    }
}
