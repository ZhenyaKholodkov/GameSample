
#include "GGame.h"
#include <windows.h>
#include <stdio.h>
#include "Utils.h"
#include "GMouseUpEventComponent.h"

//#include <vld.h>

const int windowWidth = 1024;
const int windowHeight = 768;

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT32 mode)
{
	GGame wnd;
	if (!wnd.createWindow("GameSample", "GameSample", windowWidth, windowHeight, mode)) 
	{
		return 0;
	}

	wnd.setPeriod(5);                                   
	return wnd.runGameLoop();
}