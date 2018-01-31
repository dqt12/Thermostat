#include "main.h"

/*
  Frame Information for Full
 */
const LCD_DISPLAY_PositionTypeDef PositionTable_Full[] = {
  {0,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_Full = {
  1, (LCD_DISPLAY_PositionTypeDef*)PositionTable_Full
};

/////////////////////////////////////////////////////////////////////////////


const LCD_DISPLAY_PositionTypeDef PositionTable_WIFI[] = {
  {435,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_WIFI = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_WIFI
};

/////////////////////////////////////////////////////////////////////////////

//設置溫度
const LCD_DISPLAY_PositionTypeDef PositionTable_TEMPSET[] = {
  {202,61}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_TEMPSET = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_TEMPSET
};

//設置溫度顯示
const LCD_DISPLAY_PositionTypeDef PositionTable_TEMP[] = {
  {154,86}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_TEMP = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_TEMP
};

//現在溫度顯示
const LCD_DISPLAY_PositionTypeDef PositionTable_TEMPNOW[] = {
  {183,173}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_TEMPNOW = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_TEMPNOW
};

//現在濕度顯示
const LCD_DISPLAY_PositionTypeDef PositionTable_HUMIDITY[] = {
  {299,173}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_HUMIDITY = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_HUMIDITY
};

//////////////////////////////////////////////////////////////////////
const LCD_DISPLAY_PositionTypeDef PositionTable_ADD[] = {
  {79,86}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_ADD = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_ADD
};

const LCD_DISPLAY_PositionTypeDef PositionTable_SUB[] = {
  {359,86}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_SUB = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_SUB
};

//////////////////////////////////////////////////////////////////////
const LCD_DISPLAY_PositionTypeDef PositionTable_DATE[] = {
  {0,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_DATE = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_DATE
};

const LCD_DISPLAY_PositionTypeDef PositionTable_TIME[] = {
  {183,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_TIME = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_TIME
};



UI_PLUG_TypeDef UI_PAGE1;
UI_PLUG_TypeDef UI_PAGE2;
UI_PLUG_TypeDef UI_PAGE3;
UI_PLUG_TypeDef UI_TEMPSET;
UI_PLUG_TypeDef UI_TEMPADD;
UI_PLUG_TypeDef UI_TEMPSUB;


UI_DEMO_TypeDef DUI_DataBase;

void DISPALY_WIFI(void);

void TS_SET_RECT(Touch_Screen_Rect_TypeDef *p,u16 XBg,u16 YBg ,u16 Height, u16 Width)
{

	p->xBg = XBg;
	p->xEn = XBg+Width;
	p->yBg = YBg;
	p->yEn = YBg+Height;
	p->ispress = FALSE;	
	
//	LCD_DrawRect(XBg,YBg,Height,Width,Red);
}

bool TS_Scan_RECT(Touch_Screen_Rect_TypeDef *p,TOUCH_XY_TypeDef *pt)
{
	if(pt->isPress == TRUE)
	{
		if((pt->x > p->xBg) && (pt->x < p->xEn )) 
		{	
			if((pt->y > p->yBg) &&(pt->y < p->yEn))
			{
				
				p->ispress = TRUE;
				
				if(TOUCH_CheckPressed() == TRUE)
					return (TRUE);	
				else 
					return (FALSE);
			}
		}
	}
	p->ispress = FALSE;
	
	return (FALSE);
}




void DUI_DEMO_INIT(void)
{
	
	UI_PAGE1.PicID[0] = 17;
  TS_SET_RECT(&UI_PAGE1.Button,97,206,43,50);	
	
	UI_PAGE2.PicID[0] = 18;
  TS_SET_RECT(&UI_PAGE2.Button,217,206,43,50);	
	
	UI_PAGE3.PicID[0] = 19;
  TS_SET_RECT(&UI_PAGE3.Button,338,206,43,50);	
	
	UI_TEMPSET.PicID[0] = 5;
	UI_TEMPSET.PicID[1] = 6;
	TS_SET_RECT(&UI_TEMPSET.Button,154,86,76,145);
	
	UI_TEMPADD.PicID[0] = 7;
	UI_TEMPADD.PicID[1] = 8;
	UI_TEMPADD.PicID[2] = 9;
	TS_SET_RECT(&UI_TEMPADD.Button,79,86,42,42);
	
		
	UI_TEMPSUB.PicID[0] = 11;
	UI_TEMPSUB.PicID[1] = 12;
	UI_TEMPSUB.PicID[2] = 13;
	TS_SET_RECT(&UI_TEMPSUB.Button,359,86,41,46);

	
	DUI_DataBase.page.Set_sta = 1;
	DUI_DataBase.wifi.Set_sta = FALSE;
	DUI_DataBase.updata = TRUE;
}


void DUI_TS_Scan(void)
{
	TOUCH_Logical_Coor_Get(&Tocuh);
/////////////////////////////////////////////////////		
	if(TS_Scan_RECT(&UI_PAGE1.Button,&Tocuh))
	{
		DUI_DataBase.page.Set_sta= 1;
		DUI_DataBase.updata = TRUE;
	}		
	
	if(TS_Scan_RECT(&UI_PAGE2.Button,&Tocuh))
	{
		DUI_DataBase.page.Set_sta = 2;
		DUI_DataBase.updata = TRUE;
	}
	
	if(TS_Scan_RECT(&UI_PAGE3.Button,&Tocuh))
	{	
			DUI_DataBase.page.Set_sta = 3;
			DUI_DataBase.updata = TRUE;
	}
	
	///////////////////////////////////////////////////////////////////////////		
		
	if(DUI_DataBase.page.Now_sta == 1) //page1 use to set temp
	{
		if(DUI_DataBase.tmpset.Now_sta == 1)
		{
			if(TS_Scan_RECT(&UI_TEMPADD.Button,&Tocuh))
			{
					DUI_DataBase.tmpadd.Set_sta = 2;
					DUI_DataBase.updata = TRUE;
			}
			
			
			if(TS_Scan_RECT(&UI_TEMPSUB.Button,&Tocuh))
			{
					DUI_DataBase.tmpsub.Set_sta = 2;
					DUI_DataBase.updata = TRUE;
			}
		}

		if(TS_Scan_RECT(&UI_TEMPSET.Button,&Tocuh))
		{
			Temp.CorEn = TRUE;
		}
		
	}
	/////////////////////////////////////////////////////////////////////////////////	
	

}



void DUI_DISPLAY(void)
{

	if(!DUI_DataBase.updata )
			return;
	
	
		if(DUI_DataBase.page.Now_sta != DUI_DataBase.page.Set_sta)
		{
			switch(DUI_DataBase.page.Set_sta)
			{
				case 1 :
					
				DISPLAY_full(UI_PAGE1.PicID[0]);				
				DUI_DataBase.tempset_updata = TRUE;
				DUI_DataBase.tempnow_updata = TRUE;
				break;
				
				case 2 :DISPLAY_full(UI_PAGE2.PicID[0]);;break;
				case 3 :DISPLAY_full(UI_PAGE3.PicID[0]);;break;
				default :break;
			}
			DUI_DataBase.page.Now_sta = DUI_DataBase.page.Set_sta;
			DUI_DataBase.time_updata = TRUE;
			DUI_DataBase.date_updata = TRUE;
		}
		
		if(DUI_DataBase.page.Now_sta == 1)
		{
			if(DUI_DataBase.tempset_updata == TRUE)	
			{
				LCD_TextColorSet(White);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMP,10);
				LCD_ShowNumPoint(170,86,64,32,1,Temp.Set);
				DUI_DataBase.tempset_updata = FALSE;
				
			}
			
			if(DUI_DataBase.tempnow_updata == TRUE)	
			{
				
				LCD_TextColorSet(White);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMPNOW,14);
				LCD_ShowNumPoint(188,177,16,8,1,Temp.Now);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_HUMIDITY,15);
				LCD_DrawString(305,177,16,8,1,"N/A");
				
				DUI_DataBase.tempnow_updata = FALSE;
			}		
			
			
			if(DUI_DataBase.tmpset.Now_sta != DUI_DataBase.tmpset.Set_sta)	
			{
				
				if(DUI_DataBase.tmpset.Set_sta == 1)    //1： set state
				{
					Temp.SetEn = TRUE;
					DUI_DataBase.tmpadd.Now_sta = DUI_DataBase.tmpadd.Set_sta = 1;
					DUI_DataBase.tmpsub.Now_sta = DUI_DataBase.tmpsub.Set_sta = 1;
					
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMPSET,UI_TEMPSET.PicID[1]);
					//show the add & sub button
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_ADD,UI_TEMPADD.PicID[1]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_SUB,UI_TEMPSUB.PicID[1]);
				}
				else //0： normal state 
				{
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMPSET,UI_TEMPSET.PicID[0]);
					//dont show the add & sub button
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_ADD,UI_TEMPADD.PicID[0]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_SUB,UI_TEMPSUB.PicID[0]);
				
				}
				DUI_DataBase.tmpset.Now_sta = DUI_DataBase.tmpset.Set_sta;
			}
		
			if(DUI_DataBase.tmpset.Now_sta == 1)
			{
				//add temp set
				if(DUI_DataBase.tmpadd.Now_sta != DUI_DataBase.tmpadd.Set_sta)	// 2: add press
				{
					//add tempset
					Temp.AddEn = TRUE;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_ADD,UI_TEMPADD.PicID[2]);
					DUI_DataBase.tmpadd.Now_sta = DUI_DataBase.tmpadd.Set_sta;
				}
				else 
				{
					DUI_DataBase.tmpadd.Now_sta = DUI_DataBase.tmpadd.Set_sta = 1; //1:add nopress
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_ADD,UI_TEMPADD.PicID[1]);
				}
				
				//sub temp set 
				if(DUI_DataBase.tmpsub.Now_sta != DUI_DataBase.tmpsub.Set_sta)	// 2: sub press
				{
					//sub tempset
					Temp.SubEn = TRUE;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_SUB,UI_TEMPSUB.PicID[2]);
					DUI_DataBase.tmpsub.Now_sta = DUI_DataBase.tmpsub.Set_sta;
				}
				else 
				{
					DUI_DataBase.tmpsub.Now_sta = DUI_DataBase.tmpsub.Set_sta = 1;//1:sub nopress
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_SUB,UI_TEMPSUB.PicID[1]);
				}
			}
		
		}
		
		if(DUI_DataBase.page.Now_sta == 2)
		{
			DISPALY_WIFI();
		}	
		
		if(DUI_DataBase.page.Now_sta == 3)
		{
			
		}


	if(DUI_DataBase.time_updata == TRUE)
	{	
		LCD_TextColorSet(White);
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TIME,1);
		LCD_ShowNum(220,2,16, 8,1,Temp.Time);
		DUI_DataBase.time_updata = FALSE;
	}
	
	if(DUI_DataBase.date_updata == TRUE)
	{
		LCD_TextColorSet(White);
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_DATE,0);
		LCD_DrawString(2,2, 16, 8, 1,"2018/02/01");
		DUI_DataBase.date_updata = FALSE;
	}
	
	if(DUI_DataBase.wifi_updata == TRUE)
	{
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_WIFI,3);
	}
	else 
	{
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_WIFI,4);
	}
	
	DUI_DataBase.updata = FALSE;
	
}







