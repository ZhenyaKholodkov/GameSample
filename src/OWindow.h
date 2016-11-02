//!                                    Класс окна в оконной системе.
/*!  \file Window.h

Для создания оконного приложения необходимо создать наследника от Window, переопределить
его событийные функции, такие как OnTimer, OnMouseMove, OnKeyUp и т.д.:

struct MainWindow: Window
{
void OnTimer     (int32 dTime);                               // Будет вызываться с периодм iPeriod ms
void OnMouseMove (Pixel mouse_pos);                        // Вызывается при движении мыши
};

int32 WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32 mode)
{
MainWindow wnd;
if(!wnd.Create("Main","MainWindow",800,600,mode)) return 0; // создаем окно 800x600
wnd.Period(5);                                              // период таймера = 5 ms
wnd.Show();                                                 // показываем окно
return wnd.Run();                                           // запускаем петлю сообщений
}

(!) Для работы с полноэкранным режимом необходимо подключить (Add file to project) в Builder
ddraw.lib из директории LIB Билдера.
(c) n-th.com (steps)  v.1.00   16-dec-2001..2005
*****************************************************************************************/
#ifndef OWindowH
#define OWindowH

#include "Engine\Types.h"

#include <windows.h>
#include <stdio.h>

#include <gl/gl.h>                                             // 3D рендеринг для openGL
#include <gl/glu.h>
#include <float.h>                                             // _control87() : only for Borland (floating error)

#ifdef _MSC_VER
// Компиляция производится Visual Studio:
#pragma comment (lib,"OpenGL32.lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.lib")
#pragma warning(disable: 4311)     // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312)     // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif


//========================================================================================
//! Класс окна в оконной смстеме.
//
struct Window
{
	Window();
	virtual ~Window();

	int  Create(char *caption, char *name, int32 w, int32 h, int32 mode);// Создать окно.

	int CreateGLContext();
	void DestroyGLContext();
	void UpdateContext();

	void Show();                                                // Показать окно
	int  Run();                                                 // Запустить петлю сообщений
	void Close();                                               // Закрыть окно

	void ClearWindow(Rect *rect = 0);                          // Очистка окна, если rect==0 то всего

	virtual int  OnCreate() { return 1; }//!< Вызывается один раз при создании окна.
	virtual void OnClose() {};         //!< Вызывается один раз при уничтожении окна.
	virtual void OnTimer(int) {};         //!< Вызывается с интервалом задаваемым iPeriod

	virtual void OnMouseMove(Pixel) {};         //!< Вызывается при движение мыши.
	virtual void OnLMouseDown(Pixel) {};         //!< Вызывается если левая кнопка мыши нажимается
	virtual void OnRMouseDown(Pixel) {};         //!< Вызывается если правая кнопка мыши нажимается
	virtual void OnLMouseUp(Pixel) {};         //!< Вызывается если левая кнопка мыши отжимается
	virtual void OnRMouseUp(Pixel) {};         //!< Вызывается если правая кнопка мыши отжимается
	virtual void OnDblClick(Pixel) {};         //!< Вызывается если было двойное нажатие клавиши
	virtual void OnMouseWheelDown(Pixel) {};
	virtual void OnMouseWheelUp(Pixel) {};

	virtual void OnKeyUp(uint32) {};							   //!< Вызывается если клавиша отжимается
	virtual void OnKeyDown(uint32) {};                            //!< Вызывается если клавиша нажимается
	virtual void OnChar(uint32) {};                            //!< Вызывается если нажатая клавиша была символом

	virtual void OnResize() {};         //!< Вызывается если было изменение размера экрана
	virtual void OnPaint(Rect *rect = 0) {};         //!< Вызывается на событие on_paint
	virtual void	SetFullScreen(bool aFullScreen) {};		   //!< Включить полноэкранный режим
	virtual void onActivate() {};		   //!< Вызывается при активации окна
	virtual void onDeactivate() {};		   //!< Вызывается при деактивации окна
	virtual void onMinimize() {};		   //!< Вызывается при сворачивании окна
	virtual void onMaximize() {};		   //!< Вызывается при разворачивании окна

	void   Caption(const char *format, ...);                  //  Установить заголовок окна (как printf!)
	void   SetFocus() { ::SetFocus(iMainHWND); }                 //!< Сделать окно активным

	void   Period(int32 period) { iPeriod = period; }              //!< Задать период вызова OnTimer в ms
	int32    Period() { return iPeriod; }            //!< Получить период вызова OnTimer в ms
	void   SlowTimer() { iSlowTimer = 1; SetTimer(iMainHWND, 1, Period(), 0); } //!< Установить флаг медленного таймера (перед Run!)

	int32    Time() { return timeGetTime(); };       //!< Получить текущее время.

	float  FPS() { return iFPS; }                               //!< Число кадров в секунду
	void   PeriodFPS(int32 period) { iPeriodFPS = period; }          //!< время для накопления fps-ов

	Pixel ScreenSize();                                        // Абсолютные размеры монитора

	void   Size(int32 w, int32 h);                                  // Изменить размер окна.
	Pixel Size() { return iSize; }                           //!< Размеры окна
	void   Pos(int32 x, int32 y);                               // Изменить положение окна.
	Pixel Pos() { return iPos; }                           //!< Положение окна
	Rect  GetRect() { return Rect(0, 0, iSize.W(), iSize.H()); }//!< Прямоугольник окна
	int32   Width() { return iSize.X(); }                       //!< ширина окна
	int32   Height() { return iSize.Y(); }                       //!< высота окна

	Pixel MousePos();                                        // Получить положение мыши

															  //void Show(OBitmap *bmp, int32 x=0, int32 y=0, Rect *r=0);    // вывести битмап в точке x,y с отсечение r

	void NoCaption(int32 no) { iNocaption = no; }              //!< нет заголовка
	void ColorBkground(OColor c) { iColorBkground = c; }           //!< установить цвет фона окна
																//----------------------------------------------------------------------- Не использовать !
	int32 OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Window* GetInstance();

	HDC GetHDC() { return GetDC(iMainHWND); }

	HGLRC GetOpenGlContext() { return mHGLRC; }

protected:
	Pixel iSize;                                               //!< ширина, высота окна
	Pixel iPos;                                                //!< положение окна на экране

	Int32  iPeriod;                                             //!< перидичность вызова фу-ии OnTimer
	char   iCaption[1024];                                      //!< заголовок окна

	OColor iColorBkground;                                      //!< цвет фона
	int32    iNocaption;                                          //!< нет заголовка
	int32    iNoresize;                                           //!< не может изменять размеры
	float  iFPS;                                                //!< число кадров в секунду
	Int32  iPeriodFPS;                                          //!< время для накопления fps-ов
	int32    iSlowTimer;                                          //!< таймер запускается не по Run а по WM_TIMER

	HWND   iMainHWND;                                           //! хендел главного окна.
																//HWND   iGameHWND;										   //! хендел игрового окна.
	int32    iWndMode;
	int32    iLastTime;                                           //! время с момента запуска системы
	int32    iLastID;                                             //! для нумерации Windows контролов
	int32    iNotWasWM_PAINT;                                     //! для исп. DC полученного от BeginPaint
	HBRUSH iWindowBrush;                                        //! кисть фона
	bool   iCursorVisible;                                      // Флаг того, видимый или нет курсор мыши
	bool	  iFullscreen;										   //!< Флаг включения полноэкранного режима
	HICON  mIcon;											   //!< Иконка проекта


	static Window* sInstance;									//для доступа к opengl контекту
	HGLRC mHGLRC;												//opengl контект
};

#endif

