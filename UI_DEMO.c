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
///////////////////////////////////////////////////////////////////
const LCD_DISPLAY_PositionTypeDef PositionTable_pp[] = {
  {0,190}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_pp = {
  1, (LCD_DISPLAY_PositionTypeDef*)PositionTable_pp
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



typedef struct
{
	u16 xBg;
	u16 yBg;
	u16 xEn;
	u16 yEn;
	bool ispress;
}Touch_Screen_Rect_TypeDef;


typedef struct 
{
	u8 PicID[3];
	Touch_Screen_Rect_TypeDef Button;  
	const LCD_DISPLAY_FrameInfoTypeDef *PicInfo;
}	UI_TypeDef;



UI_DEMO_TypeDef gUI_DataBase;


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



UI_TypeDef UI_PAGE1;
UI_TypeDef UI_PAGE2;
UI_TypeDef UI_PAGE3;
UI_TypeDef UI_TEMPSET;
UI_TypeDef UI_TEMPADD;
UI_TypeDef UI_TEMPSUB;

void DUI_DEMO_INIT(void)
{
	
	UI_PAGE1.PicID[0] = 17;
	UI_PAGE1.PicInfo = &FrameInfo_Full;
  TS_SET_RECT(&UI_PAGE1.Button,97,206,43,50);	
	
	UI_PAGE2.PicID[0] = 18;
	UI_PAGE2.PicInfo = &FrameInfo_Full;
  TS_SET_RECT(&UI_PAGE2.Button,217,206,43,50);	
	
	UI_PAGE3.PicID[0] = 19;
	UI_PAGE3.PicInfo = &FrameInfo_Full;
  TS_SET_RECT(&UI_PAGE3.Button,338,206,43,50);	
	
	UI_TEMPSET.PicID[0] = 5;
	UI_TEMPSET.PicID[1] = 6;
	UI_TEMPSET.PicInfo = &FrameInfo_TEMPSET;
	TS_SET_RECT(&UI_TEMPSET.Button,154,86,76,145);
	
	UI_TEMPADD.PicID[0] = 7;
	UI_TEMPADD.PicID[1] = 8;
	UI_TEMPADD.PicID[2] = 9;
	UI_TEMPADD.PicInfo = &FrameInfo_ADD;	
	TS_SET_RECT(&UI_TEMPADD.Button,79,86,42,42);
	
		
	UI_TEMPSUB.PicID[0] = 11;
	UI_TEMPSUB.PicID[1] = 12;
	UI_TEMPSUB.PicID[2] = 13;
	UI_TEMPSUB.PicInfo = &FrameInfo_SUB;	
	TS_SET_RECT(&UI_TEMPSUB.Button,359,86,41,46);


		gUI_DataBase.page.Set_sta= 1;
		gUI_DataBase.updata = TRUE;
}


void DUI_TS_Scan(void)
{
	TOUCH_Logical_Coor_Get(&Tocuh);
/////////////////////////////////////////////////////		
	if(TS_Scan_RECT(&UI_PAGE1.Button,&Tocuh))
	{
		gUI_DataBase.page.Set_sta= 1;
		gUI_DataBase.updata = TRUE;
	}		
	
	if(TS_Scan_RECT(&UI_PAGE2.Button,&Tocuh))
	{
		gUI_DataBase.page.Set_sta = 2;
		gUI_DataBase.updata = TRUE;
	}
	
	if(TS_Scan_RECT(&UI_PAGE3.Button,&Tocuh))
	{	
			gUI_DataBase.page.Set_sta = 3;
			gUI_DataBase.updata = TRUE;
	}
	
///////////////////////////////////////////////////////////////////////////		
	
if(gUI_DataBase.page.Now_sta == 1)
{
	if(gUI_DataBase.tmpset.Now_sta == 1)
	{
		if(TS_Scan_RECT(&UI_TEMPADD.Button,&Tocuh))
		{
				gUI_DataBase.tmpadd.Set_sta = 2;
				gUI_DataBase.updata = TRUE;
		}
		
		
		if(TS_Scan_RECT(&UI_TEMPSUB.Button,&Tocuh))
		{
				gUI_DataBase.tmpsub.Set_sta = 2;
				gUI_DataBase.updata = TRUE;
		}
	}

///////////////////////////////////////////////////////////////////////////	
	
	if(TS_Scan_RECT(&UI_TEMPSET.Button,&Tocuh))
	{
			
		Temp.CorEn = TRUE;
	
	}
}
/////////////////////////////////////////////////////////////////////////////////	
	
	
	
}

void DISPALY_WIFI(void);

void DUI_DISPLAY(void)
{
	
	
	if(!gUI_DataBase.updata )
			return;
	
	
		if(gUI_DataBase.page.Now_sta != gUI_DataBase.page.Set_sta)
		{
			switch(gUI_DataBase.page.Set_sta)
			{
				case 1 :
					
				DISPLAY_full(UI_PAGE1.PicID[0]);				
				gUI_DataBase.tempset_updata = TRUE;
				gUI_DataBase.tempnow_updata = TRUE;
				break;
				
				case 2 :DISPLAY_full(UI_PAGE2.PicID[0]);;break;
				case 3 :DISPLAY_full(UI_PAGE3.PicID[0]);;break;
				default :break;
			}
			gUI_DataBase.page.Now_sta = gUI_DataBase.page.Set_sta;
			gUI_DataBase.time_updata = TRUE;
			gUI_DataBase.date_updata = TRUE;
		}
		
		if(gUI_DataBase.page.Now_sta == 1)
		{
			if(gUI_DataBase.tempset_updata == TRUE)	
			{
				LCD_TextColorSet(White);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMP,10);
				LCD_ShowNumPoint(170,86,64,32,1,Temp.Set);
				
				gUI_DataBase.tempset_updata = FALSE;
			}
			
			if(gUI_DataBase.tempnow_updata == TRUE)	
			{
				
				LCD_TextColorSet(White);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TEMPNOW,14);
				LCD_ShowNumPoint(188,177,16,8,1,Temp.Now);
				DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_HUMIDITY,15);
				LCD_DrawString(305,177,16,8,1,"N/A");
				
				gUI_DataBase.tempnow_updata = FALSE;
			}		
			
			
			if(gUI_DataBase.tmpset.Now_sta != gUI_DataBase.tmpset.Set_sta)	
			{
				
				if(gUI_DataBase.tmpset.Set_sta == 1)    //set
				{
					Temp.SetEn = TRUE;
					gUI_DataBase.tmpadd.Now_sta = gUI_DataBase.tmpadd.Set_sta = 1;
					gUI_DataBase.tmpsub.Now_sta = gUI_DataBase.tmpsub.Set_sta = 1;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSET.PicInfo,UI_TEMPSET.PicID[1]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPADD.PicInfo,UI_TEMPADD.PicID[1]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSUB.PicInfo,UI_TEMPSUB.PicID[1]);
				}
				else //NORMAL
				{
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSET.PicInfo,UI_TEMPSET.PicID[0]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPADD.PicInfo,UI_TEMPADD.PicID[0]);
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSUB.PicInfo,UI_TEMPSUB.PicID[0]);
				
				}
				gUI_DataBase.tmpset.Now_sta = gUI_DataBase.tmpset.Set_sta;
			}
		
			if(gUI_DataBase.tmpset.Now_sta == 1)
			{
				//add temp set
				if(gUI_DataBase.tmpadd.Now_sta != gUI_DataBase.tmpadd.Set_sta)	
				{
					//add tempset
					Temp.AddEn = TRUE;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPADD.PicInfo,UI_TEMPADD.PicID[2]);
					gUI_DataBase.tmpadd.Now_sta = gUI_DataBase.tmpadd.Set_sta;
				}
				else 
				{
					gUI_DataBase.tmpadd.Now_sta = gUI_DataBase.tmpadd.Set_sta = 1;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPADD.PicInfo,UI_TEMPADD.PicID[1]);
				}
				
				//sub temp set 
				if(gUI_DataBase.tmpsub.Now_sta != gUI_DataBase.tmpsub.Set_sta)	
				{
					//sub tempset
					Temp.SubEn = TRUE;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSUB.PicInfo,UI_TEMPSUB.PicID[2]);
					gUI_DataBase.tmpsub.Now_sta = gUI_DataBase.tmpsub.Set_sta;
				}
				else 
				{
					gUI_DataBase.tmpsub.Now_sta = gUI_DataBase.tmpsub.Set_sta = 1;
					DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)UI_TEMPSUB.PicInfo,UI_TEMPSUB.PicID[1]);
				}
				
				
			
			
			}
		
		
		}
		
		if(gUI_DataBase.page.Now_sta == 2)
		{
			DISPALY_WIFI();
		}	
		
		if(gUI_DataBase.page.Now_sta == 3)
		{
			
		}

	

		
	if(gUI_DataBase.time_updata == TRUE)
	{	
		LCD_TextColorSet(White);
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_TIME,1);
		LCD_ShowNum(220,2,16, 8,1,Temp.Time);
		gUI_DataBase.time_updata = FALSE;
	}
	
	if(gUI_DataBase.date_updata == TRUE)
	{
		LCD_TextColorSet(White);
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_DATE,0);
		LCD_DrawString(2,2, 16, 8, 1,"2018/02/01");
		gUI_DataBase.date_updata = FALSE;
	}
	
	
		
	if(FLAG_WIFI.APLINK == TRUE)
	{
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_WIFI,3);
	}
	else 
	{
		DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_WIFI,4);
	}
	
	
	
	gUI_DataBase.updata = FALSE;
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
    //gUI.IsDemo1Update = FALSE;
    
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
  //gUI.IsDemo1Update = FALSE;

  init.Mode             = LCD_DISPLAY_MODE_NORMAL;
  init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_Full;
 //  init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_pp;	
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


void DISPLAY_ADD(u8 PicID)
{
	LCD_DISPLAY_InitTypedef init;
  //gUI.IsDemo1Update = FALSE;

  init.Mode             = LCD_DISPLAY_MODE_NORMAL;
  init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_ADD;
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












