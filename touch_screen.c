#include "touch_screen.h"

#define ABS(v) ((v>0) ? v : -v)

TOUCH_CAL_TypeDef	Touch_Cal;
TOUCH_XY_TypeDef  Tocuh;	
/*********************************************************************************************************//**
 * @brief delay sometime 
 * @param count: delay vlaue
 * @retval NONE
 ************************************************************************************************************/
void DelayT(u32 count)
{
	u32 i,j;
	for(i=0;i<count;i++)
		for(j=0;j<72000;j++);
}

/*********************************************************************************************************//**
 * @brief Calculation the Calibration Value
 * @param *pcal: Five group X_Y Value Point
 * @retval TURE: the data can calculation ;FALSE:the data was wrong
 ************************************************************************************************************/
bool Calculation_calibration(TOUCH_Calibration_TypeDef *pcal)
{
    u8 j;
    float n, x, y, x2, y2, xy, z, zx, zy;
    float det, a, b, c, e, f, i;
    float scaling = 65536.0;
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
    pcal->Var.cal[0] = (int)((a * z + b * zx + c * zy) * (scaling));
    pcal->Var.cal[1] = (int)((b * z + e * zx + f * zy) * (scaling));
    pcal->Var.cal[2] = (int)((c * z + f * zx + i * zy) * (scaling));

    // Get sums for y calibration
    z = zx = zy = 0;

    for (j = 0; j < 5; j++)
    {
        z  += (float)pcal->yfb[j];
        zx += (float)(pcal->yfb[j] * pcal->x[j]);
        zy += (float)(pcal->yfb[j] * pcal->y[j]);
    }

    // Now multiply out to get the calibration for framebuffer y coord
    pcal->Var.cal[3] = (int)((a * z + b * zx + c * zy) * (scaling));
    pcal->Var.cal[4] = (int)((b * z + e * zx + f * zy) * (scaling));
    pcal->Var.cal[5] = (int)((c * z + f * zx + i * zy) * (scaling));
		
    // If we got here, we're OK, 
    pcal->Var.scaling = (int)scaling;		
		
		
		//If we want to remmber the calibration,use for checksum
		checksum = pcal->Var.scaling;
		
		for (j = 0; j < 6; j++)
		{
			checksum += pcal->Var.cal[j];
		}
		
		pcal->Var.checksum = checksum;
		
    return (TRUE);
}

/*********************************************************************************************************//**
 * @brief get the adc data for touch screen res 
 * @param X point 
 * @param Y point	
 * @retval   TURE : adc data is OK ; FALSE :adc data is Jitter
 ************************************************************************************************************/
bool TOUCH_MeasureXY(u16 *x, u16 *y)
{
    u16 x1, y1;
    u16 x2, y2;
		u16 j;
    x1 = TOUCH_X_MeasureX();
    y1 = TOUCH_X_MeasureY();

    for(j=0;j<1000;j++);

    x2 = TOUCH_X_MeasureX();
    y2 = TOUCH_X_MeasureY();

    if (ABS(x1 - x2) < 50 && ABS(y1 - y2) < 50)
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;

        return (TRUE);
    }
    else 
    {
        return (FALSE);
    }
}

/*********************************************************************************************************//**
 * @brief check the touch screen is press ? (USE by ADC DATA POLLING)
 * @retval TURE: touch is press ;FALSE: Touch isn't press 
 ************************************************************************************************************/
//bool TOUCH_CheckPressed(void)
//{
//    u16 xPhys;
//    u16 yPhys;
//    xPhys = TOUCH_X_MeasureX();
//    yPhys = TOUCH_X_MeasureY();

