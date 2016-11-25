#include "OWindow.h"                                              
#include "CommCtrl.h"

#ifdef _MSC_VER
#pragma warning(disable: 4311) // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312) // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif

Window* Window::sInstance = 0;

Window* Window::GetInstance()
{
	return sInstance;
}

LRESULT CALLBACK WindowFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window *wnd = (Window *)(GetWindowLong(hWnd, GWL_USERDATA));
	if (wnd) return wnd->OnWindowMessage(hWnd, message, wParam, lParam);
	else    return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window() :
	mPeriod(100),
	mColorBkground(0x000000),
	mSlowTimer(0),
	mIcon(0)
{
	sInstance = this;
}

Window::~Window()
{
	sInstance = 0;
}

int32 Window::Create(char *caption, char *name, int32 w, int32 h, int32 mode)
{

	mWndMode = mode;
	mWindowBrush = (HBRUSH)CreateSolidBrush(mColorBkground.getColor());
	WNDCLASSEX wcl;                                             // window class.

	wcl.hInstance = GetModuleHandle(NULL);                     // handle to this instance

	mIcon = 0;

	wcl.lpszClassName = name;                                   // window class name
	wcl.lpfnWndProc = WindowFunc;                               // window function
	wcl.style = 0;                                              // default style
	wcl.cbSize = sizeof(WNDCLASSEX);                            // set size of WNDCLASSES



	wcl.hIcon = mIcon;										   // large icon
	wcl.hIconSm = 0;											   // small icon
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
	if (w == 0 || h == 0) w = h = CW_USEDEFAULT;

	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | CS_DBLCLKS;

	//no resize
	dwStyle &= ~WS_SIZEBOX;
	dwStyle &= ~WS_MAXIMIZEBOX;

	dwStyle |= WS_SYSMENU;
	dwStyle |= WS_CLIPCHILDREN;
	dwStyle |= WS_CLIPSIBLINGS;

	RECT rc = { 0, 0, w, h };
	AdjustWindowRect(&rc, dwStyle, FALSE);

	int t_w = rc.right - rc.left;
	int t_h = rc.bottom - rc.top;

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

	if (!mMainHWND) {
		MessageBox(NULL, "Can\'t create window", "Error", MB_OK);
		return false;
	}
	SetWindowLong(mMainHWND, GWL_USERDATA, LONG(this));

	strcpy(mCaption, caption);

	RECT Rect;
	GetClientRect(mMainHWND, &Rect);
	mSize.Set(int32(Rect.right - Rect.left), int32(Rect.bottom - Rect.top));

	if (!OnCreate()) { OnClose(); return false; }
	return true;
}

void Window::Show()
{
	ShowWindow(mMainHWND, mWndMode); 
	UpdateWindow(mMainHWND);
}

void Window::Pos(int32 x, int32 y)
{
	mPos.Set(x, y);
	SetWindowPos(mMainHWND, HWND_NOTOPMOST, mPos.X(), mPos.Y(), mSize.X(), mSize.Y(), SWP_SHOWWINDOW);
}

void Window::Size(int32 w, int32 h)
{
	mSize.Set(w, h);
	SetWindowPos(mMainHWND, HWND_NOTOPMOST, mPos.X(), mPos.Y(), mSize.X(), mSize.Y(), SWP_SHOWWINDOW);
}

int32 Window::Run()
{
	if (mSlowTimer) 
	{
		MSG msg;
		while (GetMessage(&msg, (HWND)NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnClose();                        
		KillTimer(mMainHWND, 1);
		DeleteObject(mWindowBrush);
		return int32(msg.wParam);
	}

	TIMECAPS tc;  
	if (timeGetDevCaps(&tc, sizeof(tc)) == TIMERR_NOERROR)
		if (timeBeginPeriod(tc.wPeriodMin) != TIMERR_NOERROR) tc.wPeriodMin = 0;

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
				OnTimer(dTime);  
			}
		}
	} while (msg.message != WM_QUIT);

	if (tc.wPeriodMin) timeEndPeriod(tc.wPeriodMin); 

	OnClose();                                                
	DeleteObject(mWindowBrush);
	return (int32)msg.wParam;
}

void Window::Close()
{
	SendMessage(mMainHWND, WM_CLOSE, 0, 0);
}

void  Window::ClearWindow(Rect  *rect)
{
	HRGN  hRgn = (rect) ? CreateRectRgn(rect->X1(), rect->Y1(), rect->X2(), rect->Y2()) : 0;
	InvalidateRgn(mMainHWND, hRgn, 1);
	UpdateWindow(mMainHWND);
	if (hRgn) DeleteObject(hRgn);
}

Pixel Window::MousePos()
{
	POINT mouse = { 0,0 };
	GetCursorPos(&mouse);
	ScreenToClient(mMainHWND, &mouse);
	Pixel pos(int32(mouse.x), int32(mouse.y));
	return pos;
};

