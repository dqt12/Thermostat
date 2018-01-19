/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include <string.h>
#include "lcm.h"
#include "spi_flash.h"
#include "ESP8266.h"
#include "main.h"

#include "tslib.h"




// FOR USB VCP
#include "ht32_usbd_core.h"
#include "ht32_usbd_class.h"
#include "ht32_usbd_descriptor.h"
#include "iap_handler.h"
#include "_ht32_project_source.h"

//USE FOR LCD DISPLAY
//#include "lcd_display.h"

/* Global variables ----------------------------------------------------------------------------------------*/
__ALIGN4 USBDCore_TypeDef gUSBCore;
USBD_Driver_TypeDef gUSBDriver;
u32 gIsLowPowerAllowed = TRUE;


/*
  Frame Information for Full
 */
const LCD_DISPLAY_PositionTypeDef PositionTable_Full[] = {
  {0,0}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_Full = {
  1, (LCD_DISPLAY_PositionTypeDef*)PositionTable_Full
};

const LCD_DISPLAY_PositionTypeDef PositionTable_pp[] = {
  {0,190}
};
const LCD_DISPLAY_FrameInfoTypeDef FrameInfo_pp = {
  1, (LCD_DISPLAY_PositionTypeDef*)PositionTable_pp
};

//extern u16 Demo_N;
//KeyCmd_TypeDef KeyCmd;
//vu32 gTimebaseDelayCounter;
UI_T gUI;
LCD_DrawSize LCD_IData;
u16 RemapTable[178];
LCD_DISPLAY_FlagTypedef FLAG_IMG;


/* Private function prototypes -----------------------------------------------------------------------------*/
void USB_Configuration(void);

void CKCU_Configuration(void);
void GPIO_Configuration(void);
void KEY_Configuration(void);
void RTC_Configuration(void);
void ADC_Configuration(void);
void ADC_POLL_Configuration(void);
void ADC_PDMA_Configuration(void);
void BFTM_Configuration(void);
void SYSTICK_configuration(void);
void NVIC_Configuration(void);
void Delay(u32 nTime);
void TimingDelay(void);




/* Global functions ----------------------------------------------------------------------------------------*/
vu16 ADC_DATA[3];
vu16 Touch_X;
vu16 Touch_Y;


//0~50Ce
const u16 NTC_10K_MAP[]={
1221	,1264	,1307	,1350	,1394	,1439	,1484	,1529	,1574	,1620	,1666	,
1713	,1759	,1806	,1852	,1899	,1945	,1992	,2038	,2084	,2130	,2175	,
2220	,2265	,2309	,2353	,2396	,2439	,2481	,2523	,2564	,2605	,2644	,
2684	,2722	,2760	,2797	,2833	,2868	,2903	,2937	,2970	,3003	,3034	,
3065	,3096	,3125	,3154	,3182	,3209	,3235	
};



//wifi Ç˜›îÖf◊h
////////////////////////E,0,S,2,5,6,N,2,5,0,/r,/n
u8 Thermostat_DATA[12]={'E','0','S','2','6','0','N','2','6','0','\r','\n'};

struct
{
	u8 	En;
	u8 	SetEn;
	u16 Now;
	u16	Set;
	u16 Time;
}TEMP;



vu8 FLAG_10mS ;
vu8 FLAG_20mS ;
vu8 FLAG_500mS ;
vu8 FLAG_1S ;

u8 KEY_STATE;

u8 FLAG_DISPLAY ;


void Display_Temp(void)
{

//		LCD_DrawString(10,180, 16, 8, 0,"ADCT X~Y:");
//		LCD_ShowNum(80,180,16,0,ADC_TFiler_X);
//		LCD_ShowNum(80,180+16*1,16,0,Touch_X);

//	  LCD_ShowNum(120,180,16,0,ADC_TFiler_Y);
//		LCD_ShowNum(120,180+16*1,16,0,Touch_Y);
	
	
	
	
		LCD_DrawString(40,160, 16, 8, 0,"X  ~  Y:");
		
		LCD_ShowNum(40,180,16,0,Tocuh.xPhys);
		LCD_ShowNum(40,180+16*1,16,0,Tocuh.x);

	  LCD_ShowNum(80,180,16,0,Tocuh.yPhys);
		LCD_ShowNum(80,180+16*1,16,0,Tocuh.y);
		
//		LCD_ShowNum(120,180,16,0,ADC_TFiler_Z);
//		LCD_ShowNum(120,180+16*1,16,0,Touch_Z/10000);
//		LCD_ShowNum(120+8*4,180+16*1,16,0,Touch_Z%10000);
}


void Display_Temp_1(void)
{
		if(TEMP.SetEn)
		{
			LCD_BackColorSet(Yellow);                                                
			LCD_TextColorSet(Red);
		}	
		else 
		{																																
			LCD_BackColorSet(Red);                                                           
			LCD_TextColorSet(Yellow);
		}
		
		LCD_DrawString(10, 60, 16, 8, 0,"Set Temp:");			
		LCD_ShowTemp(50,60,0,TEMP.Set);		
		
	
		LCD_BackColorSet(Red);
		LCD_TextColorSet(Yellow);
		
		LCD_DrawString(10, 0, 16, 8, 0,"Now Temp:");	
		LCD_ShowTemp(50,0,0,TEMP.Now);
		
		LCD_DrawString(10, 120, 16, 8, 0,"KEY_STATE:");
		LCD_ShowNum(100,120,16,0,KEY_STATE);	

		LCD_DrawString(10, 140, 16, 8, 0,"TIME:");
		LCD_ShowNum(100,140,16,0,TEMP.Time);
		
		LCD_DrawString(10,160, 16, 8, 0,"ADC0~2:");
		LCD_ShowNum(80,160,16,0,ADC_DATA[0]);
		LCD_ShowNum(120,160,16,0,ADC_DATA[1]);
		LCD_ShowNum(160,160,16,0,ADC_DATA[2]);
}



/*
u16 Line ;
void Display_State_Clear(void)
{
	Line = 0;
	TFT_Fill(250,0,479,271,White);
}
	


void Display_State(char *Str)
{
	u8 line;
	
	if(FLAG_DISPLAY == NULL)
	{
	
		LCD_BackColorSet(White);                                                
		LCD_TextColorSet(Black);
		line = TFT_ShowString(255,Line,Str,16,0);
		if(line == 0) 
		{
			Line = 16;
			
		}
		Line = Line + line;
	}

}
*/

u16 TEMP_LIMIT(u16 data)
{
	//32~21C
		if(data >= 320) data = 320;
		if(data <= 210) data = 210;
	
		return data;
}


void KEY_Scan(void)
{
		switch(KEY_STATE)
		{
			case 0x01 : 
			{
				TEMP.SetEn ^=(1<<0) ;
			}; break;//key1
			
			case 0x02 : 
			{
				if(TEMP.SetEn) 
				{ 
					TEMP.Set += 5;
					TEMP.Set = TEMP_LIMIT(TEMP.Set);
				}
				else
				{
//					Display_State_Clear();
//					FLAG_DISPLAY = SET;
//					TFT_DrawPicDMA(0, 0, 272, 480,0);
					
//					LCD_Clear(Red);
//					TFT_DrawPicture(20, 220, 48, 100, HT32_Table);
//					TFT_Fill(250,0,479,271,White);
//					Display_Temp();
//					FLAG_DISPLAY = NULL;					
					
					
				}
			}break;//key2

			case 0x04 : 
			{
				if(TEMP.SetEn) 
				{ 
						TEMP.Set -= 5;
						TEMP.Set = TEMP_LIMIT(TEMP.Set);						
				}
				else 
				{
					
					
					
				}
			};break;//key3	
			
			case 0x08 :
			{
				CMD_Cont = L_REST;
				CMD_Cont_Trg = L_REST;
			};break;//key4
									
			default : break;
		
		}
				KEY_STATE = 0;
}



u16 ADC_to_TEMP(u16 NTC_adc)
{
	u16 i;
	vu16 buf;
	
	if(NTC_adc >=4095 ) return 100;//260-160
	
	for(i=0;i<50;i++)
	{
		if(NTC_adc <= NTC_10K_MAP[i]) 	break;
	}
	
	if(i>0)//>0C
	{
		
		buf = (NTC_10K_MAP[i]+ NTC_10K_MAP[i-1])/2;
		
		if(NTC_adc <= buf) i =i*10+5;
			else i =i*10;
	}
	else i =i*10;
	
	
	return i;

}





void UPDATA(void)
{
	if(FLAG_WIFI_UPDATA)
	{
		FLAG_WIFI_UPDATA = 0;
		
		if(Thermostat_DATA[0] == 'E')
			Thermostat_DATA[1] = TEMP.En + '0' ;
		
		if(Thermostat_DATA[2] == 'S')
		{
			TEMP.Set = (Thermostat_DATA[3]-'0')*100; 
			TEMP.Set += (Thermostat_DATA[4]-'0')*10; 
			TEMP.Set += (Thermostat_DATA[5]-'0'); 	
		}
	}	
	else 
	{
	  Thermostat_DATA[0] = 'E';
	  Thermostat_DATA[1] = TEMP.En + '0' ;

		Thermostat_DATA[2] = 'S';
		Thermostat_DATA[3] = TEMP.Set/100 + '0';
		Thermostat_DATA[4] = TEMP.Set%100/10 + '0';
		Thermostat_DATA[5] = TEMP.Set%10 + '0';
	
	
		Thermostat_DATA[6] = 'N';
		Thermostat_DATA[7] = TEMP.Now/100 + '0';
		Thermostat_DATA[8] = TEMP.Now%100/10 + '0';
		Thermostat_DATA[9] = TEMP.Now%10 + '0';	
	

	}
	
}

/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/



void LCD_TEST(void)
{
	LCD_BackColorSet(White);
	LCD_TextColorSet(Red);

	LCD_DrawChar(0,0,12,6,0,'1');
	LCD_DrawChar(6,0,16,8,0,'2');
	LCD_DrawChar(14,0,24,16,0,'3');

	LCD_DrawPoint(100,10,Green);
	LCD_DrawPoint(100,20,Red);
	LCD_DrawPoint(100,30,Blue);
	
	LCD_DrawLine(150,10,100,Horizontal,Green);	
	LCD_DrawLine(150,20,100,Horizontal,Red);	
	LCD_DrawLine(150,30,100,Horizontal,Blue);	 
	
	LCD_DrawRect(50,50,20,20,Green);
	LCD_DrawRect(60,60,20,20,Red);
	LCD_DrawRect(70,70,20,20,Blue);
	
	LCD_DrawFillRect(150,50,20,20,Green);
	LCD_DrawFillRect(160,60,20,20,Red);
	LCD_DrawFillRect(170,70,20,20,Blue);	
	
	LCD_DrawString(0, 100, 12, 6, 0, "123456789ABCD");
	LCD_DrawString(0, 120, 16, 8, 0, "123456789ABCD");
  LCD_DrawString(0, 140, 24, 16, 0, "123456789ABCD");
	 
	LCD_DrawBigChar(0, 200, 0, 2);


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


/*********************************************************************************************************//**
  * @brief  Demo1.
  * @retval None
  ***********************************************************************************************************/
void Demo_part(LCD_DISPLAY_FrameInfoTypeDef* qq)
{
    LCD_DISPLAY_InitTypedef init;
    //gUI.IsDemo1Update = FALSE;
    
    init.Mode             = LCD_DISPLAY_MODE_NORMAL;
    init.pFrameInfo       = qq;
    init.pImageRemapTable = NULL;
    init.ImageStartIndex  = gUI.Demo1_ShowPicID;
    init.ImageLength      = 1;
    init.FrameRate        = 0;
    LCD_DISPLAY_Init(&init);

    while(gLCD_DISPLAY.ImageCounter < init.ImageLength)
    {
      LCD_DISPLAY_Process();
    }
}

void Demo_full(void)
{
	LCD_DISPLAY_InitTypedef init;
  //gUI.IsDemo1Update = FALSE;
 // init.
	
  init.Mode             = LCD_DISPLAY_MODE_NORMAL;
 // init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_Full;
  init.pFrameInfo       = (LCD_DISPLAY_FrameInfoTypeDef*)&FrameInfo_pp;	
  init.pImageRemapTable = NULL;
  init.ImageStartIndex  = gUI.Demo1_ShowPicID;
  init.ImageLength      = 1;
  init.FrameRate        = 0;
  LCD_DISPLAY_Init(&init);

	while(gLCD_DISPLAY.ImageCounter < init.ImageLength)
  {
    LCD_DISPLAY_Process();
  }
}


//void 

#define PIN_XL_PORT		HT_GPIOD
#define PIN_XL_PIN		GPIO_PIN_1

#define PIN_XR_PORT		HT_GPIOA
#define PIN_XR_PIN		GPIO_PIN_1

#define PIN_YT_PORT		HT_GPIOA
#define PIN_YT_PIN		GPIO_PIN_0
	
#define PIN_YB_PORT		HT_GPIOD
#define PIN_YB_PIN		GPIO_PIN_2


#define READ_XR_PORT		GPIO_PA
#define READ_XR_PIN			AFIO_PIN_1

#define READ_YT_PORT		GPIO_PA
#define READ_YT_PIN		  AFIO_PIN_0


vu16 ADC_T;
vu8 FLAG_ADC_END;

Touch_Screen_Enum Tocuh_Sreen_ADC_CH;
u8 flag_Read_pin = 0;//1 : X   ;  2 : Y
void Read_X_PIN(void)//ADCy
{	
	if(flag_Read_pin != 1)
	{
		flag_Read_pin = 1;
		AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_GPIO);//Y+
		GPIO_DriveConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DV_8MA); //Y+ HIGH
		GPIO_DirectionConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_YT_PORT, PIN_YT_PIN, SET); 
		
		GPIO_DriveConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DV_8MA); //Y- LOW
		GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_YB_PORT, PIN_YB_PIN, RESET); 	
		
		GPIO_PullResistorConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_PR_DISABLE);	//X- FLOT
		GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_IN);

		AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_ADC);//X+ ADC
	}
}