//		if((xPhys > MIN_AD_X ) && (xPhys < MAX_AD_X)) 
//		{	
//			if((yPhys > MIN_AD_Y) &&(yPhys < MAX_AD_Y))
//			{
//				return (TRUE);
//			}
//		}
//	
//	return (FALSE); 
//}
bool TOUCH_CheckPressed(void)
{
#if (USE_TOUCH_PRESS_INT == TOUCH_NOUSE)
    u16 xPhys;
    u16 yPhys;
    xPhys = TOUCH_X_MeasureX();
//    yPhys = TOUCH_X_MeasureY();

		if((xPhys > MIN_AD_X ) && (xPhys < MAX_AD_X)) 
		{	
	//		if((yPhys > MIN_AD_Y) &&(yPhys < MAX_AD_Y))
			{
				return (TRUE);
			}
		}
	
	return (FALSE); 
		
#elif (USE_TOUCH_PRESS_INT == TOUCH_USE)
		
		if (TOUCH_PRESS_INT == TOUCH_PRESS)
			return (TRUE);
		
		return (FALSE); 
#endif
}




				
			
/*********************************************************************************************************//**
 * @brief get the calibration data for touch screen
 * @param *pcal: TOUCH_Calibration_TypeDef
 * @param group: the data group number
 * @param x: check the x 
 * @param y: check the y
 * @param isRight: check the in the screen direction
 * @retval NONE
 ************************************************************************************************************/
 void get_sample(TOUCH_Calibration_TypeDef *pcal, u8 group,u16 x,u16 y,bool isRight)
{
		Touch_Display_Piont(x, y);
	
	  if (isRight)
		{
				Touch_Display_MSG(x - 120, y - 8,"PRESS_PIXEL");
		}
    else
		{
				Touch_Display_MSG(x + 20, y - 8, "PRESS_PIXEL");
		}
		
    // Wait touch to release
    while (1)
    {
				if (TOUCH_CheckPressed() == FALSE)		
				{
					break;
				}				
				
				DelayT(100);
    }
		
		
    // Wait touch pressed
    while(1)
    {
				if (TOUCH_CheckPressed() == TRUE)		
				{
						if (TOUCH_MeasureXY(&pcal->x[group], &pcal->y[group]))
						{
								break;
						}
				}

				DelayT(100);
    }

    /* Tell user to release */
		Touch_Display_Clear();
		
	  if (isRight)
		{
				Touch_Display_MSG(x - 80, y - 8,"PRESS_PIXEL_OK");
		}
    else
		{
				Touch_Display_MSG(x + 20, y - 8,"PRESS_PIXEL_OK");
		}
		
    pcal->xfb[group] = x;
    pcal->yfb[group] = y;
}


/*********************************************************************************************************//**
 * @brief Tocuh Screen Main Calibration
 * @param *pcal: TOUCH_Calibration_TypeDef
 * @retval TURE:  Calibration OK ; FALSE: Calibration NOK
 ************************************************************************************************************/
bool  TOUCH_Calibration(TOUCH_Calibration_TypeDef *pcal)
{
    bool result;
        
    /* _Calibrate upper left */
		Touch_Display_Clear();
	
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
		Touch_Display_Clear();
		
    result = Calculation_calibration(pcal);

    if (result)
		{
			Touch_Display_MSG(80, 8,"TOUCH_SCREEN_CALIBRATION_SUCCESSFUL");
		}
    else
		{
			Touch_Display_MSG(80, 8,"TOUCH_SCREEN_CALIBRATION_NOT_SUCCESSFUL");
		}
		
		
		Touch_Display_MSG(100, 30, "PRESS_TOUCH_TO_CONTINUE");
			

		// Wait touch to release
    while (1)
    {
				if (TOUCH_CheckPressed() == FALSE)		
				{
					break;
				}				
				
				DelayT(200);
    }
		
    // Wait the touch to continue
    while (1)
    {
				if(TOUCH_CheckPressed() == TRUE)		
				{
					Touch_Display_Clear();
					break;
				}

				DelayT(200);
    }

    return (result);
}


