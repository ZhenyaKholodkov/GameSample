#include "GGameWindow.h"   
#include "Utils.h"
#include "GRenderManager.h"

#include <time.h>


LRESULT CALLBACK WindowFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GGameWindow *wnd = (GGameWindow *)(GetWindowLong(hWnd, GWL_USERDATA));
	if (wnd) 
		return wnd->onWindowMessage(hWnd, message, wParam, lParam);
	else    
		return DefWindowProc(hWnd, message, wParam, lParam);
}

GGameWindow::GGameWindow() :
	mPeriod(10)
{
}

GGameWindow::~GGameWindow()
{
}

int32 GGameWindow::createWindow(char *caption, char *name, int32 width, int32 height, int32 mode)
{
	mWndMode = mode;

	WNDCLASSEX wcl;     // window class.
	wcl.hInstance = GetModuleHandle(NULL);                     // handle to this instance
	wcl.lpszClassName = name;                                   // window class name
	wcl.lpfnWndProc = WindowFunc;                               // window function
	wcl.style = 0;                                              // default style
	wcl.cbSize = sizeof(WNDCLASSEX);                            // set size of WNDCLASSES
	wcl.hIcon = 0;							       			   // large icon
	wcl.hIconSm = 0;											// small icon
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);                  // cursor style
	wcl.lpszMenuName = NULL;                                    // no menu
	wcl.cbClsExtra = 0;                                         // no extra
	wcl.cbWndExtra = 0;                                         // information needed
	wcl.hbrBackground = mWindowBrush;

	if (!RegisterClassEx(&wcl)) {                                 // Register the window class
		MessageBox(NULL, "Can\'t register window class", "Error", MB_OK);
		return false;
	}

	// Now that a window class has been registered, a window  can be created.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | CS_DBLCLKS;

	//no resize
	//dwStyle &= ~WS_SIZEBOX;
	//dwStyle &= ~WS_MAXIMIZEBOX;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, dwStyle, FALSE);

	mMainHWND = CreateWindow(
		name, caption,                                           // name of window class && title
		dwStyle,                                                 // window style - normal
		CW_USEDEFAULT, CW_USEDEFAULT,                            // X,Y coordinate - let Windows decide
		rc.right - rc.left, rc.bottom - rc.top,                                                     // width ,  height
		HWND_DESKTOP,                                            // no parent window
		NULL,                                                    // no menu
		wcl.hInstance,                                           // handle of this instance of the program
		NULL                                                     // no additional arguments
		);

	if (!mMainHWND) 
	{
		MessageBox(NULL, "Can\'t create window", "Error", MB_OK);
		return false;
	}
	SetWindowLong(mMainHWND, GWL_USERDATA, LONG(this));
	
	if (createGLContext())
	{
		GRenderManager::Instance()->init(width, height);
	}
	else
	{
		GLog::printLog("Cannot create OpenGL context!");
		onClose();
		return false; 
	}
	srand((uint32)time(0));

	if (!onCreate())
	{
		GLog::printLog("Cannot create Game!");
		onClose();
		return false;
	}

	show();

	return true;
}

void GGameWindow::show()
{
	ShowWindow(mMainHWND, mWndMode); 
	UpdateWindow(mMainHWND);
}

int32 GGameWindow::runGameLoop()
{
	MSG msg;
	mLastTime = timeGetTime();
	//the game loop
	do
	{ 
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);                               // allow use of keyboard
			DispatchMessage(&msg);                                // return control to Windows
		}
		else
		{
			MsgWaitForMultipleObjects(0, NULL, 1, mPeriod, QS_ALLINPUT);
			int32 tm = timeGetTime(), dTime = tm - mLastTime;
			if (dTime >= mPeriod) 
			{
				mLastTime = tm;
				onTimer(dTime);  
				updateContext();
			}
		}
	} while (msg.message != WM_QUIT);

	onClose();                                                
	DeleteObject(mWindowBrush);
	return (int32)msg.wParam;
}

void GGameWindow::close() const
{
	SendMessage(mMainHWND, WM_CLOSE, 0, 0);
}

int32 GGameWindow::onWindowMessage(HWND iHWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		mLastTime = timeGetTime();
		break;
	case WM_MOUSEMOVE:
		onMouseMove(GCursor(LOWORD(lParam), HIWORD(lParam), mCurrentMousePressed));
		break;
	case WM_LBUTTONDOWN:
		mCurrentMousePressed = true;
		onLMouseDown(GCursor(LOWORD(lParam), HIWORD(lParam), mCurrentMousePressed));
		break;
	case WM_LBUTTONUP:
		mCurrentMousePressed = false;
		onLMouseUp(GCursor(LOWORD(lParam), HIWORD(lParam), mCurrentMousePressed));
		break;
	case WM_RBUTTONDOWN:
		onRMouseDown(GCursor(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_RBUTTONUP:
		onRMouseUp(GCursor(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_KEYUP:
		onKeyUp(convertToGKey(uint32(wParam)));
		break;
	case WM_KEYDOWN:
		onKeyDown(convertToGKey(uint32(wParam)));
		break;
	case WM_DESTROY://------------------------------------- terminate the program
		PostQuitMessage(0);
		break;
	default:
		return int32(DefWindowProc(iHWND, message, wParam, lParam));
	}
	return 0;
}

bool GGameWindow::createGLContext()
{
	if (mHGLRC != 0)
	{
		GLog::printLog("Context has been already created!");
		return true;
	}
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.dwFlags |= PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 0;
	pfd.cAccumBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	//--------------------------------------------------------------------------------------
	HDC hDC = GetDC(mMainHWND);
	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0)
	{
		GLog::printLog("Pixel format has not been matched to the device context format!");
		return false;
	}
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
	{
		GLog::printLog("Cannot set pixel format for the device context!");
		return false;
	}

	DescribePixelFormat(hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	mHGLRC = wglCreateContext(hDC);
	if (mHGLRC == 0)
	{
		GLog::printLog("Context has not been created!");
		return false;
	}
	if (wglMakeCurrent(hDC, mHGLRC) == 0)
	{
		GLog::printLog("Context has not been made as current!");
		return false;
	}

	ReleaseDC(mMainHWND, hDC);
	return true;
}

void GGameWindow::destroyGLContext() const
{
	if (mHGLRC == 0) 
		return;
	HDC hDC = wglGetCurrentDC();
	wglMakeCurrent(0, 0);
	ReleaseDC(mMainHWND, hDC);
	wglDeleteContext(mHGLRC);
}

void GGameWindow::updateContext() const
{
	glFinish();
	HDC hDC = GetDC(mMainHWND);
	SwapBuffers(hDC);
	ReleaseDC(mMainHWND, hDC);
}

GKey GGameWindow::convertToGKey(uint32 key) const
{
	switch (key)
	{
	case VK_LEFT:
		return KEY_LEFT;
		break;
	case VK_UP:
		return KEY_UP;
		break;
	case VK_RIGHT:
		return KEY_RIGHT;
		break;
	case VK_DOWN:
		return KEY_DOWN;
		break;
	}
	return KEY_NONE;
}