void Read_Y_PIN(void)//ADCx
{	
	if(flag_Read_pin != 2)
	{
		flag_Read_pin = 2;
		AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_GPIO);//X+
		GPIO_DriveConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DV_8MA); //X+ HIGH
		GPIO_DirectionConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_XR_PORT, PIN_XR_PIN, SET); 
		
		GPIO_DriveConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DV_8MA); //X- LOW
		GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_XL_PORT, PIN_XL_PIN, RESET); 	
		
		GPIO_PullResistorConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_PR_DISABLE);	//Y- FLOT
		GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_IN);
		
		AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_ADC);//Y+ ADC
	}
}

void Read_Z(void)
{	
	
	AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_GPIO);//Y+
  GPIO_DriveConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DV_8MA); //Y+ HIGH
  GPIO_DirectionConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DIR_OUT);
	GPIO_WriteOutBits(PIN_YT_PORT, PIN_YT_PIN, SET); 
	
  GPIO_PullResistorConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_PR_DISABLE);	//Y- FLOT
  GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_IN);
	
  GPIO_DriveConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DV_8MA); //X- LOW
  GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_OUT);
	GPIO_WriteOutBits(PIN_XL_PORT, PIN_XL_PIN, RESET); 	

	AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_ADC);//X+ ADC
	
}

