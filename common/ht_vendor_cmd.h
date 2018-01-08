#ifndef		HT_VENDOR_CMD_H 
#define		HT_VENDOR_CMD_H 

#include "usb_bulk_only_transport.h"

#define SBC_CMD_VENDOR_TEST												(0xF1)
#define SBC_CMD_ABABFBFB													(0xF2)
#define SBC_CMD_VENDOR_WRITE											(0xF3)
#define SBC_CMD_VENDOR_READ												(0xF4)
#define SBC_CMD_VENDOR_ERASE											(0xF5)
#define SBC_CMD_VENDOR_ALTERNATE_MODE							(0xF6)

void _SBC_CMDVendorTest(BOT_Media_TypeDef *pMedia, u8 *pCB);
			 void _SBC_ABABFBFB(BOT_Media_TypeDef *pMedia, u8 *pCB);
void _SBC_CMDVendorWrite(BOT_Media_TypeDef *pMedia, u8 *pCB);
void _SBC_CMDVendorRead(BOT_Media_TypeDef *pMedia, u8 *pCB);
void _SBC_CMDVendorErase(BOT_Media_TypeDef *pMedia, u8 *pCB);
void _SBC_CMDVendorAlternateMode(BOT_Media_TypeDef *pMedia, u8 *pCB);

#endif
