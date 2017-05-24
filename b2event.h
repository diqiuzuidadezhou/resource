#ifndef       _B2EVENT_H
#define       _B2EVENT_H

enum {
	HIGH_PRIORITY = 0,
	DEFAULT_PRIORITY	 
};


//_b2event 的前面必须与b2event相同
typedef struct _evt {
	void(*proc)(void);
	void            *p;
	int             *t;
	char            para[4];
	struct _evt   *next;
}_b2event;

typedef struct {
	void(*proc)(void);
	void            *p;
	int             *t;
	char            para[4];
}b2event;


typedef struct {
	void(*proc)(void);
	int            bkp;
	int            count;
}taskBlock;




void b2PushQueue(b2event *e, int prio);
_b2event *b2PopQueue(void);
void  b2Interrupt(void);
void  b2Start( void(*_main)(void) );
void  b2InitSystem(void);
void  b2DeInitEvent(b2event *e);
void  b2SendEvent(void(*proc)(void), int *b2tim);
_b2event *current;
_b2event *head;
_b2event *rear;
_b2event *free_head;


#endif
