#include "queue.h"  


SqQueue *CreateQueue(QLentype maxsize)  
{  
		SqQueue *Q = NULL;
	
	  Q = (SqQueue *)malloc(sizeof(SqQueue));  
		
		Q->pbase = (QElemtype *)malloc(sizeof(QLentype)*maxsize);
	
    Q->front = 0;         //
    Q->rear = 0;  
    Q->maxsize = maxsize;  
		
		return Q;
}  		

status DestoryQueue(SqQueue *Q)  
{  
	  free(Q);  
	
    if(NULL != Q)  
    {  
			return false;
    }  

		return true;
}  	


status CreateQueueStr(SqQueue *Q,QLentype maxsize)  
{  
	
	  Q->pbase = (QElemtype *)malloc(sizeof(QLentype)*maxsize);  
	
    if(NULL == Q->pbase)  
    {  
			return false;
    }  
    Q->front = 0;         //
    Q->rear = 0;  
    Q->maxsize = maxsize;  
		
		return true;
}  		


status DestoryQueueStr(SqQueue *Q)  
{  
	  free(Q->pbase);  
	
    if(NULL != Q->pbase)  
    {  
			return false;
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
	




