#ifndef __QUEUE_H_  
#define __QUEUE_H_  

#include "ht32.h"
#include <stdio.h>  
#include <stdlib.h>  

#ifdef __cplusplus
extern "C" {
#endif

typedef char 							QElemtype;//���Ԫ��
typedef unsigned char	 		QLentype;//����L��׃����ͣ�u8 max =255,u16 max = 65535;��

typedef struct
{
    QElemtype *pbase; //��Д��Mָ�
    QElemtype front;	//��^
    QElemtype rear;		//�β	
		QLentype maxsize;	//��L
	
}SqQueue,*PQueue;

typedef enum 
{
	false = 0,
	true = 1,
}status;

SqQueue *CreateQueue(QLentype maxsize) ;// �ѷ���һ�����(ȫ��)
status DestoryQueue(SqQueue *Q);//  ��ጷ�һ�����(ȫ��)

status CreateQueueStr(SqQueue *Q,QLentype maxsize); // �ѷ���һ�����*(�惦���M��
status DestoryQueueStr(SqQueue *Q); //  ��ጷ�һ�����(�惦���M��

void InitQueue(SqQueue *Q,QLentype maxsize,QElemtype *buff) ; // �����M��ʼ����ѭ�h���

status EnQueue(SqQueue *Q,QElemtype e);// ��� ���
status DeQueue(SqQueue *Q,QElemtype **e);// ��� ���
status FullQueue(SqQueue *Q);// ��� �z���Ƿ�ꠝM
status EmptyQueue(SqQueue *Q);// ��� �z���Ƿ�꠿�
QLentype QueueLength(SqQueue *Q);// ��� �z��nǰ����L��

char *StrQueue(SqQueue *Q,char *Str);// ���Q�͔��MStr���^����ƥ�䣬�t��������byteָᘣ��粻ƥ��t����NULL��
status CpyQueue(SqQueue *Q,char *Str,QLentype len);//���Q�}�u�����MStr��len���L��

#ifdef __cplusplus
}
#endif



#endif  