void Window::Caption(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(mCaption, format, args);

	unsigned long result;
	SendMessageTimeout(mMainHWND, WM_SETTEXT, 0, (LPARAM)mCaption, SMTO_ABORTIFHUNG | SMTO_NORMAL, 100, &result);

	va_end(args);
}

Pixel Window::ScreenSize()
{
	Pixel size;
	HDC iDC = GetDC(0);
	size.Set(GetDeviceCaps(iDC, HORZRES), GetDeviceCaps(iDC, VERTRES));
	ReleaseDC(mMainHWND, iDC);
	return size;
}
int32 Window::OnWindowMessage(HWND iHWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintstruct;
	RECT rect;
	static bool altPressed = false;
	static bool cntrlPressed = false;
	switch (message) {
	case WM_TIMER:
		if (mSlowTimer) {
			int32 tm = timeGetTime(), dTime = tm - mLastTime;
			if (dTime >= mPeriod) { mLastTime = tm; OnTimer(dTime); } // сработало таймерное событие
		}
		break;
	case WM_CREATE:
		mLastTime = timeGetTime();
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_LBUTTONDOWN:
		OnLMouseDown(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_LBUTTONUP:
		OnLMouseUp(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_RBUTTONDOWN:
		OnRMouseDown(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_RBUTTONUP:
		OnRMouseUp(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_LBUTTONDBLCLK:
		OnDblClick(Pixel(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_MOUSEWHEEL:
	{
		SHORT delta = HIWORD(wParam);
		if (delta > 0) OnMouseWheelUp(Pixel(LOWORD(lParam), HIWORD(lParam)));
		else if (delta < 0) OnMouseWheelDown(Pixel(LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_KEYUP:
		if (wParam == VK_CONTROL)
			cntrlPressed = false;
		OnKeyUp(uint32(wParam));
		break;
	case WM_KEYDOWN:
		if (wParam == VK_CONTROL)
			cntrlPressed = true;
		OnKeyDown(uint32(wParam));
		break;
	case WM_SYSKEYDOWN:
		if (lParam & (1 << 29))		// Alt Down
		{
			altPressed = true;
			switch (wParam)
			{
			case VK_RETURN:         // Alt + Enter
			{
				break;
			}
			default:		return DefWindowProc(iHWND, message, wParam, lParam);

			}
		}
		break;
	case WM_SYSKEYUP:
		if (lParam & (1 << 29))	// Alt Up
		{
			altPressed = false;
		}
	case WM_CHAR:
		OnChar((char)wParam);
		break;
	case WM_PAINT:
	{ //-------------------------------------- process a repaint request:
		RECT rect;
		if (!GetUpdateRect(iHWND, &rect, 0)) break;
		BeginPaint(iHWND, &paintstruct);
		Rect r; r.Set(int32(rect.left), int32(rect.top), int32(rect.right), int32(rect.bottom));
		OnPaint(&r);
		EndPaint(iHWND, &paintstruct);
	}
	break;
	case WM_SHOWWINDOW:
		OnPaint();
		break;
	case WM_SIZE:
		GetClientRect(iHWND, &rect);                            // get windows sizes:
		mSize.Set(int32(rect.right - rect.left), int32(rect.bottom - rect.top));
		OnResize();
		if (wParam == SIZE_RESTORED)
		{
			onMaximize();
		}
		if (wParam == SIZE_MINIMIZED)
		{
			onMinimize();
		}
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY://------------------------------------- terminate the program
		if (mSlowTimer) OnClose();                             // free all memory
		PostQuitMessage(0);
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE || wParam == SC_KEYMENU || wParam == SC_MOVE)
			return 0;
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{
			onDeactivate();
		}
		else
		{
			if (wParam == WA_ACTIVE)
			{
				onActivate();
			}
			return int32(DefWindowProc(iHWND, message, wParam, lParam));
		}
		break;
	case WM_NCACTIVATE:
		if (wParam == WA_INACTIVE)
		{
			onDeactivate();
		}
		else
		{
			if (wParam == WA_ACTIVE)
				onActivate();
		}
		break;
	default:
		return int32(DefWindowProc(iHWND, message, wParam, lParam));
	}
	return 0;
}

int Window::CreateGLContext()
{
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
		return 0;
	}
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
	{
		return 0;
	}

	DescribePixelFormat(hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		return 0;
	}

	mHGLRC = wglCreateContext(hDC);
	if (mHGLRC == 0)
	{
		return 0;
	}
	if (wglMakeCurrent(hDC, mHGLRC) == 0)
	{
		return 0;
	}

	ReleaseDC(mMainHWND, hDC);
	return 1;
}

void Window::DestroyGLContext()
{
	if (mHGLRC == 0) return;
	HDC hDC = wglGetCurrentDC();
	wglMakeCurrent(0, 0);
	ReleaseDC(mMainHWND, hDC);
	wglDeleteContext(mHGLRC);
}

void Window::UpdateContext()
{
	glFinish();

	HDC hDC = GetDC(mMainHWND);
	SwapBuffers(hDC);
	ReleaseDC(mMainHWND, hDC);
}