void Read_Z2(void)
{	
	
//	AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_GPIO);//Y+
//  GPIO_DriveConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DV_8MA); //Y+ HIGH
//  GPIO_DirectionConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DIR_OUT);
//	GPIO_WriteOutBits(PIN_YT_PORT, PIN_YT_PIN, SET); 
//	
//  GPIO_PullResistorConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_PR_DISABLE);	//Y- FLOT
//  GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_IN);
//	
//  GPIO_DriveConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DV_8MA); //X- LOW
//  GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_OUT);
//	GPIO_WriteOutBits(PIN_XL_PORT, PIN_XL_PIN, RESET); 	

//	AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_ADC);//X+ ADC
//	
	
	AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_GPIO);//X+
  GPIO_PullResistorConfig(READ_XR_PORT, READ_XR_PIN, GPIO_PR_DISABLE);	//X- FLOT
  GPIO_DirectionConfig(READ_XR_PORT, READ_XR_PIN, GPIO_DIR_IN);	
	
	
	
  GPIO_DriveConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DV_8MA); //X+ HIGH
  GPIO_DirectionConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DIR_OUT);
	GPIO_WriteOutBits(PIN_XR_PORT, PIN_XR_PIN, SET); 
	
  GPIO_DriveConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DV_8MA); //X- LOW
  GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_OUT);
	GPIO_WriteOutBits(PIN_XL_PORT, PIN_XL_PIN, RESET); 	
	

	AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_ADC);//Y+ ADC
	
}

