#include <stdio.h>  
#include <stdlib.h>  
#include "queue.h"  


//status CreateQueue(SqQueue *Q,QLentype maxsize)  
//{  
//	//  Q->pbase = (int *)malloc(sizeof(int)*maxsize);  
//    if(NULL==Q->pbase)  
//    {  
//			return false;
//    }  
//    Q->front=0;         //
//    Q->rear=0;  
//    Q->maxsize=maxsize;  
//		
//		return true;
//}  		


status DestoryQueue(SqQueue *Q,QLentype Len)  
{  
	QLentype nowlen =  QueueLength(Q);
	if(nowlen < Len)
	{
		 return false;
	}
		else if (nowlen > Len )
		{
			Q->front = Q->front + Len; 
		}   
			else 
			{
				Q->front = 0;	
				Q->rear = 0; 
			}
			 
		return true;
}  	


void InitQueue(SqQueue *Q,QLentype maxsize,QElemtype *buff)  
{  
    Q->pbase = buff;  
    Q->front = 0;         
    Q->rear = 0;  
    Q->maxsize = maxsize;  
}  		
		
		
status EnQueue(SqQueue *Q,QElemtype e)
{
    if((Q->rear+1)%Q->maxsize==Q->front)
        return false;
    Q->pbase[Q->rear]=e;
    Q->rear=(Q->rear+1)%Q->maxsize;
    return true;
}

status DeQueue(SqQueue *Q,QElemtype **e)
{
    if(Q->front==Q->rear)
        return false;
    *e = &(Q->pbase[Q->front]);
    Q->front =(Q->front+1)%Q->maxsize;
    return true;
}

//QElemtype *DeQueue(SqQueue *Q)
//{
//		QElemtype *e;
//    e = &(Q->pbase[Q->front]);
//    Q->front =(Q->front+1)%Q->maxsize;
//		return e;
//}

status FullQueue(SqQueue *Q)
{
    if((Q->rear+1)%Q->maxsize==Q->front)
      return true;
		else 
			return false;
}

status EmptyQueue(SqQueue *Q)
{
    if(Q->front==Q->rear)
      return true;
		else 
			return false;
}

QLentype QueueLength(SqQueue *Q)
{
    return (Q->rear-Q->front+Q->maxsize)%Q->maxsize;
}


char *StrQueue(SqQueue *Q,QElemtype *Str)
{
   char *p1;
	 char *p2;
	
	if(*Str=='\0') return Q->pbase;
	
	p2 = Str;
	
	do
	{
			if(DeQueue(Q,&p1)) //取的第一個幀
			{
			
				if(*p1 == *p2) 
				{
					p2++;
					if(*p2 == '\0') 
					{
					//DeQueue(Q,&p1);
						p1++;
						return p1;
					}
						
				}
				else  
					p2 = Str;
				
			}
			else
			{
					return NULL;
			}
	}
	//while(*p1 != '\n');
	while(!EmptyQueue(Q));
	
		return NULL;

}


status CpyQueue(SqQueue *Q,QElemtype *Str,QLentype len)
{
	QElemtype *p = NULL;
	
		do
		{
			if(DeQueue(Q,&p)) //取的第一個幀
			{
				*(Str++) = *p;
			}
			else  
			{
				return false;
			}
			len--;
			
		}while(len);
		
		return true;
}
	




