#include <stdio.h>
#include <windows.h>
#include "b2event.h"

void main1(void);

void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	b2Interrupt();
}

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	SetTimer(NULL, 0, 1, TimeProc);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			TranslateMessage(&msg);    // 翻译消息  
			DispatchMessage(&msg);     // 分发消息  
		}
	}
	KillTimer(NULL, 10);
	return 0;
}



void main0(void);
int tim1 = 157;
int tim2 = 39;
void _main(void)
{
	static sign = 0;
	tim1 = 157;
	b2SendEvent(_main, &tim1);
	if (sign == 0)
	{
	 b2SendEvent(main0, &tim2);
	  sign = 1;
    }
	printf("_main\n");
}
void main0(void)
{
	tim2 = 39;
	b2SendEvent(main1,&tim2);

	printf("main1\n");
}
void main1(void)
{
	static int tim3 = 3;
    tim3 = 567;
	b2SendEvent(main0, &tim3);
	printf("main0\n");
}

void main(void)
{
	
	HANDLE t1 = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);

	b2InitSystem();
	b2Start(_main);
	
}
