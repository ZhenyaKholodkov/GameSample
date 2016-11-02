#include "OWindow.h"                                              
#include "CommCtrl.h"

#ifdef _MSC_VER                // Компиляция производится Visual Studio
#pragma warning(disable: 4311) // 'type cast' : pointer truncation from 'LPVOID' to 'Int32'
#pragma warning(disable: 4312) // 'type cast' : conversion from 'LONG' to 'Window *' of greater size
#endif

//#ifndef VISUAL_CPP_EXPRESS
//#include "resource.h"
//#endif


Window* Window::sInstance = 0;

Window* Window::GetInstance()
{
	return sInstance;
}

//========================================================================================
//! Callback вызывает обработчик Window, указатель на которую хранит в USERDATA
//
LRESULT CALLBACK WindowFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window *wnd = (Window *)(GetWindowLong(hWnd, GWL_USERDATA));
	if (wnd) return wnd->OnWindowMessage(hWnd, message, wParam, lParam);
	else    return DefWindowProc(hWnd, message, wParam, lParam);
}
//========================================================================================
//                                   class Window
//========================================================================================
//
Window::Window() :
	iPeriod(100),
	iPeriodFPS(1000),
	iNocaption(false),
	iNoresize(true),										              // по умолчанию - стандартное окно
	iColorBkground(0x000000),                                  // с черным цветом фона.
	iLastID(100),                                              // стартовое значение ID контролов
	iNotWasWM_PAINT(true),
	iCursorVisible(true),
	iFPS(1000),
	iSlowTimer(0),
	iFullscreen(false),
	mIcon(0)
{
	sInstance = this;
}
//========================================================================================
//
Window::~Window()
{
	//for(DWORD i=0; i<WindowControl.size(); i++) delete WindowControl[i];

	sInstance = 0;
}
//========================================================================================
//! Создать окно.
//
int32 Window::Create(char *caption, char *name, int32 w, int32 h, int32 mode)
{

	iWndMode = mode;
	iWindowBrush = (HBRUSH)CreateSolidBrush(iColorBkground.Color());
	WNDCLASSEX wcl;                                             // window class.

	wcl.hInstance = GetModuleHandle(NULL);;                     // handle to this instance

//#if !defined(VISUAL_CPP_EXPRESS)
//	mIcon = ::LoadIcon(wcl.hInstance, MAKEINTRESOURCE(IDI_ICON1));
//#else
	mIcon = 0;
//#endif

	wcl.lpszClassName = name;                                   // window class name
	wcl.lpfnWndProc = WindowFunc;                               // window function
	wcl.style = 0;                                              // default style
	wcl.cbSize = sizeof(WNDCLASSEX);                            // set size of WNDCLASSES
																/*
																wndClass.hIcon			= mIcon; //LoadIcon(instance, "ID_LARGEICON");
																wndClass.hIconSm		= 0; //LoadIcon(instance, "ID_SMALLICON");
																wndClass.hCursor		= mCursor; //LoadCursor(NULL, IDC_ARROW);
																*/
	wcl.hIcon = mIcon;										   // large icon
	wcl.hIconSm = 0;											   // small icon
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);                  // cursor style
	wcl.lpszMenuName = NULL;                                    // no menu
	wcl.cbClsExtra = 0;                                         // no extra
	wcl.cbWndExtra = 0;                                         // information needed
	wcl.hbrBackground = iWindowBrush;

	if (!RegisterClassEx(&wcl)) {                                 // Register the window class
		MessageBox(NULL, "Can\'t register window class", "Error", MB_OK);
		return false;
	}

	// Now that a window class has been registered, a window  can be created.
	if (w == 0 || h == 0) w = h = CW_USEDEFAULT; // let Windows decide

	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | CS_DBLCLKS;

	if (iNocaption) dwStyle &= ~(WS_CAPTION);
	if (iNocaption) dwStyle &= ~(WS_SYSMENU);
	//dwStyle &= WS_SYSMENU;
	if (iNoresize)
	{
		dwStyle &= ~WS_SIZEBOX;
		dwStyle &= ~WS_MAXIMIZEBOX;
	}
	dwStyle |= WS_SYSMENU;
	dwStyle |= WS_CLIPCHILDREN;								   // для отображения поля ввода
	dwStyle |= WS_CLIPSIBLINGS;

	/*DWORD dwStyle = (нужный стиль окна);

	RECT rc = {0, 0, [желаемые размеры клиентской области]};
	AdjustWindowRect(&rc, dwStyle, FALSE);

	CreateWindow(..., rc.right-rc.left, rc.bottom-rc.top, ...);
	*/

	RECT rc = { 0, 0, w, h };
	AdjustWindowRect(&rc, dwStyle, FALSE);

	int t_w = rc.right - rc.left;
	int t_h = rc.bottom - rc.top;

	iMainHWND = CreateWindow(
		name, caption,                                           // name of window class && title
		dwStyle,                                                 // window style - normal
		CW_USEDEFAULT, CW_USEDEFAULT,                            // X,Y coordinate - let Windows decide
		rc.right - rc.left, rc.bottom - rc.top,                                                     // width ,  height
		HWND_DESKTOP,                                            // no parent window
		NULL,                                                    // no menu
		wcl.hInstance,                                           // handle of this instance of the program
		NULL                                                     // no additional arguments
	);

	if (!iMainHWND) {
		MessageBox(NULL, "Can\'t create window", "Error", MB_OK);
		return false;
	}
	HDC hdc = wglGetCurrentDC();
	SetWindowLong(iMainHWND, GWL_USERDATA, LONG(this));         // see WindowFunc

	strcpy(iCaption, caption);                                  // сохраняем заголовок

	RECT Rect;                                                  // get windows sizes:
	GetClientRect(iMainHWND, &Rect);
	iSize.Set(int32(Rect.right - Rect.left), int32(Rect.bottom - Rect.top));

	/*
	iGameHWND = CreateWindow ("STATIC",
	"GameWindow",
	(WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_BORDER & ~WS_CAPTION,
	0,											// x
	0,											// y
	t_w, t_h,									// ширина и высота
	iMainHWND,									// родительское окно
	NULL,										// идентификатор данного контрола
	wcl.hInstance,
	NULL) ;
	*/
	if (!OnCreate()) { OnClose(); return false; }                 // все инициализируем
	return true;
}
//========================================================================================
//! Показать окно.
//
void Window::Show()
{
	ShowWindow(iMainHWND, iWndMode);                                // Display the window
	UpdateWindow(iMainHWND);
}
//========================================================================================
//! Изменить положение окна.
//
void Window::Pos(int32 x, int32 y)
{
	iPos.Set(x, y);
	SetWindowPos(iMainHWND, HWND_NOTOPMOST, iPos.X(), iPos.Y(), iSize.X(), iSize.Y(), SWP_SHOWWINDOW);
}
//========================================================================================
//! Изменить размер окна.
//
void Window::Size(int32 w, int32 h)
{
	iSize.Set(w, h);
	SetWindowPos(iMainHWND, HWND_NOTOPMOST, iPos.X(), iPos.Y(), iSize.X(), iSize.Y(), SWP_SHOWWINDOW);
}
//========================================================================================
//! Запустить петлю сообщений.
//
int32 Window::Run()
{
	if (iSlowTimer) {
		MSG msg;
		while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnClose();                                                  // free all memory
		KillTimer(iMainHWND, 1);
		DeleteObject(iWindowBrush);
		return int32(msg.wParam);
	}

	TIMECAPS tc;                                                // Повышаем точность таймера
	if (timeGetDevCaps(&tc, sizeof(tc)) == TIMERR_NOERROR)
		if (timeBeginPeriod(tc.wPeriodMin) != TIMERR_NOERROR) tc.wPeriodMin = 0;

	MSG msg;
	iLastTime = timeGetTime();
	do {                                                         // Create the message loop:
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);                               // allow use of keyboard
			DispatchMessage(&msg);                                // return control to Windows
		}
		else {
			MsgWaitForMultipleObjects(0, NULL, 1, iPeriod, QS_ALLINPUT);
			int32 tm = timeGetTime(), dTime = tm - iLastTime;
			if (dTime >= iPeriod) {
				iLastTime = tm;
				OnTimer(dTime);                                    // сработало таймерное событие
			}
		}
	} while (msg.message != WM_QUIT);

	if (tc.wPeriodMin) timeEndPeriod(tc.wPeriodMin);             // освобождаем таймер

	OnClose();                                                  // free all memory
	DeleteObject(iWindowBrush);
	return (int32)msg.wParam;
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
	pfd.cAlphaBits = 0;                                        // ?? no picture if > 0 on Win2000
	pfd.cAccumBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	//--------------------------------------------------------------------------------------
	HDC hDC = GetDC(iMainHWND);
	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0) {
		//ShowMessage("Error Init3D: ChoosePixelFormat");
		return 0;
	}
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) {
		//ShowMessage("Error Init3D: SetPixelFormat");
		return 0;
	}

	DescribePixelFormat(hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (pfd.dwFlags & PFD_NEED_PALETTE) {
		//ShowMessage("Error Init3D: DescribePixelFormat (Wrong video mode)");
		return 0;
	}
	//--------------------------------------------------------------------------------------
	mHGLRC = wglCreateContext(hDC);
	if (mHGLRC == 0)
	{
		//ShowMessage("Error Init3D: wglCreateContext");
		return 0;
	}
	//--------------------------------------------------------------------------------------

	if (wglMakeCurrent(hDC, mHGLRC) == 0) {
		//ShowMessage("Error Init3D: wglMakeCurrent");
		return 0;
	}

	ReleaseDC(iMainHWND, hDC);
	return 1;
}

