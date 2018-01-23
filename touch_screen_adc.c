#include "touch_screen_adc.h"

volatile ADC_TOUCH_SCREEN_TypeDef  ADC_TS;
	
void Read_X_PIN(void)//ADCy
{	
	if(ADC_TS.LastChannl != READ_X)
	{
		ADC_TS.LastChannl = READ_X;
		AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_GPIO);//Y+
		GPIO_DriveConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DV_8MA); //Y+ HIGH
		GPIO_DirectionConfig(PIN_YT_PORT, PIN_YT_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_YT_PORT, PIN_YT_PIN, SET); 
		
		GPIO_DriveConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DV_8MA); //Y- LOW
		GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_YB_PORT, PIN_YB_PIN, SET); 	
		
		GPIO_PullResistorConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_PR_DISABLE);	//X- FLOT
		GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_IN);

		AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_ADC);//X+ ADC
	}
}


void Read_Y_PIN(void)//ADCx
{	
	if(ADC_TS.LastChannl != READ_Y)
	{
		ADC_TS.LastChannl = READ_Y;
		AFIO_GPxConfig(READ_XR_PORT,READ_XR_PIN, AFIO_FUN_GPIO);//X+
		GPIO_DriveConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DV_8MA); //X+ HIGH
		GPIO_DirectionConfig(PIN_XR_PORT, PIN_XR_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_XR_PORT, PIN_XR_PIN, SET); 
		
		GPIO_DriveConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DV_8MA); //X- LOW
		GPIO_DirectionConfig(PIN_XL_PORT, PIN_XL_PIN, GPIO_DIR_OUT);
		GPIO_WriteOutBits(PIN_XL_PORT, PIN_XL_PIN, SET); 	
		
		GPIO_PullResistorConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_PR_DISABLE);	//Y- FLOT
		GPIO_DirectionConfig(PIN_YB_PORT, PIN_YB_PIN, GPIO_DIR_IN);
		
		AFIO_GPxConfig(READ_YT_PORT,READ_YT_PIN, AFIO_FUN_ADC);//Y+ ADC
	}
}



bool ADC_TS_READ_INT(void)
{
	u16 dataf = 0;
	
	Read_X_PIN();
	
	ADC_TS.Channl = READ_X;

		ADC_TS.isEND = FALSE;
		while(ADC_TS.isEND == FALSE);
		dataf = ADC_TS.DATA;;


	if(dataf <= 4000 )
		return TRUE;

	return FALSE;
}

u16 ADC_TS_READ_XY(Touch_Screen_CH_Enum ch)
{
	u16 data[ADC_TS_DATA_COUNT];
	u16 dataf = 0;
	u8 i;
	
	if(ch == READ_X)
	{
		ADC_TS.Channl = READ_X;
		Read_X_PIN();
	}
	else if(ch == READ_Y)
	{
		ADC_TS.Channl = READ_Y;
		Read_Y_PIN();
	}
	else 
		return 0;
	
	for(i=0;i<ADC_TS_DATA_COUNT;i++)
	{
		ADC_TS.isEND = FALSE;
		while(ADC_TS.isEND == FALSE);
		data[i] = ADC_TS.DATA;
	}

	dataf = ADC_Filer(&data[0],ADC_TS_DATA_COUNT);
	
	return dataf;
}



u16 ADC_TS_READ_X(void)
{
	u16 data[6] ;
	u16 dataf = 0;
	u8 i;
	
	Read_X_PIN();
	ADC_TS.Channl = READ_X;
	
	for(i=0;i<6;i++)
	{
		ADC_TS.isEND = FALSE;
		while(ADC_TS.isEND == FALSE);
		data[i] = ADC_TS.DATA;
	}

	dataf = ADC_Filer(&data[0],6);
	
	return dataf;
}


u16 ADC_TS_READ_Y(void)
{
	u16 data[6] ;
	u16 dataf = 0;
	u8 i;
	
	Read_Y_PIN();
	ADC_TS.Channl = READ_Y;
	
	for(i=0;i<6;i++)
	{
		ADC_TS.isEND = FALSE;
		while(ADC_TS.isEND == FALSE);
		data[i] = ADC_TS.DATA;
	}

	dataf = ADC_Filer(&data[0],6);
	
	return dataf;
}

/*********************************************************************************************************//**
 * @brief Return the result of ADC regular channel conversion.
 * @param ADC_REGULAR_DATAn where x can be 0 ~ 15
 * @retval NONE
 ************************************************************************************************************/
void ADC_TS_Conversion(u8 X_ADC_DATA,u8 Y_ADC_DATA)
{
	if(ADC_TS.Channl == READ_X)
	{
		 ADC_TS.DATA = ADC_GetConversionData(HT_ADC,X_ADC_DATA);	
	}		
	else if(ADC_TS.Channl == READ_Y)
	{

		 ADC_TS.DATA = ADC_GetConversionData(HT_ADC,Y_ADC_DATA);	
	}
	
	ADC_TS.isEND  = TRUE;
}


/*********************************************************************************************************//**
 * @brief Return the result of ADC regular channel conversion.
 * @param ADC_REGULAR_DATAn where x can be 0 ~ 15
 * @retval NONE
 ************************************************************************************************************/
u16 ADC_Filer(u16 *data,u8 num)
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
	
	for(i=(ADC_FILER_DATA_COUNT/2);i<(num - ADC_FILER_DATA_COUNT/2);i++)
	{
		sum += data[i];
	}
	sum = sum/(num-ADC_FILER_DATA_COUNT);
	
	return (u16)sum;
}








