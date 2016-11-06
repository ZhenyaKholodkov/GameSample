#ifndef _APPLICATION_H
#define _APPLICATION_H

#define _CRTDBG_MAP_ALLOC

#include "OWindow.h"
#include "GResManager.h"
#include "IGame.h"
#include "IGRender.h"

#include <time.h>

// ������� ����:
#define WIND_W 1024
#define WIND_H 768

class IGFontManager;

//----------------------------------------------------------------------------------------
// ����� �������� ����
//
class Application : public Window
{
public:
	Application();
	~Application();

	static  Application* GetInstance() { return _instance; }
	HWND	GetMainWndHandle() { return iMainHWND; }
	//HWND	GetGameWndHandle()			{return iGameHWND;}

	void	screenWidth(int val) { _screenWidth = val; }
	void	screenHeight(int val) { _screenHeight = val; }
	int	screenWidth() { return _screenWidth; }
	int	screenHeight() { return _screenHeight; }

	int		say(int what, int data = 0);							//!< ������� ������� ���������� data ���� what �� ������� from
	enum {
		SAY_UPDATE_WINDOW										//!< ���������� �������� ���� (�������� ��� �����. ����� � ���� ����� � �������� ������)
	};
	
private:
	int32  OnCreate();                                           //!< ���������� ���� ��� ��� �������� ����.
	void OnClose();                                           //!< ���������� ���� ��� ��� ����������� ����.

	void OnTimer(int32 dTime);                               //!< ����� ���������� � ������� iPeriod ms
	void OnMouseMove(Pixel mouse_pos);                        //!< ���������� ��� �������� ����
	void OnLMouseDown(Pixel);                                 //!< ���������� ���� ����� ������ ���� ����������
	void OnLMouseUp(Pixel);                                 //!< ���������� ���� ����� ������ ���� ����������
	void OnMouseWheelDown(Pixel);
	void OnMouseWheelUp(Pixel);
	void OnKeyDown(uint32 key);                                //!< ������ �������
	void OnKeyUp(uint32 key);                                //!< ������ �������
	void SetFullScreen(bool aFullScreen);						      //!< �������� ������������� �����
	void onActivate();											         //!< ���������� ��� ��������� ����
	void onDeactivate();										            //!< ���������� ��� ����������� ����
	void onMinimize();											         //!< ���������� ��� ������������ ����
	void     onMaximize();											      //!< ���������� ��� �������������� ����

	void updateWindowParams();									//!< �������� ��������� ����

	static Application*	_instance;								//!< ��������� ������
	IGame*            mGame;
	IGRender *		mRenderContext;                            //!< ������
	IGFontManager*	_systemFont;							//!< �������� ������
	//IGRender *		_ctx;                            //!< ������
	//GResImg *		_clips;                          //!< �������� ����������� ��������
	int 			_screenWidth;								//!< ������ ������
	int			_screenHeight;								//!< ������ ������	
	LONG			_windowStyles;
	WINDOWPLACEMENT _windowPlacement;
	BOOL			_screenSaveActive;
	bool			_webBrowserIsVisible;
	bool			_isMinimize;								//!< ���� �������� � ����

	GMouse _currMouse;											//!< ������� ���������� ����

	bool	_showSplashScreen;									//!< � ������� ������ ������������ splashscreen
	uint32	_spashScreenTextureID;								//!< id �������� splashscrren
};

#endif