void Window::DestroyGLContext()
{
	if (mHGLRC == 0) return;
	HDC hDC = wglGetCurrentDC();
	wglMakeCurrent(0, 0);
	ReleaseDC(iMainHWND, hDC);
	wglDeleteContext(mHGLRC);
}
//========================================================================================
//! Закрыть окно.
//
void Window::Close()
{
	SendMessage(iMainHWND, WM_CLOSE, 0, 0);
}
//========================================================================================
//! Очистка окна, если rect==0 то всего
//
void  Window::ClearWindow(Rect  *rect)
{
	HRGN  hRgn = (rect) ? CreateRectRgn(rect->X1(), rect->Y1(), rect->X2(), rect->Y2()) : 0;
	InvalidateRgn(iMainHWND, hRgn, 1);
	UpdateWindow(iMainHWND);
	if (hRgn) DeleteObject(hRgn);
}
//========================================================================================
//! Получить положение мыши.
//
Pixel Window::MousePos()
{
	POINT mouse = { 0,0 };
	GetCursorPos(&mouse);
	ScreenToClient(iMainHWND, &mouse);
	Pixel pos(int32(mouse.x), int32(mouse.y));
	return pos;
};
//========================================================================================
//! Установить заголовок окна (как printf!)
//
void Window::Caption(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(iCaption, format, args);

	unsigned long result;
	SendMessageTimeout(iMainHWND, WM_SETTEXT, 0, (LPARAM)iCaption, SMTO_ABORTIFHUNG | SMTO_NORMAL, 100, &result);

	va_end(args);
}
//========================================================================================
//! Абсолютные размеры монитора.
//
Pixel Window::ScreenSize()
{
	Pixel size;
	HDC iDC = GetDC(0);
	size.Set(GetDeviceCaps(iDC, HORZRES), GetDeviceCaps(iDC, VERTRES));
	ReleaseDC(iMainHWND, iDC);
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
		if (iSlowTimer) {
			int32 tm = timeGetTime(), dTime = tm - iLastTime;
			if (dTime >= iPeriod) { iLastTime = tm; OnTimer(dTime); } // сработало таймерное событие
		}
		break;
	case WM_CREATE:
		iLastTime = timeGetTime();
		break;
	case WM_MOUSEMOVE:
		/*
		TRACKMOUSEEVENT tme;

		tme.cbSize = sizeof(TRACKMOUSEEVENT);

		tme.dwFlags = TME_LEAVE;

		tme.hwndTrack = iHWND;

		_TrackMouseEvent(&tme);
		*/
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
			case VK_RETURN:											// Alt + Enter
			{
				SetFullScreen(!iFullscreen);
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
	case WM_PAINT: { //-------------------------------------- process a repaint request:
		RECT rect;
		if (!GetUpdateRect(iHWND, &rect, 0)) break;
		//iNotWasWM_PAINT = false;
		BeginPaint(iHWND, &paintstruct);
		Rect r; r.Set(int32(rect.left), int32(rect.top), int32(rect.right), int32(rect.bottom));
		OnPaint(&r);
		EndPaint(iHWND, &paintstruct);                         // release DC
															   //iNotWasWM_PAINT = true;
	}
				   break;
	case WM_SHOWWINDOW:
		OnPaint();
		break;
	case WM_SIZE:
		GetClientRect(iHWND, &rect);                            // get windows sizes:
		iSize.Set(int32(rect.right - rect.left), int32(rect.bottom - rect.top));
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
	case WM_COMMAND://------------------------- сообщения от стандартных контролов:
					//for(i=0; i<WindowControl.size(); i++)
					//    if(WindowControl[i]->ID==LOWORD(wParam))  OnControl(WindowControl[i], lParam);
		break;
	case WM_DESTROY://------------------------------------- terminate the program
		if (iSlowTimer) OnClose();                             // free all memory
		PostQuitMessage(0);
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE || wParam == SC_KEYMENU || wParam == SC_MOVE)      return 0;
		// Обязательно должны попасть в default:
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{
			onDeactivate();
			if (iFullscreen && altPressed && (!cntrlPressed))
			{
				::CloseWindow(iHWND); // or ShowWindow(iHWND, SW_MINIMIZE);		
			}
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