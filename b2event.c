
#include "b2event.h"

#define   NULL             0
#define   EVENT_LIST_LEN   32

_b2event event_list[EVENT_LIST_LEN];

void main2(void)
{
	printf("main2\n");
}
taskBlock task_list[] = {
	{ main2,789,0}
};

void b2PushQueue(b2event *e, int prio)
{
    if(NULL == free_head)
       return;
    _b2event *p = free_head;
    free_head = free_head->next;
    
    int i;
    char *src = (char*)e,*dst = (char *)p;
    for(i=0;i<sizeof(b2event);i++)
    {
        *(dst+i) = *(src+i);
    }
	
	if (NULL != rear)
	{
		switch (prio)
		{
			case HIGH_PRIORITY:
				p->next = head;
				head = p;
				break;
			case DEFAULT_PRIORITY:
				p->next = NULL;
				rear->next = p;
				rear = p;
				break;
			default :
				break;
		}
		
		return;
	}
       p->next = NULL;
       rear = p;
       head = p;
	   return;
}

_b2event *b2PopQueue(void)
{
    _b2event **p = &head;
	
    while(NULL != *p)
    {
        if( *(*p)->t <= 0 )
        {	
			_b2event *tmp;
			tmp = *p;
			if (head == rear)
			{
				rear = NULL;
			}
			else if(*p == rear)
			{
				_b2event *pre = NULL;			
				rear = (_b2event *)((char *)p - &pre->next);
			
			}
			*p = (*p)->next;
            return tmp;
        }
        p = &(*p)->next;
    }
    return  NULL;
}

void b2AddfreeList(_b2event *e)
{
     e->next = free_head;
	 free_head = e;
}

void b2InitSystem(void)
{
    int   i;
    char  *p = (char *)event_list;
    
    if(0 == sizeof(event_list))
       return;
//初始化事件缓存，清0   
    for(i=0; i < sizeof(event_list); i++)
    {
        *(p+i) = 0;
    }
    
    _b2event *index = event_list;
    for(i=0;i<EVENT_LIST_LEN-1; i++)
    {
       (index+i)->next = index+i+1;      
    }
	free_head = event_list;
}

void  b2Interrupt(void)
{
    _b2event   **p = &head;
    int        *tim = NULL;
       //找到时间大于0的事件，依次递减
     while(NULL != *p)
     {
       tim = (*p)->t;
       if(0 < *tim)
       (*tim)--;
       p = &(*p)->next;
     }
 
    for(int i = 0; i<sizeof(task_list) / sizeof(taskBlock);i++)
    {
        if(0 < task_list[i].count)
        {
           task_list[i].count--;
        }
    }

}


void  b2Start( void(*_main)(void))
{
     b2event    evt;
     char       *index = &evt;
     int        i;
	 static  int t = 0;
     for(i=0;i<sizeof(b2event);i++)
     {
       *(index+i) = 0;
     }
     evt.proc = _main; 
	 evt.t = &t;
     b2PushQueue(&evt,DEFAULT_PRIORITY);
    
	 while (1)
	 {
		 //执行时间为0的函数
		 current = b2PopQueue();
		 if (NULL != current)
		 {
			 if (*(current->t) == 0)
				 current->proc();
			 b2AddfreeList(current);
			 current = NULL;
		 }
		
		 for(i = 0; i<sizeof(task_list)/sizeof(taskBlock);i++)
		 {
			 if(0 == task_list[i].count)
			 {
			   task_list[i].count = task_list[i].bkp;
			   task_list[i].proc();	   
			 }
		 }
		
	 } 
}

void  b2SendEvent(void(*proc)(void), int *b2tim)
{
	b2event e;
	e.proc = proc;
	e.t = b2tim;
	b2PushQueue(&e, DEFAULT_PRIORITY);
}


void  b2DeInitEvent(b2event *e)
{
	static const int tim = -1;
	char *p = (char *)e;
	for (int i = 0; i < sizeof(event_list); i++)
	{
		*(p + i) = 0;
	}
	e->t = &tim;
	e->proc = NULL;
}

