u16 ADC_Filer(vu16 *data,u8 num)
{
	u8 i,j;
	u32 sum = 0;
	
	for(i=0;i<num;i++)
	{
			for(j=i;j<num;j++)
			{
				if(data[i] > data[j])
				{
					sum = data[i];
					data[i] = data[j];
					data[j] = sum;
				
				}
			}	
	}
	
	sum = 0;
	
	for(i=num/2;i<num;i++)
	{
		sum += data[i];
	}
	sum = sum/(num-num/2);
	
	return (u16)sum;
}



void Touch_Screen_calibration(void)
{
	LCD_Clear(Red);
	LCD_TextColorSet(Green);
	LCD_BackColorSet(Red);
	
	LCD_DrawChar(50, 50, 24, 16, 0,'+');
	
	LCD_DrawChar(430, 50, 24, 16, 0,'+');
	LCD_DrawChar(50, 222, 24, 16, 0,'+');
	LCD_DrawChar(430, 222, 24, 16, 0,'+');
}


u16 ADC_READ_X(void)
{
	u16 data[6] ;
	u16 dataf = 0;
	u8 i;
	
	Read_X_PIN();
	
	Tocuh_Sreen_ADC_CH = READ_X;
	for(i=0;i<6;i++)
	{
		FLAG_ADC_END = RESET;
		while(FLAG_ADC_END == RESET);
		data[i] = ADC_T;
	}

	dataf = ADC_Filer(&data[0],6);
	
	return dataf;
}