void DISPALY_WIFI(void)
{
	u8 line = 16;

	LCD_TextColorSet(White);
	
	LCD_DrawString(90,line*2, 16, 8, 1,"WIFI AUTO LINKing...");
	
	
	if(CMD_Cont_Trg >= L_SMART  && CMD_Cont_Trg < L_LINKED)	
	{
		if(FLAG_WIFI.SMARTLINK == TRUE) 
		{
			LCD_DrawString(90,line*3, 16, 8, 1,"WIFI LINK TO AP BY APP!");
		}

	}
	
	if(CMD_Cont_Trg >= L_Dev )	
	{
		LCD_DrawString(90,line*4, 16, 8, 1,"WIFI AUTO LINK OK!");
		LCD_DrawString(90,line*5, 16, 8, 1,"WIFI INF:");
		LCD_DrawString(90,line*6, 16, 8, 1,WIFI_MAC);
		LCD_DrawString(90,line*7, 16, 8, 1,WIFI_IP);
		LCD_DrawString(90,line*8, 16, 8, 1,"PORT:1001");
	}
	
	
}




/*********************************************************************************************************//**
  * @brief  Demo1.
  * @retval None
  ***********************************************************************************************************/
void DISPLAY_part( LCD_DISPLAY_FrameInfoTypeDef* qq,u8 PicID)
{
    LCD_DISPLAY_InitTypedef init;
	
    init.Mode             = LCD_DISPLAY_MODE_NORMAL;
    init.pFrameInfo       = qq;
    init.pImageRemapTable = NULL;
    init.ImageStartIndex  = PicID;
    init.ImageLength      = 1;
    init.FrameRate        = 0;
    LCD_DISPLAY_Init(&init);

    while(gLCD_DISPLAY.ImageCounter < init.ImageLength)
    {
      LCD_DISPLAY_Process();
    }
}

void DISPLAY_full(u8 PicID)
{
	LCD_DISPLAY_InitTypedef init;

  init.Mode             = LCD_DISPLAY_MODE_NORMAL;
  init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_Full;
  init.pImageRemapTable = NULL;
  init.ImageStartIndex  = PicID;
  init.ImageLength      = 1;
  init.FrameRate        = 0;
  LCD_DISPLAY_Init(&init);

	while(gLCD_DISPLAY.ImageCounter < init.ImageLength)
  {
    LCD_DISPLAY_Process();
  }
}











