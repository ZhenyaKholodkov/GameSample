
#include "Application.h"
#include <windows.h>
#include <stdio.h>

bool checkInstance();
int subWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT32 mode);

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT32 mode)
{
	if (checkInstance())
		return 0;
	return subWinMain(hInstance, hPrevInstance, lpCmdLine, mode);
}

int subWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT32 mode)
{
	Application wnd;
	
	char debbuf[256] = "";
	int winWConf = 1024;
	int winHConf = 768;
	
	int screenW = ::GetSystemMetrics(SM_CXSCREEN);
	int screenH = ::GetSystemMetrics(SM_CYSCREEN);
	
	float aspW = 1.0f;
	float aspH = 1.0f;
	
	if (winWConf>screenW)
	{
		aspW = (float)screenW / (float)winWConf;
	}
	
	if (winHConf>screenH)
	{
		aspH = (float)screenH / (float)winHConf;
	}
	
	float asp = min(aspW, aspH);
	
	if (asp<1.0f)
		asp *= 0.8;						// делаем немного меньше, чтоб вместилось меню пуск
	
	winWConf = (int)(winWConf*asp);
	winHConf = (int)(winHConf*asp);
	
	wnd.screenWidth(winWConf);
	wnd.screenHeight(winHConf);
	
	//узнаем реальную ширину и высоту окна при размере области вывода WIND_W х WIND_H
	//int aWindowWidth  = WIND_W; //2 * GetSystemMetrics(SM_CXBORDER) + 2 * GetSystemMetrics(SM_CXDLGFRAME) + WIND_W + 10;
	//int aWindowHeight = WIND_H; //2 * GetSystemMetrics(SM_CXBORDER) + 2 * GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION) + WIND_H + 10;
	
	if (!wnd.Create(WindowCaption, "AbsViewerClass", (int)wnd.screenWidth(), (int)wnd.screenHeight(), mode)) return 0;
	
	wnd.Period(5);                                              // период таймера = 5 ms
	wnd.Show();                                                 // показываем окно
	int retVal = wnd.Run();                                     // запускаем петлю сообщений

	return retVal;
}

bool checkInstance()
{
	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, 0, WindowMutex);
	if (!hMutex)
	{
		hMutex = ::CreateMutex(0, 0, WindowMutex);
		return false;
	}
	else
	{
		HWND hWnd = ::FindWindow("AbsViewerClass", WindowCaption);
		::ShowWindow(hWnd, SW_SHOW);
		::SetForegroundWindow(hWnd);
		return true;
	}
}