u16 ADC_READ_Y(void)
{
	u16 data[6] ;
	u16 dataf = 0;
	u8 i;
	
	Read_Y_PIN();
	
	Tocuh_Sreen_ADC_CH = READ_Y;
	
	for(i=0;i<6;i++)
	{
		FLAG_ADC_END = RESET;
		while(FLAG_ADC_END == RESET);
		data[i] = ADC_T;
	}

	dataf = ADC_Filer(&data[0],6);
	
	return dataf;
}


int main(void)
{

	u8 j;
	int checksum = 0;
	
  CKCU_Configuration();               /* System Related configuration       */ 	
	GPIO_Configuration();
	NVIC_Configuration();
	SYSTICK_configuration();
	BFTM_Configuration();
//	RTC_Configuration();
	SPI_FLASH_Init();
// Initialize LCD related peripheral
  LCD_Init();
//// LCD driver configuration
  LCD_Config();	
	LCD_TEST();
	
	HT32F_DVB_PBInit(BUTTON_KEY1,BUTTON_MODE_GPIO);
	Delay(100);
	KEY_STATE = HT32F_DVB_PBGetState(BUTTON_KEY1);
	if(KEY_STATE == 0x00)
	{
		LCD_Clear(White);
	  LCD_BackColorSet(White);
	  LCD_TextColorSet(Red);
		LCD_DrawString(50, 100, 24, 16, 0, "USB_IVP DownLoad Bin...");
		LCD_DrawString(50, 140, 24, 16, 0, "MCU Reset to Ese");
		USB_Configuration();	
		IAP_Handler();	
	}	
	
//	KEY_Configuration();
	ADC_Configuration();
//	WIFI_INIT();


	KEY_STATE = 0;
	TEMP.En = 0;
	TEMP.SetEn = 0;
	TEMP.Now = 260;
	TEMP.Set = 260;

	
	LCD_Clear(Black);
	
	SPI_FLASH_BufferRead((u8*)&mCurrentSettings.cal[0], 0X3FFF000, sizeof(mCurrentSettings.cal));//Read cal
	
	for (j = 0; j < 7; j++)
		checksum += mCurrentSettings.cal[j];

	if(mCurrentSettings.cal[7] != checksum)
	{
	
		ExecCalibration(&mCurrentSettings);

		SPI_FLASH_SectorErase(0X3FFF000);
		SPI_FLASH_BufferWrite((u8*)&mCurrentSettings.cal[0], 0X3FFF000, sizeof(mCurrentSettings.cal));//Read cal
	}

	
	LCD_Clear(Black);
	

	
	while(1)
	{

		if(FLAG_10mS)
		{
			FLAG_10mS = 0;
		//	WIFI_CAP();
		TOUCH_Exec(&Tocuh);
			LCD_DrawFillRect(Tocuh.x,Tocuh.y,5,5,Blue);
		}		 
		 	
		if(FLAG_20mS)
		{
			FLAG_20mS = 0;
				
			
			
//			KEY_Scan();
		}
		
		if(FLAG_500mS)
		{
			FLAG_500mS = 0;
			
			Display_Temp();
			
//			WIFI_Control();
//			UPDATA();
			
				
		}
		
//		if(FLAG_1S) //1s
//		{
//			FLAG_1S = 0;
//			TEMP.Time++;

//		}
	}	
//=======
//	 FLAG_IMG = LCD_DISPLAY_GetImageInfo();

//	LCD_Clear(Black);
//	
//	while(1)
//	{
//			for(i=0; i< gLCD_Display_ImageInfo.Count; i++)
//			{
//				gUI.Demo1_ShowPicID = i;
//				Demo_full();
//				Delay(15);
//			}
//			Delay(500);
//	}		

//>>>>>>> master

}	

