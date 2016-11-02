//!                                    ����� ���� � ������� �������.
/*!  \file Window.h

��� �������� �������� ���������� ���������� ������� ���������� �� Window, ��������������
��� ���������� �������, ����� ��� OnTimer, OnMouseMove, OnKeyUp � �.�.:

struct MainWindow: Window
{
void OnTimer     (int32 dTime);                               // ����� ���������� � ������� iPeriod ms
void OnMouseMove (Pixel mouse_pos);                        // ���������� ��� �������� ����
};

int32 WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32 mode)
{
MainWindow wnd;
if(!wnd.Create("Main","MainWindow",800,600,mode)) return 0; // ������� ���� 800x600
wnd.Period(5);                                              // ������ ������� = 5 ms
wnd.Show();                                                 // ���������� ����
return wnd.Run();                                           // ��������� ����� ���������
}

(!) ��� ������ � ������������� ������� ���������� ���������� (Add file to project) � Builder
ddraw.lib �� ���������� LIB �������.
(c) n-th.com (steps)  v.1.00   16-dec-2001..2005
*****************************************************************************************/
#ifndef OWindowH
#define OWindowH

#include "Engine\Types.h"

#include <windows.h>
#include <stdio.h>

#include <gl/gl.h>                                             // 3D ��������� ��� openGL
#include <gl/glu.h>
#include <float.h>                                             // _control87() : only for Borland (floating error)

#ifdef _MSC_VER
// ���������� ������������ Visual Studio:
#pragma comment (lib,"OpenGL32.lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.lib")
#pragma warning(disable: 4311)     // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312)     // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif


//========================================================================================
//! ����� ���� � ������� �������.
//
struct Window
{
	Window();
	virtual ~Window();

	int  Create(char *caption, char *name, int32 w, int32 h, int32 mode);// ������� ����.

	int CreateGLContext();
	void DestroyGLContext();
	void UpdateContext();

	void Show();                                                // �������� ����
	int  Run();                                                 // ��������� ����� ���������
	void Close();                                               // ������� ����

	void ClearWindow(Rect *rect = 0);                          // ������� ����, ���� rect==0 �� �����

	virtual int  OnCreate() { return 1; }//!< ���������� ���� ��� ��� �������� ����.
	virtual void OnClose() {};         //!< ���������� ���� ��� ��� ����������� ����.
	virtual void OnTimer(int) {};         //!< ���������� � ���������� ���������� iPeriod

	virtual void OnMouseMove(Pixel) {};         //!< ���������� ��� �������� ����.
	virtual void OnLMouseDown(Pixel) {};         //!< ���������� ���� ����� ������ ���� ����������
	virtual void OnRMouseDown(Pixel) {};         //!< ���������� ���� ������ ������ ���� ����������
	virtual void OnLMouseUp(Pixel) {};         //!< ���������� ���� ����� ������ ���� ����������
	virtual void OnRMouseUp(Pixel) {};         //!< ���������� ���� ������ ������ ���� ����������
	virtual void OnDblClick(Pixel) {};         //!< ���������� ���� ���� ������� ������� �������
	virtual void OnMouseWheelDown(Pixel) {};
	virtual void OnMouseWheelUp(Pixel) {};

	virtual void OnKeyUp(uint32) {};							   //!< ���������� ���� ������� ����������
	virtual void OnKeyDown(uint32) {};                            //!< ���������� ���� ������� ����������
	virtual void OnChar(uint32) {};                            //!< ���������� ���� ������� ������� ���� ��������

	virtual void OnResize() {};         //!< ���������� ���� ���� ��������� ������� ������
	virtual void OnPaint(Rect *rect = 0) {};         //!< ���������� �� ������� on_paint
	virtual void	SetFullScreen(bool aFullScreen) {};		   //!< �������� ������������� �����
	virtual void onActivate() {};		   //!< ���������� ��� ��������� ����
	virtual void onDeactivate() {};		   //!< ���������� ��� ����������� ����
	virtual void onMinimize() {};		   //!< ���������� ��� ������������ ����
	virtual void onMaximize() {};		   //!< ���������� ��� �������������� ����

