#ifndef __TOUCH_SCREEN_ADC_H_  
#define __TOUCH_SCREEN_ADC_H_    

#include "ht32.h"
#include "main.h"

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
	
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
	
	
#define ADC_TS_DATA_COUNT		          	6 //must n%2 = 0;
#define ADC_FILER_DATA_COUNT						2 //must n%2 = 0;
	
typedef enum
{
	READ_NULL,
	READ_X = 1,
	READ_Y = 2,
} Touch_Screen_CH_Enum;

typedef struct
{
	Touch_Screen_CH_Enum Channl;
	Touch_Screen_CH_Enum LastChannl;
	vu16 DATA;
	volatile bool isEND;
}ADC_TOUCH_SCREEN_TypeDef;

extern ADC_TOUCH_SCREEN_TypeDef ADC_TS;

u16 ADC_TS_READ_XY(Touch_Screen_CH_Enum ch);
void ADC_TS_Conversion(u8 X_ADC_DATA,u8 Y_ADC_DATA);

u16 ADC_Filer(u16 *data,u8 num);

bool ADC_TS_READ_INT(void);	
u16 ADC_TS_READ_X(void);
u16 ADC_TS_READ_Y(void);






#ifdef __cplusplus
}
#endif



#endif  