/*********************************************************************************************************//**
  * @brief  Configure the system clocks.
  * @retval None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.PDMA       = 1;
  CKCUClock.Bit.USBD       = 1;
  CKCUClock.Bit.CKREF      = 0;
  CKCUClock.Bit.EBI        = 1;
  CKCUClock.Bit.CRC        = 1;
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 1;
  CKCUClock.Bit.I2C0       = 0;
  CKCUClock.Bit.I2C1       = 0;
  CKCUClock.Bit.SPI0       = 1;
  CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 1;
  CKCUClock.Bit.USART1     = 1;
  CKCUClock.Bit.UART0      = 0;
  CKCUClock.Bit.UART1      = 0;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 1;
  CKCUClock.Bit.I2S        = 0;
  CKCUClock.Bit.MCTM0      = 0;
  CKCUClock.Bit.MCTM1      = 0;
  CKCUClock.Bit.WDT        = 0;
  CKCUClock.Bit.BKP        = 1;
  CKCUClock.Bit.GPTM0      = 0;
  CKCUClock.Bit.GPTM1      = 0;
  CKCUClock.Bit.BFTM0      = 1;
  CKCUClock.Bit.BFTM1      = 0;
  CKCUClock.Bit.CMP        = 0;
  CKCUClock.Bit.ADC        = 1;
  CKCUClock.Bit.SDIO       = 0;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /*--------------------------------------------------------------------------------------------------------*/
  /* Set USB Clock as PLL / 2                                                                               */
  /*--------------------------------------------------------------------------------------------------------*/
//  CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV2);
}

/*********************************************************************************************************//**
  * @brief  Configures USB.
  * @retval None
  ***********************************************************************************************************/
void USB_Configuration(void)
{
 // CKCU_SetUSBPrescaler(HTCFG_USBPRE_DIV);
	CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV2);

  gUSBCore.pDriver = (u32 *)&gUSBDriver;
  gUSBCore.Power.CallBack_Suspend.func  = NULL;
  //USBCore.Power.CallBack_Suspend.uPara = 0;

  USBDDesc_Init(&gUSBCore.Device.Desc);
  USBDClass_Init(&gUSBCore.Class);
  USBDCore_Init(&gUSBCore);

  NVIC_EnableIRQ(USB_IRQn);
  HT32F_DVB_USBConnect();
  USBDCore_MainRoutine(&gUSBCore);
}

/* Function Configuration-----------------------------------------------------------------------------*/

void GPIO_Configuration(void)
{
	


}

void KEY_Configuration(void)
{
  /*--------------------------------------------------------------------------------------------------------*/
  /* Configure EXTI for KEY                                                                      */
  /*--------------------------------------------------------------------------------------------------------*/
//	HT32F_DVB_PBInit(BUTTON_WAKEUP,BUTTON_MODE_EXTI);	
	HT32F_DVB_PBInit(BUTTON_KEY1,BUTTON_MODE_EXTI);
	HT32F_DVB_PBInit(BUTTON_KEY2,BUTTON_MODE_EXTI);
	HT32F_DVB_PBInit(BUTTON_KEY3,BUTTON_MODE_EXTI);
	HT32F_DVB_PBInit(BUTTON_KEY4,BUTTON_MODE_EXTI);
}

void USB_GPIO_Configuration(void)
{
	HT32F_DVB_PBInit(BUTTON_KEY1,BUTTON_MODE_EXTI);
	HT32F_DVB_LEDInit(HT_LED1);
	HT32F_DVB_LEDOff(HT_LED1);
}


