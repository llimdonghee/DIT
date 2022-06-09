#include "stdafx.h"
//#include "pch.h"

#include <MMSystem.h>
#pragma comment(lib, "winmm")

void Delay(DWORD timeout)
{
	timeBeginPeriod(1);

	MSG msg;
	DWORD endTick = timeGetTime() + timeout;

	while (timeGetTime() < endTick)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	timeEndPeriod(1);
}