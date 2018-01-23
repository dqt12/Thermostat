#ifndef __QUEUE_H_  
#define __QUEUE_H_  

#include "ht32.h"
#include <stdio.h>  
#include <stdlib.h>  

#ifdef __cplusplus
extern "C" {
#endif

typedef char 							QElemtype;//隊列元素
typedef unsigned char	 		QLentype;//隊列長度變量類型（u8 max =255,u16 max = 65535;）

typedef struct
{
    QElemtype *pbase; //隊列數組指針
    QElemtype front;	//隊頭
    QElemtype rear;		//隊尾	
		QLentype maxsize;	//隊長
	
}SqQueue,*PQueue;

typedef enum 
{
	false = 0,
	true = 1,
}status;

SqQueue *CreateQueue(QLentype maxsize) ;// 堆分配一個隊列(全部)
status DestoryQueue(SqQueue *Q);//  堆釋放一個隊列(全部)

status CreateQueueStr(SqQueue *Q,QLentype maxsize); // 堆分配一個隊列*(存儲數組）
status DestoryQueueStr(SqQueue *Q); //  堆釋放一個隊列(存儲數組）

void InitQueue(SqQueue *Q,QLentype maxsize,QElemtype *buff) ; // 將數組初始化為循環隊列

status EnQueue(SqQueue *Q,QElemtype e);// 隊列 入隊
status DeQueue(SqQueue *Q,QElemtype **e);// 隊列 出隊
status FullQueue(SqQueue *Q);// 隊列 檢查是否隊滿
status EmptyQueue(SqQueue *Q);// 隊列 檢查是否隊空
QLentype QueueLength(SqQueue *Q);// 隊列 檢查檔前隊列長度

char *StrQueue(SqQueue *Q,char *Str);// 隊列Q和數組Str比較，如匹配，則返回最後byte指針，如不匹配則返回NULL。
status CpyQueue(SqQueue *Q,char *Str,QLentype len);//隊列Q複製到數組Str，len個長度

#ifdef __cplusplus
}
#endif



#endif  