void ADC_Configuration(void)
{
  /* ADCLK frequency is set to 96/64 MHz = 1.125MHz                                                         */
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

  /* Config AFIO mode as ADC function            
	*/
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_9, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_10, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_11, AFIO_FUN_ADC);
	
	AFIO_GPxConfig(GPIO_PA,AFIO_PIN_0, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PA,AFIO_PIN_1, AFIO_FUN_ADC);
	
  /* Continuous Mode, Length 5, SubLength 1                                                                 */
  ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 5, 1);

  /* ADC Channel n, Rank 0, Sampling clock is (1.5 + n) ADCLK
     Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_8, 0, 2);//for NTC1
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_9, 1, 2);//for for NTC2
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_10, 2, 2);//for for NTC3
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_0, 3, 3); //for LCD-Touch
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_1, 4, 3); //for LCD-Touch
			
  /* Use Software Trigger as ADC trigger source                                                             */
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);
	
  /* Enable ADC CYC end of conversion interrupt,*/
  ADC_IntConfig(HT_ADC, ADC_INT_CYCLE_EOC , ENABLE);//| ADC_INT_SINGLE_EOC

	NVIC_EnableIRQ(ADC_IRQn);
	
	ADC_Cmd(HT_ADC, ENABLE); 
	
  /* Software trigger to start continuous mode                                                              */
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);

	
}




void ADC_PDMA_Configuration(void)
{
	PDMACH_InitTypeDef PDMACH_InitStructure;
	/* Configure Timer Channel n as PWM output mode used to represent potentiometer's level                   */
	
  /* Configure PDMA channel to move ADC result from ADC->DR[0] to DATA                          */
  PDMACH_InitStructure.PDMACH_SrcAddr = (u32)(&HT_ADC->DR[0]);
  PDMACH_InitStructure.PDMACH_DstAddr = (u32)(&ADC_DATA[0]);
  PDMACH_InitStructure.PDMACH_BlkCnt = 5;
  PDMACH_InitStructure.PDMACH_BlkLen = 1;
  PDMACH_InitStructure.PDMACH_DataSize = WIDTH_32BIT;
  PDMACH_InitStructure.PDMACH_Priority = L_PRIO;
  //PDMACH_InitStructure.PDMACH_AdrMod = SRC_ADR_FIX | DST_ADR_FIX | AUTO_RELOAD;
	PDMACH_InitStructure.PDMACH_AdrMod = SRC_ADR_LIN_INC|DST_ADR_LIN_INC| AUTO_RELOAD;

  PDMA_Config(PDMA_CH0, &PDMACH_InitStructure);
  PDMA_EnaCmd(PDMA_CH0, ENABLE);

  /* ADCLK frequency is set to 96/64 MHz = 1.125MHz                                                         */
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

  /* Config AFIO mode as ADC function            
	*/
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_9, AFIO_MODE_2);
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_10, AFIO_MODE_2);
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_11, AFIO_MODE_2);
	
	AFIO_GPxConfig(GPIO_PA,AFIO_PIN_0, AFIO_MODE_2);
	AFIO_GPxConfig(GPIO_PA,AFIO_PIN_1, AFIO_MODE_2);
	
  /* Continuous Mode, Length 1, SubLength 1                                                                 */
  ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 5, 1);

  /* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
     Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_8, 0, 0);
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_9, 1, 0);
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_10, 2, 0);
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_0, 3, 1);
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_1, 4, 1);
			
	
  /* Use Software Trigger as ADC trigger source                                                             */
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);
	
  /* Issue ADC DMA request when cycle end of conversion occur                                               */
  ADC_PDMAConfig(HT_ADC, ADC_PDMA_REGULAR_CYCLE, ENABLE);
	
  /* Enable ADC single end of conversion interrupt,
     The ADC ISR will store the ADC result into global variable gPotentiometerLevel. */
  //ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);
	ADC_Cmd(HT_ADC, ENABLE); 
	
  /* Software trigger to start continuous mode                                                              */
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);

}

