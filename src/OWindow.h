#ifndef OWindowH
#define OWindowH

#include "Engine\Types.h"

#include <windows.h>
#include <stdio.h>

#include <gl/gl.h>  

#ifdef _MSC_VER
#pragma comment (lib,"OpenGL32.lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.lib")
#pragma warning(disable: 4311)     // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312)     // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif

struct Window
{
	Window();
	virtual ~Window();

	int  Create(char *caption, char *name, int32 w, int32 h, int32 mode);

	int CreateGLContext();
	void DestroyGLContext();
	void UpdateContext();

	void Show();                                           
	int  Run();                                            
	void Close();                                          

	void ClearWindow(Rect *rect = 0);                      

	virtual int  OnCreate() { return 1; }
	virtual void OnClose() {};           
	virtual void OnTimer(int) {};        

	virtual void OnMouseMove(Pixel) {};        
	virtual void OnLMouseDown(Pixel) {};       
	virtual void OnRMouseDown(Pixel) {};       
	virtual void OnLMouseUp(Pixel) {};         
	virtual void OnRMouseUp(Pixel) {};         
	virtual void OnDblClick(Pixel) {};         
	virtual void OnMouseWheelDown(Pixel) {};
	virtual void OnMouseWheelUp(Pixel) {};

	virtual void OnKeyUp(uint32) {};			
	virtual void OnKeyDown(uint32) {};         
	virtual void OnChar(uint32) {};            

	virtual void OnResize() {};        
	virtual void OnPaint(Rect *rect = 0) {};           
	virtual void SetFullScreen(bool aFullScreen) {};
	virtual void onActivate() {};		   
	virtual void onDeactivate() {};		   
	virtual void onMinimize() {};		   
	virtual void onMaximize() {};		   

	void   Caption(const char *format, ...);         
	void   SetFocus() { ::SetFocus(mMainHWND); }     

	void   Period(int32 period) { mPeriod = period; }
	int32  Period() { return mPeriod; }            
	void   SlowTimer() { mSlowTimer = 1; SetTimer(mMainHWND, 1, Period(), 0); } 

	int32  Time() { return timeGetTime(); };     

	Pixel  ScreenSize();                                    

	void   Size(int32 w, int32 h);                         
	Pixel  Size() { return mSize; }                         
	void   Pos(int32 x, int32 y);                          
	Pixel  Pos() { return mPos; }                           
	Rect   GetRect() { return Rect(0, 0, mSize.W(), mSize.H()); }
	int32  Width() { return mSize.X(); }                       
	int32  Height() { return mSize.Y(); }                      

	Pixel  MousePos();   
	void   ColorBkground(IGColor c) { mColorBkground = c; }     
															
	int32  OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Window* GetInstance();

	HDC   GetHDC() { return GetDC(mMainHWND); }

	HGLRC GetOpenGlContext() { return mHGLRC; }

protected:
	static Window* sInstance;

	Pixel  mSize;                                            
	Pixel  mPos;                                             

	int32  mPeriod;                                         
	char   mCaption[1024];                                  

	IGColor mColorBkground;
	int32   mSlowTimer;                                    

	HWND   mMainHWND;                                       
															
	int32  mWndMode;
	int32  mLastTime;                                    
	HBRUSH mWindowBrush;     								
	HICON  mIcon;			
	HGLRC  mHGLRC;											
};

#endif

