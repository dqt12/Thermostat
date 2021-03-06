#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "ht32.h"
#include "stdlib.h"
#include "queue.h" 

#ifdef __cplusplus
 extern "C" {
#endif

//USE PC12 FOR WIFI RESET
#define WIFI_RST_PORT   HT_GPIOC
#define WIFI_RST_PIN    GPIO_PIN_12

#define USE_WIFI_AUTOLINK
//#define USE_WIFI_LIST_DISPLAY
	 
#ifdef 	 USE_WIFI_LIST_DISPLAY
#define  WIFI_DISPLAY(x)			Display_State(x)
#endif
	 
typedef enum 
{
    STA = 1,
    AP,
    STA_AP,
	
}WIFI_MODE_TypeDef;

//接收模塊結構體
typedef struct 
{
	char *CMD; //發送命令
	char *ACK; //等待返回
	
	bool WAIT_EN;  //是否啟動等待
	u8 WAIT_MAXTIME; //等待最長時間nS
	u8 WAIT_1SCNT;	//等待計數
	
	//接收模塊當前狀態
	enum 
	{
		CENABLE,
		CDISABLE,
		CSUCCESS,
		CTIME_OUT,
	}STA;	
	
	//接收模塊方式
	enum 
	{
		FREE = 0X00,
		SEND = 0x01,
		REC = 0x02,
		SENDREC = SEND|REC,
	}MODE;

	bool JUMP;	//跳過亂碼
	
}WIFI_REC_TypeDef ;
extern WIFI_REC_TypeDef WIFI_REC;//接收模模塊變量反映其狀態
//發送模塊流程表
typedef enum 
{
	L_READY,
	L_CHECK_LINK_1,
	L_CHECK_LINK_2,
	L_REST,
	L_5,
	L_6,
	L_7,
	L_8,
	L_SMART,
	L_10,
	L_LINKED,
	L_12,
	L_13,
	L_14,
	L_15,
	L_Dev,
	L_REC,
	L_SEND_1,
	L_SEND_2,
	L_SEND_3,
	L_NULL,
	CAP_STATUE = 100,
  CAP_DATA = 101,
}WIFI_CMD_CONT_TypeDef;

extern WIFI_CMD_CONT_TypeDef CMD_Cont;	 //發送模塊流程項目
extern WIFI_CMD_CONT_TypeDef CMD_Cont_Trg;

typedef struct 
{
	char *TxBuf;
	vu8 TxCont;
	vu8 RxFin ;
}WIFI_UART_TypeDef ;

extern WIFI_UART_TypeDef WIFI_UART;

////URAT使用

#define REC_BUF_SIZE		64  //接收隊列最大長度（30~64）
extern SqQueue WIFI_FRAM;   //接收數據隊列

typedef struct 
{
	bool AUTOLINK;//WIFI是否啟動自動連接
	bool SMARTLINK;//WIFI是否已經連接上路由
	bool APLINK;//WIFI是否已經連接上路由
	bool DEVLINK;//WIFI是否有設備連接
	bool UPDATA;//WIFI是否有接收到設備信息
}WIFI_FLAG_TypeDef ;

extern WIFI_FLAG_TypeDef FLAG_WIFI;

void WIFI_INIT(void);//WIFI初始化
void WIFI_HW_STA(FlagStatus sta);//WIFI 硬件復位
void WIFI_SendCMD(char *Str);//發送指令
void WIFI_WAIT_FUNC(void);

void WIFI_CMDSET(char *cmd,char *ack ,u16 waittime);//設置發送指令、回復信息、等待回復時間
void WIFI_LIST_POINT(WIFI_CMD_CONT_TypeDef type);//設置命令跳轉到接收模塊的類型
void WIFI_READY(void);
void WIFI_AUTOLINK(void);

extern char WIFI_IP[14];//WIFI IP地址
extern char WIFI_MAC[18];//WIFI MAC地址
bool WIFI_GotIP(char *mac,char *ip);//獲取WIFI MAC IP 地址

void WIFI_Control(void);//WIFI發送模塊（300ms~1s）

void WIFI_CAP_STATUE(void);//接收單一回復
void WIFI_CAP_DATA(void);//接收數據回復
void WIFI_CAP_CONNECT(void);//檢測設備是否斷開

void WIFI_CAP(void);//WIFI接收模塊（<20ms）

#ifdef __cplusplus
}
#endif


#endif

