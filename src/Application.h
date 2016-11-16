#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "OWindow.h"
#include "GResManager.h"
#include "IGame.h"
#include "IGRender.h"

#include <time.h>

#define WIND_W 1024
#define WIND_H 768

class IGFontManager;

class Application : public Window
{
public:
	Application();
	~Application();

	static  Application* GetInstance() { return _instance; }
	HWND	GetMainWndHandle()         { return iMainHWND; }

	void	screenWidth(int val)  { _screenWidth = val;   }
	void	screenHeight(int val) { _screenHeight = val;  }
	int	    screenWidth()         { return _screenWidth;  }
	int	    screenHeight()        { return _screenHeight; }

	
private:
	int32  OnCreate();                                      
	void OnClose();                                         

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

	static Application*	_instance;							
	IGame*           mGame;
	IGRender *		 mRenderContext;                            
	int 			_screenWidth;								
	int			    _screenHeight;								
};

#endif