	void   Caption(const char *format, ...);                  //  ���������� ��������� ���� (��� printf!)
	void   SetFocus() { ::SetFocus(iMainHWND); }                 //!< ������� ���� ��������

	void   Period(int32 period) { iPeriod = period; }              //!< ������ ������ ������ OnTimer � ms
	int32    Period() { return iPeriod; }            //!< �������� ������ ������ OnTimer � ms
	void   SlowTimer() { iSlowTimer = 1; SetTimer(iMainHWND, 1, Period(), 0); } //!< ���������� ���� ���������� ������� (����� Run!)

	int32    Time() { return timeGetTime(); };       //!< �������� ������� �����.

	float  FPS() { return iFPS; }                               //!< ����� ������ � �������
	void   PeriodFPS(int32 period) { iPeriodFPS = period; }          //!< ����� ��� ���������� fps-��

	Pixel ScreenSize();                                        // ���������� ������� ��������

	void   Size(int32 w, int32 h);                                  // �������� ������ ����.
	Pixel Size() { return iSize; }                           //!< ������� ����
	void   Pos(int32 x, int32 y);                               // �������� ��������� ����.
	Pixel Pos() { return iPos; }                           //!< ��������� ����
	Rect  GetRect() { return Rect(0, 0, iSize.W(), iSize.H()); }//!< ������������� ����
	int32   Width() { return iSize.X(); }                       //!< ������ ����
	int32   Height() { return iSize.Y(); }                       //!< ������ ����

	Pixel MousePos();                                        // �������� ��������� ����

															  //void Show(OBitmap *bmp, int32 x=0, int32 y=0, Rect *r=0);    // ������� ������ � ����� x,y � ��������� r

	void NoCaption(int32 no) { iNocaption = no; }              //!< ��� ���������
	void ColorBkground(OColor c) { iColorBkground = c; }           //!< ���������� ���� ���� ����
																//----------------------------------------------------------------------- �� ������������ !
	int32 OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Window* GetInstance();

	HDC GetHDC() { return GetDC(iMainHWND); }

	HGLRC GetOpenGlContext() { return mHGLRC; }

protected:
	Pixel iSize;                                               //!< ������, ������ ����
	Pixel iPos;                                                //!< ��������� ���� �� ������

	Int32  iPeriod;                                             //!< ������������ ������ ��-�� OnTimer
	char   iCaption[1024];                                      //!< ��������� ����

	OColor iColorBkground;                                      //!< ���� ����
	int32    iNocaption;                                          //!< ��� ���������
	int32    iNoresize;                                           //!< �� ����� �������� �������
	float  iFPS;                                                //!< ����� ������ � �������
	Int32  iPeriodFPS;                                          //!< ����� ��� ���������� fps-��
	int32    iSlowTimer;                                          //!< ������ ����������� �� �� Run � �� WM_TIMER

	HWND   iMainHWND;                                           //! ������ �������� ����.
																//HWND   iGameHWND;										   //! ������ �������� ����.
	int32    iWndMode;
	int32    iLastTime;                                           //! ����� � ������� ������� �������
	int32    iLastID;                                             //! ��� ��������� Windows ���������
	int32    iNotWasWM_PAINT;                                     //! ��� ���. DC ����������� �� BeginPaint
	HBRUSH iWindowBrush;                                        //! ����� ����
	bool   iCursorVisible;                                      // ���� ����, ������� ��� ��� ������ ����
	bool	  iFullscreen;										   //!< ���� ��������� �������������� ������
	HICON  mIcon;											   //!< ������ �������


	static Window* sInstance;									//��� ������� � opengl ��������
	HGLRC mHGLRC;												//opengl �������
};

#endif

