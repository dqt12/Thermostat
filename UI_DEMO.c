#include "main.h"

UI_T gUI;
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




const LCD_DISPLAY_PositionTypeDef PositionTable_WIFI[] = {
  {435,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_WIFI = {
  1, 
	(LCD_DISPLAY_PositionTypeDef*)PositionTable_WIFI
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




void DISPLAY(u8 PicID)
{
	switch (PicID)
{
	case 4: DISPLAY_part((LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_WIFI,4);break;
		

	default:	break;
	
}
	

}