/*********************************************************************************************************//**
 * @brief Configures the RTC.
 * @retval None
 * @details RTC configuration as following:
 *  - S/W reset backup domain.
 *  - Configure Low Speed External clock (LSE).
 *    - The LSE use fast startup mode.
 *      Shortly startup time but higher power consumption.
 *    - Enable the LSE.
 *    - Waits till LSE is ready.
 *  - Configure RTC.
 *    - Select LSE as RTC Clock Source.
 *    - Enable the RTC Second interrupt.
 *    - RTC prescaler = 32768 to generate 1 second interrupt.
 *    - Enable clear RTC counter by match function.
 *
 ************************************************************************************************************/
void RTC_Configuration(void)
{
  /* Reset Backup Domain                                                                                    */
  PWRCU_DeInit();

  /* Configure Low Speed External clock (LSE)                                                               */
  RTC_LSESMConfig(RTC_LSESM_FAST);
  RTC_LSECmd(ENABLE);
  while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);

  /* Configure the RTC                                                                                      */
  RTC_ClockSourceConfig(RTC_SRC_LSE);
  RTC_IntConfig(RTC_INT_CSEC, ENABLE);
 // RTC_SetPrescaler(RTC_RPRE_32768);
	
//	RTC_SetPrescaler(RTC_RPRE_16384);
  /* Restart counter when match event occurred                                                              */
  RTC_CMPCLRCmd(ENABLE);
	
 /* Enable NVIC RTC interrupt                                                                              */
  NVIC_EnableIRQ(RTC_IRQn);
	

	/* Reset RTC Counter when Time is 23:59:59                                                              */
	RTC_SetCompare(86400);

	 /* Enable RTC               
                                                                           */
  RTC_Cmd(ENABLE);

}

void BFTM_Configuration(void)
{
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000);
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
	NVIC_EnableIRQ(BFTM0_IRQn);
}



/*********************************************************************************************************//**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @return Enter time, unit is second within a day.
  ***********************************************************************************************************/

static vu32 DelayTime;
/*********************************************************************************************************//**
  * @brief  SYSTICK configuration.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void SYSTICK_configuration(void)
{
	  /* SYSTICK configuration */
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8
  SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms on chip
  SYSTICK_IntConfig(ENABLE);                  // Enable SYSTICK Interrupt
}


/*********************************************************************************************************//**
  * @brief  Insert NVIC
  * @retval None
  ***********************************************************************************************************/
/* Private constants ---------------------------------------------------------------------------------------*/
#define NVIC_PRIORITY_GROUP_3          3 /* Preemption: 4 bits / Subpriority: 0 bits                        */
#define NVIC_PRIORITY_GROUP_4          4 /* Preemption: 3 bits / Subpriority: 1 bits                        */
#define NVIC_PRIORITY_GROUP_5          5 /* Preemption: 2 bits / Subpriority: 2 bits                        */
#define NVIC_PRIORITY_GROUP_6          6 /* Preemption: 1 bits / Subpriority: 3 bits                        */
#define NVIC_PRIORITY_GROUP_7          7 /* Preemption: 0 bits / Subpriority: 4 bits                        */
																				 /* Preemption: ìåÅ◊Éûœ»ºâ / Subpriority: Ìëë™Éûœ»ºâ                */
void NVIC_Configuration(void)
{
  /* Configure 3 bits for preemption priority 1 bits for subpriority                                        */
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4);

  /* Configure the Exception Handler Interrupts Priority                                                    */
//  NVIC_SetPriority(SPI0_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_4, 1, 0));
//	NVIC_SetPriority(USB_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_4, 0, 0));
  NVIC_SetPriority(PDMACH0_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_4, 0, 0));
	NVIC_SetPriority(ADC_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_4, 0, 0));
  /* Set pending bits at the same time                                                                      */
  NVIC_SetPendingSystemHandler(SYSTEMHANDLER_NMI | SYSTEMHANDLER_PSV | SYSTEMHANDLER_SYSTICK);
	

}


/*********************************************************************************************************//**
  * @brief  Insert Delay time.
  * @param  nTime: Insert Delay time.
  * @retval None
  ***********************************************************************************************************/
void Delay(u32 nTime)
{
  /* Enable the SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

  DelayTime = nTime;

  while(DelayTime != 0);

  /* Disable SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
  /* Clear SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

/*********************************************************************************************************//**
  * @brief  Decrements the TimingDelay variable.
  * @retval None
  ***********************************************************************************************************/
void TimingDelay(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}




#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
		;
  }
}
#endif

