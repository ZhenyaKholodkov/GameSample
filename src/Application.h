#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "OWindow.h"
#include "GResManager.h"
#include "IGame.h"
#include "gRenderManager.h"

#include <time.h>

#define WIND_W 1024
#define WIND_H 768

class IGFontManager;

class Application : public Window
{
public:
	Application();
	~Application();

	static  Application* GetInstance() { return sInstance; }
	HWND	GetMainWndHandle()         { return mMainHWND; }

	void	screenWidth(int val)  { mScreenWidth = val;   }
	void	screenHeight(int val) { mScreenHeight = val;  }
	int	    screenWidth()         { return mScreenWidth;  }
	int	    screenHeight()        { return mScreenHeight; }

	
private:
	int32  OnCreate();                                      
	void   OnClose();                                         

	void OnTimer(int32 dTime);                              
	void OnMouseMove(Pixel mouse_pos);                      
	void OnLMouseDown(Pixel);                               
	void OnLMouseUp(Pixel);                                 
	void OnMouseWheelDown(Pixel);
	void OnMouseWheelUp(Pixel);
	void OnKeyDown(uint32 key);                             
	void OnKeyUp(uint32 key);                               
	void onActivate();										
	void onDeactivate();									
	void onMinimize();										
	void onMaximize();	

	GKey convertToGKey(uint32 key);
private:
	static Application*	sInstance;							
	IGame*           mGame;
	int 			mScreenWidth;								
	int			    mScreenHeight;	

	bool            mCurrentMousePressed;
};

#endif