void _StoreUnstable(u16 *x, u16 *y)
{
    static u16 _xLast = 0;
    static u16 _yLast = 0;	
		u16  xDiff, yDiff;
	
		xDiff = abs (*x - _xLast);
		yDiff = abs (*y - _yLast);
	
		if (xDiff + yDiff > 2) 
		{
			_xLast = *x;
			_yLast = *y;
		}
	
		*x = _xLast;
		*y = _yLast;
}



/*********************************************************************************************************//**
 * @brief TOUCH_SCREEN_INIT 
 * @param able_cal: force the Calibration, can be ENABLE or DISABLE.
 * @retval TOUCH_SCREEN_INIT 
 ************************************************************************************************************/
void TOUCH_Logical_Coor_Get(TOUCH_XY_TypeDef *tocuh)
{
	tocuh->isPress = TOUCH_CheckPressed();

	if (tocuh->isPress)
	{             
		 TOUCH_MeasureXY(&tocuh->xPhys, &tocuh->yPhys);
		/* Convert values into logical values */
		 tocuh->x = (u16)((Touch_Cal.cal[0] + Touch_Cal.cal[1] * tocuh->xPhys +
								 Touch_Cal.cal[2] * tocuh->yPhys) / Touch_Cal.scaling );

		 tocuh->y = (u16)((Touch_Cal.cal[3] + Touch_Cal.cal[4] * tocuh->xPhys +
								 Touch_Cal.cal[5] * tocuh->yPhys) / Touch_Cal.scaling );

			if ((tocuh->x  > LCD_XSIZE) || (tocuh->y > LCD_YSIZE))
			{
				tocuh->x = LCD_XSIZE;
				tocuh->y = LCD_YSIZE;
			}
			else 
			{	
				//Filer shake
				 _StoreUnstable(&tocuh->x, &tocuh->y);
			}
			
	}
		 
    
}

/*********************************************************************************************************//**
 * @brief TOUCH_SCREEN_INIT 
 * @param force_cal: force the Calibration, can be ENABLE or DISABLE.
 * @retval TOUCH_SCREEN_INIT 
 ************************************************************************************************************/
bool TOUCH_SCREEN_INIT(ControlStatus force_cal)
{
	TOUCH_Calibration_TypeDef  *Touch_Cal_Frist;		
	bool result = FALSE;
	
#if (USE_TOUCH_CAL_SAVE	== TOUCH_USE)
	
	u8 i;
	int checksum = 0;
	
	Touch_Cal_Var_Read(Touch_Cal); 
	
	checksum = Touch_Cal.scaling;
	
	for (i = 0; i < 6; i++)
	{
		checksum += Touch_Cal.cal[i];
	}
	

	if(Touch_Cal.checksum != checksum || force_cal) //if checksum is not same ,use for cal again 
	{
		Touch_Cal_Frist = (TOUCH_Calibration_TypeDef*)malloc(sizeof(TOUCH_Calibration_TypeDef));
		
		if(TOUCH_Calibration(Touch_Cal_Frist))//if cal OK,save the cal var
		{

				Touch_Cal_Var_Erase();
				Touch_Cal_Var_Save(Touch_Cal_Frist->Var);
				Touch_Cal_Var_Read(Touch_Cal);
				
				if(Touch_Cal_Frist->Var.checksum == Touch_Cal.checksum) //read the cal var ,check the chceksum
					result = TRUE;
				else 
					result = FALSE;
				
				free(Touch_Cal_Frist);
		}
		
	}
	else//if checksum is same ,use the cal var 
	{
			result = TRUE;
	}
	
	return result;
#endif


#if (USE_TOUCH_CAL_SAVE	== TOUCH_NOUSE)
	
		Touch_Cal_Frist = (TOUCH_Calibration_TypeDef*)malloc(sizeof(TOUCH_Calibration_TypeDef));

		result = TOUCH_Calibration(Touch_Cal_Frist);
		
		memcpy(&Touch_Cal,&Touch_Cal_Frist->Var,sizeof(Touch_Cal));

	
		free(Touch_Cal_Frist);
	
		return result;
		
#endif	
}




