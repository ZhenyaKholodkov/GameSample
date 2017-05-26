#ifndef OWindowH
#define OWindowH

#include "Types.h"
#include "gEntityManager.h"
#include "gSystemManager.h"

#include <windows.h>
#include <stdio.h>
#include <memory>

#include <gl/gl.h>  

#ifdef _MSC_VER
#pragma comment (lib,"OpenGL32.lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.lib")
#pragma warning(disable: 4311)     // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312)     // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif

class GGameWindow
{
	friend LRESULT CALLBACK WindowFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	GGameWindow();
	virtual ~GGameWindow();

	int  createWindow(char *caption, char *name, int32 width, int32 height, int32 mode);
	int  runGameLoop();

	void close() const;

	void   setPeriod(int32 period) { mPeriod = period; }
	int32  getPeriod() const { return mPeriod; }

private:
	void show();
	GKey convertToGKey(uint32 key) const;

	virtual int  onCreate() { return 1; }
	virtual void onClose() {};           
	virtual void onTimer(int) {};        

	virtual void onMouseMove(GCursor cursor) {};
	virtual void onLMouseDown(GCursor cursor) {};
	virtual void onRMouseDown(GCursor cursor) {};
	virtual void onLMouseUp(GCursor cursor) {};
	virtual void onRMouseUp(GCursor cursor) {};
	virtual void onMouseWheelDown(GCursor cursor) {};
	virtual void onMouseWheelUp(GCursor cursor) {};

	virtual void onKeyUp(GKey key) {};			
	virtual void onKeyDown(GKey key) {};
															
	int32  onWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HGLRC getOpenGlContext() const { return mHGLRC; }

protected:
	bool createGLContext();
	void destroyGLContext() const;
	void updateContext() const;

protected:          
	int32  mPeriod;
	int32  mWndMode;             //!< the window mode                                   

	HWND   mMainHWND;            //!< the handle to a window       
	HBRUSH mWindowBrush;
	HGLRC  mHGLRC;		         //!<  the handle to an OpenGL rendering context.	                                  
					
	std::shared_ptr<GEntityManager> mEntityManager;
	std::shared_ptr<GSystemManager> mSystemManager;

	int32  mLastTime;
	bool   mCurrentMousePressed;
};

#endif

