
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

	int winWConf = 1024;
	int winHConf = 768;

	winWConf = (int)(winWConf);
	winHConf = (int)(winHConf);
	
	wnd.screenWidth(winWConf);
	wnd.screenHeight(winHConf);
	
	if (!wnd.Create(WindowCaption, "AbsViewerClass", (int)wnd.screenWidth(), (int)wnd.screenHeight(), mode)) return 0;
	
	wnd.Period(5);                                        
	wnd.Show();                                           
	int retVal = wnd.Run();                               

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