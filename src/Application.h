#ifndef _APPLICATION_H
#define _APPLICATION_H

#define _CRTDBG_MAP_ALLOC

#include "OWindow.h"
#include "GResManager.h"
#include "IGame.h"
#include "IGRender.h"

#include <time.h>

// размеры окна:
#define WIND_W 1024
#define WIND_H 768

class IGFontManager;

//----------------------------------------------------------------------------------------
// Класс главного окна
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

	int		say(int what, int data = 0);							//!< послать объекту информацию data типа what от объекта from
	enum {
		SAY_UPDATE_WINDOW										//!< необходимо обновить окно (например для воспр. видео и поля ввода в полноэкр режиме)
	};
	
private:
	int32  OnCreate();                                           //!< Вызывается один раз при создании окна.
	void OnClose();                                           //!< Вызывается один раз при уничтожении окна.

	void OnTimer(int32 dTime);                               //!< Будет вызываться с периодм iPeriod ms
	void OnMouseMove(Pixel mouse_pos);                        //!< Вызывается при движении мыши
	void OnLMouseDown(Pixel);                                 //!< Вызывается если левая кнопка мыши нажимается
	void OnLMouseUp(Pixel);                                 //!< Вызывается если левая кнопка мыши отжимается
	void OnMouseWheelDown(Pixel);
	void OnMouseWheelUp(Pixel);
	void OnKeyDown(uint32 key);                                //!< Нажата клавиша
	void OnKeyUp(uint32 key);                                //!< Отжата клавиша
	void SetFullScreen(bool aFullScreen);						      //!< изменить полноэкранный режим
	void onActivate();											         //!< Вызывается при активации окна
	void onDeactivate();										            //!< Вызывается при деактивации окна
	void onMinimize();											         //!< Вызывается при сворачивании окна
	void     onMaximize();											      //!< Вызывается при разворачивании окна

	void updateWindowParams();									//!< обновить параметры окна

	static Application*	_instance;								//!< экземпляр класса
	IGame*            mGame;
	IGRender *		mRenderContext;                            //!< Рендер
	IGFontManager*	_systemFont;							//!< Менеджер текста
	//IGRender *		_ctx;                            //!< Рендер
	//GResImg *		_clips;                          //!< Менеджер графических ресурсов
	int 			_screenWidth;								//!< Ширина экрана
	int			_screenHeight;								//!< Высота экрана	
	LONG			_windowStyles;
	WINDOWPLACEMENT _windowPlacement;
	BOOL			_screenSaveActive;
	bool			_webBrowserIsVisible;
	bool			_isMinimize;								//!< окно свернуто в трей

	GMouse _currMouse;											//!< Текущее попложение мыши

	bool	_showSplashScreen;									//!< в текущий момент отображается splashscreen
	uint32	_spashScreenTextureID;								//!< id текстуры splashscrren
};

#endif