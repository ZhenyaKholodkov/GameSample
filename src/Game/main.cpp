
#include "GGame.h"
#include <windows.h>
#include <stdio.h>

#include "Utils.h"
#include "GDefines.h"

//#include <vld.h>


INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT32 mode)
{	
	GGame game;
	if (!game.createWindow("GameSample", "GameSample", WIDTH, HEIGHT, mode))
	{
		return 0;
	}

	game.setPeriod(5);                                   
	return game.runGameLoop();
}