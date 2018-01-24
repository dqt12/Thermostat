#include "main.h"

void LCD_TEST(void)
{
	LCD_BackColorSet(White);
	LCD_TextColorSet(Red);

	LCD_DrawChar(0,0,12,6,0,'1');
	LCD_DrawChar(10,0,16,8,0,'2');
	LCD_DrawChar(20,0,24,16,0,'3');
	
	LCD_DrawChar(0,40,12,6,1,'1');
	LCD_DrawChar(10,40,16,8,1,'2');
	LCD_DrawChar(20,40,24,16,1,'3');

	LCD_ShowTemp(0,120,0,234);
	LCD_ShowTemp(0,150,1,234);
	
	LCD_DrawPoint(100,10,Green);
	LCD_DrawPoint(100,20,Red);
	LCD_DrawPoint(100,30,Blue);
	
	LCD_DrawLine(150,10,100,Horizontal,Green);	
	LCD_DrawLine(150,20,100,Horizontal,Red);	
	LCD_DrawLine(150,30,100,Horizontal,Blue);	 

	LCD_DrawRect(50,50,20,20,Green);
	LCD_DrawRect(80,50,20,20,Red);
	LCD_DrawRect(110,50,20,20,Blue);
	
	LCD_DrawFillRect(150,50,20,20,Green);
	LCD_DrawFillRect(180,50,20,20,Red);
	LCD_DrawFillRect(210,50,20,20,Blue);	
	
	
	LCD_DrawCircle(50,100,10,Green);
	LCD_DrawCircle(80,100,10,Red);
	LCD_DrawCircle(110,100,10,Blue);
	
	LCD_DrawFillCircle(150,100,10,Green);
	LCD_DrawFillCircle(180,100,10,Red);
	LCD_DrawFillCircle(210,100,10,Blue);
	
	LCD_DrawString(0, 210, 12, 6, 0, "123456789ABCD");
	LCD_DrawString(0, 230, 16, 8, 0, "123456789ABCD");
  LCD_DrawString(0, 250, 24, 16, 0, "123456789ABCD");

	
	while(1);
}

//	u8 text[256];
//	u8 cont;
//void FLASH_TEST(void)
//{
//	u16 i;
//	SPI_FLASH_Init();
//	
//	SPI_FLASH_WriteStatus(0x00);
//	
//	SPI_FLASH_BufferRead(text, 0, 200);
//	
//	SPI_FLASH_SectorErase(0);  
//	
//	SPI_FLASH_BufferRead(text, 0, 200);
//	
//	for(i=0;i<200;i++) text[i]=i;
//	SPI_FLASH_BufferWrite(text, 0, 200);

//	for(i=0;i<200;i++) text[i]=0;
//	
//	SPI_FLASH_BufferRead(text, 0, 200);

//	cont = 200;
//}

void Display_Temp(void)
{
		if(Temp.SetEn == TRUE)
		{
		 LCD_BackColorSet(Black);
		 LCD_TextColorSet(White);	
		}
		else 
		{
			LCD_BackColorSet(Blue2);
		  LCD_TextColorSet(Yellow);
		}
		
		
		LCD_DrawString(110, 0, 16, 8, 1,"Set Temp:");			
		LCD_ShowTemp(110,20,0,Temp.Set);		
		
		LCD_DrawString(100, 60, 24, 16, 0," + ");	
		LCD_DrawString(160, 60, 24, 16, 0," - ");	
		
		LCD_BackColorSet(Blue2);
		LCD_TextColorSet(Yellow);	
		
		LCD_DrawString(0, 0, 16, 8, 1,"Now Temp:");	
		LCD_ShowTemp(0,20,0,Temp.Now);
		
//		LCD_DrawString(0, 50, 16, 8, 1,"KEY_STATE:");
//		LCD_ShowNum(0,70,16,0,KEY_STATE);	

		LCD_DrawString(0, 50, 16, 8, 1,"Second:");
		LCD_ShowNum(0,70,16,0,Temp.Time);
		
//		LCD_DrawString(110, 50, 16, 8, 1,"Second:");
//		LCD_ShowNum(110,70,16,0,Temp.Time);
		
		LCD_DrawString(0,90, 16, 8, 1,"ADC0~1~2:");
		LCD_ShowNum(0,110,16,0,ADC_DATA[0]);
		LCD_ShowNum(40,110,16,0,ADC_DATA[1]);
		LCD_ShowNum(80,110,16,0,ADC_DATA[2]);
//		
//		
		LCD_DrawString(0,160, 16, 8, 1,"TS:X ~ Y:");
		LCD_ShowNum(0,180,16,0,Tocuh.xPhys);
		LCD_ShowNum(0,180+16*1,16,0,Tocuh.x);
	  LCD_ShowNum(40,180,16,0,Tocuh.yPhys);
		LCD_ShowNum(40,180+16*1,16,0,Tocuh.y);
}


u16 Display_State_Line = 0;
void Display_State(char *Str)
{
	                                      
	LCD_TextColorSet(Black);
	
	if(Display_State_Line >= 256) 
	{
//		LCD_DrawFillRect(279,0,271,200,White);
		Display_State_Line = 0;
	}
	
	LCD_DrawString(285,Display_State_Line, 16, 8, 0,Str);
	Display_State_Line += 16;	
}

