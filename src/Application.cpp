#include "Application.h"


bool segmentedLoad = false;                // �������� ������� � ����������� �������. TODO - ���������� �� ����������� �����
bool loadByNeed = false;                // �������� ���� �� ���� �������������. ������ ��� ���������� segmentedLoad
bool deleteWhenNoNeed = false;                // ������� �����, ������� ��� �� ������ ������ �� �����. ������ ��� ���������� loadByNeed
int  sessionStartTime = 0;
int  sessionStartTimeSuspendCounts = 0;
int  launchesNum = 0;

Application*	Application::_instance = NULL;

Application::Application() :
	Window(),
	//_viewer(0),
	//_ctx(0),
	//_clips(0),
	mGame(NULL),
	_screenWidth(1024),
	_screenHeight(768),
	_webBrowserIsVisible(false),
	_isMinimize(false),
	_showSplashScreen(0),
	_spashScreenTextureID(0)
{
	_instance = this;
}

//----------------------------------------------------------------------------------------
// ���������� ���� ��� ��� �������� ����
//
int32 Application::OnCreate()
{
	if (!CreateGLContext())
	{
		return 0;
	}
	srand((int)time(0));                                        // ����������� ���������

	RECT clientRect;
	GetClientRect(iMainHWND, &clientRect);
	//glViewport(0, 0, clientRect.right/2, clientRect.bottom/2);

	int aScreenWidth = _screenWidth;
	int aScreenHeight = _screenHeight;

	int mWidth = (aScreenWidth > aScreenHeight) ? aScreenWidth : aScreenHeight;
	int mHeight = (aScreenWidth > aScreenHeight) ? aScreenHeight : aScreenWidth;

	ResDictionary dict;
	int32 hash1 = dict.hash("/temp/", "image.png");
	int32 hash2 = dict.hash("/temp/", "image.png");
	mRenderContext = IGRender::Create();
	mRenderContext->init(mWidth, mHeight);

	mGame = IGame::Instane();
	mGame->Create();

	//_viewer = new GViewer();
	//
	//// ���������� ���������� ����������
	//static char buf[S3E_CONFIG_STRING_MAX];
	//s3eResult s3eRes = s3eConfigGetString("S3E", "DispFixRot", buf);
	//if (s3eRes == S3E_RESULT_SUCCESS && strlen(buf) > 0)
	//{
	//	if (strcmp("Landscape", buf) == 0 || strcmp("FixedLandscape", buf) == 0)
	//	{
	//		_viewer->screenOrient(GViewer::ORIENT_LANDSCAPE);
	//	}
	//	else if (strcmp("Portrait", buf) == 0 || strcmp("FixedPortrait", buf) == 0)
	//	{
	//		_viewer->screenOrient(GViewer::ORIENT_PORTRAIT);
	//	}
	//}
	//
	//_ctx = IGRender::Create();
	//_clips = new GResImg();
	//
	//_ctx->init(mWidth, mHeight);
	//ShowSplashScreen();												// ���������� ����������, ���� ����
	//
	//_clips->render(_ctx);
	//GClip::setResManager(_clips);
	//
	//
	//_systemFont = IGFontManager::Create();
	//_ctx->setSystemFont(_systemFont);
	//
	//_viewer->init((float)mWidth, (float)mHeight);
	//
	//
	////_viewer->loadProject(  "flashgammNoH.seproj", 1, 0, *_ctx );
	//// �������� ������ ��� ����������. ���� ������ �� ���������� ������
	//int config_loading_res = 0;
	//if (mWidth == 960 || mWidth == 640) {
	//	// iPhone 4 detected
	//	config_loading_res = _viewer->loadIni("viewer_960_640.xml");
	//}
	//else if (mWidth == 480 || mWidth == 320) {
	//	// iPhone 3 detected
	//	config_loading_res = _viewer->loadIni("viewer_480_320.xml");
	//}
	//else {
	//	// ������� �������������� �������
	//	static char buf[256];
	//	if (mWidth >= mHeight) {
	//		sprintf(buf, "viewer_%d_%d.xml", mWidth, mHeight);
	//	}
	//	else {
	//		sprintf(buf, "viewer_%d_%d.xml", mHeight, mWidth);
	//	}
	//	config_loading_res = _viewer->loadIni(buf);
	//}
	//
	//if (config_loading_res <= 0)
	//	_viewer->loadIni("viewer.xml"); // ��������� ����
	//
	//_viewer->startMoreGames();
	//_viewer->startGlobalTop();
	//_viewer->startFlurry();
	//_viewer->startAdMob();
	//_viewer->startServices();
	//
	//_clips->videoMemory(_viewer->defaultVideoMemory()); // ����� ������ ��� �����
	//_viewer->loadProject(*_ctx);

	return 1;
}
//----------------------------------------------------------------------------------------
// ���������� ���� ��� ��� ����������� ����
//
void Application::OnClose()
{
	DestroyGLContext();
	//s3eFile* closeFile = s3eFileOpen("survey.txt", "r");
	//if (closeFile)
	//{
	//	s3eFileSeek(closeFile, 0, S3E_FILESEEK_END);
	//	int len = s3eFileTell(closeFile);
	//	s3eFileSeek(closeFile, 0, S3E_FILESEEK_SET);
	//	char url[2048];
	//	s3eFileReadString(url, 2048, closeFile);
	//	s3eFileClose(closeFile);
	//	openURL(url);
	//}
	//_viewer->close();                                             // ��������� ������
	//Close3D();
}
//----------------------------------------------------------------------------------------
//
void Application::OnTimer(int32 dt)
{
	mGame->Update(dt);
	mRenderContext->endFrame();
	UpdateContext();
	//if (::gameDebug)
	//  Caption("%s %s fps=%.0f : m=%3.0f,  %3.0f [%d]", WindowCaption, (_viewer->_isPause? "[PAUSE]": ""), FPS(), _viewer->_mouse.x, _viewer->_mouse.y, _viewer->_mouse.press? 1:0);
	//Caption("Who Am I");
	//CalcFPS(dt);
	//if (_showSplashScreen)
	//	HideSplashScreen();
	//
	//_viewer->timer(dt);
	//
	//_ctx->startFrame();
	//
	//_viewer->draw(*_ctx);
	//
	//_ctx->endFrame();

	//SetFullScreen(GObject::fullScreen());			// ���� �������� �� �������� - ������ �� ���������� 
}
//----------------------------------------------------------------------------------------
//! ������ �������
//
void Application::OnKeyDown(uint32 msKey)
{
	//s3eKey key = msTos3eKey(msKey);
	//
	//bool res = _viewer->onKeyDown(key);
	//
	//if (::gameDebug)
	//{
	//	if (key == s3eKeyF1)
	//		_viewer->_isHelp = !_viewer->_isHelp;
	//	if (key == s3eKeyF2)
	//		_viewer->_isOptions = !_viewer->_isOptions;
	//	if (key == s3eKeySpace)
	//		_viewer->_isPause = !_viewer->_isPause;
	//	if (key == s3eKeyT)
	//		_viewer->_isTime = !_viewer->_isTime;
	//	if (key == s3eKeyM)
	//		_viewer->_isMouse = !_viewer->_isMouse;
	//	if (key == s3eKeyF)
	//		GObject::_showFrame = !GObject::_showFrame;
	//	if (key == s3eKeyS) {
	//		_viewer->_isStates = !_viewer->_isStates;
	//		if (_viewer->_isStates)
	//			_viewer->createInfoStages();
	//		else
	//			_viewer->deleteInfoStages();
	//	}
	//	if (key == s3eKeyD)
	//		_viewer->_isDevices = !_viewer->_isDevices;
	//	if (key == s3eKeyR)
	//		_viewer->reloadLayout();
	//	if (key == s3eKeyV) {
	//		_viewer->showInfo("viewer_info.html");
	//		ShellExecute(NULL, "open", "viewer_info.html", NULL, NULL, SW_SHOWNORMAL);
	//	}
	//	//if(key== s3eKeyLeft)
	//	//	_viewer->_originX -= 100.0f;
	//	//if(key== s3eKeyRight)
	//	//	_viewer->_originX += 100.0f;
	//	//if(key== s3eKeyUp)
	//	//	_viewer->_originY -= 100.0f;
	//	//if(key== s3eKeyDown)
	//	//	_viewer->_originY += 100.0f;
	//	//if (key == s3eKeyC)
	//	//	_viewer->cycleDebugTextColor();
	//}
}

void Application::OnKeyUp(uint32 msKey)
{
	//s3eKey key = msTos3eKey(msKey);

	//bool res = _viewer->onKeyUp(key);

	//if (key == s3eKeyEsc)
	//	_viewer->onEscKey();
}

//----------------------------------------------------------------------------------------
// ���������� ���� ����� ������ ���� ����������
//
void Application::OnLMouseDown(Pixel  mouse_pos)
{
	_currMouse.x = (float)mouse_pos.X(); _currMouse.y = (float)mouse_pos.Y();
	_currMouse.press = true;
	//_viewer->mouseDn(_currMouse);
	//if (::gameDebug)
	//{
	//	if (_viewer && (_currMouse.x>0 && _currMouse.x<20))
	//	{
	//		_viewer->_isOptions = !_viewer->_isOptions;
	//	}
	//}
}
//----------------------------------------------------------------------------------------
// ���������� ���� ����� ������ ���� ����������
//
void Application::OnLMouseUp(Pixel  mouse_pos)
{
	_currMouse.x = (float)mouse_pos.X(); _currMouse.y = (float)mouse_pos.Y();
	_currMouse.press = false;

	//_viewer->mouseUp(_currMouse);
}
//----------------------------------------------------------------------------------------
// ���������� ��� �������� ����
//
void Application::OnMouseMove(Pixel mouse_pos)
{
	static int border = 10;
	_currMouse.x = (float)mouse_pos.X(); _currMouse.y = (float)mouse_pos.Y();
	//if (_currMouse.press)
	//{
	//	if (																// ��������� ���� ��� ��������� ����, ���� ��� ��� ������ ������������ �� ���� �����
	//		(_currMouse.x<border) ||
	//		(_currMouse.y<border) ||
	//		(_currMouse.x>(_ctx->width() - border)) ||
	//		(_currMouse.y>(_ctx->height() - border))
	//		)
	//	{
	//		_currMouse.press = false;
	//		//_viewer->mouseUp(_currMouse);	// ��������� ��������� ����
	//	}
	//	else
	//		//_viewer->mouseMv(_currMouse);
	//}
	//_viewer->mouseAllMv(_currMouse);
}

//----------------------------------------------------------------------------------------
// ���������� ��� ��������� �������� ���� ����
//
void Application::OnMouseWheelDown(Pixel mouse_pos)
{
	GMouse m; m.x = (float)mouse_pos.X(); m.y = (float)mouse_pos.Y(); m.press = false;
	//_viewer->mouseWheelDn(m);
}
//----------------------------------------------------------------------------------------
// ���������� ��� ��������� �������� ���� �����
//
void Application::OnMouseWheelUp(Pixel mouse_pos)
{
	GMouse m; m.x = (float)mouse_pos.X(); m.y = (float)mouse_pos.Y(); m.press = false;
	//_viewer->mouseWheelUp(m);
}
//----------------------------------------------------------------------------------------
// ���������� ������������� �����
//
void Application::SetFullScreen(bool aFullscreen)
{
	if (iFullscreen == aFullscreen)
		return;

	iFullscreen = aFullscreen;


	if (iFullscreen) {
		//GLog::push("Switching fullscreen on");
		//AbsTrace(L_INFO, GLog, "Switching fullscreen on");
		_windowStyles = ::GetWindowLongPtr(iMainHWND, GWL_STYLE);
		::GetWindowPlacement(iMainHWND, &_windowPlacement);

		BOOL vParam;
		::SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &vParam, 0);
		_screenSaveActive = vParam;

		::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, 0, SPIF_SENDWININICHANGE);

	}
	else {
		//GLog::push("Switching fullscreen off");
		//AbsTrace(L_INFO, GLog, "Switching fullscreen off");
		::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, _screenSaveActive, 0, SPIF_SENDWININICHANGE);
	}

	RECT fullrect = { 0, 0, 0, 0 };
	int scr_width = ::GetSystemMetrics(SM_CXSCREEN);
	int scr_height = ::GetSystemMetrics(SM_CYSCREEN);

	::SetRect(&fullrect, 0, 0, scr_width, scr_height);

	WINDOWPLACEMENT newplacement = _windowPlacement;
	newplacement.showCmd = SW_SHOWNORMAL;
	newplacement.rcNormalPosition = fullrect;

	if (iFullscreen)
	{
		DEVMODE  device_mode;
		device_mode.dmSize = sizeof(DEVMODE);
		::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &device_mode);
		::ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN);
		updateWindowParams();

		::SetWindowPlacement(iMainHWND, &newplacement);

		//_viewer->changeScreenSize(*_ctx, scr_width, scr_height);
	}
	else
	{
		HWND taskbar = ::FindWindow("Shell_TrayWnd", NULL);
		HWND start = ::FindWindow("Button", NULL);

		if (taskbar)
		{
			::ShowWindow(taskbar, SW_HIDE);
			::UpdateWindow(taskbar);
		}
		if (start)
		{
			::ShowWindow(start, SW_HIDE);
			::UpdateWindow(start);
		}
		::CloseWindow(iMainHWND);
		_isMinimize = false;
		::SetWindowLongPtr(iMainHWND, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER | WS_CLIPCHILDREN) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
		::SetWindowPlacement(iMainHWND, &_windowPlacement);
		//_viewer->changeScreenSize(*_ctx, (int)_screenWidth, (int)_screenHeight);
	}


	if (!iFullscreen)
	{
		HWND taskbar = ::FindWindow("Shell_TrayWnd", NULL);
		HWND start = ::FindWindow("Button", NULL);

		if (taskbar)
		{
			::ShowWindow(taskbar, SW_SHOW);
			::UpdateWindow(taskbar);
		}
		if (start)
		{
			::ShowWindow(start, SW_SHOW);
			::UpdateWindow(start);
		}
	}
	::ShowWindow(iMainHWND, SW_SHOW);
	::UpdateWindow(iMainHWND);
	::SetFocus(iMainHWND);
/*
	if (_viewer)
		_viewer->onFullscreen(iFullscreen);*/
}

//----------------------------------------------------------------------------------------
// ���������� ��� ��������� ����
//
void Application::onActivate()
{
	/*if (_viewer)
	{
		_viewer->onResume(false);
	}*/
}

//----------------------------------------------------------------------------------------
// ���������� ��� ����������� ����
//
void Application::onDeactivate()
{/*
	if (_viewer)
		_viewer->onPause();*/
}

void Application::onMinimize()
{
	_isMinimize = true;
}

void Application::onMaximize()
{
	//if (_viewer)
	//{
	//	if (_isMinimize)
	//		_viewer->onResume(true);
	//}
	_isMinimize = false;
}

int	Application::say(int what, int data)
{
	switch (what)
	{
	case SAY_UPDATE_WINDOW:	updateWindowParams();	break;
	}
	return 0;
}

void	Application::updateWindowParams()
{
	if (iFullscreen)
	{
		//if (GEdit::attachedToSlotsObgect || GVideo::isActiveVideo())
		//{
		//	//��� ���� ������, ����� �������� �� ��������� �������� �������
		//	::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN);
		//	/* ������ ����������� ����
		//		�� ������ ��������:
		//		���� ��� �������� �������� ���� ������� � ����� WS_CLIPCHILDREN ���
		//		����� ��, �� ������ ��� �������� ������, � ��������� ������� ������ ��
		//		�����, ��� ������� ��������� ��� �� ������� ���������, ��� ��������� �
		//		����� � ��������� ��� ������� ����� ����� �������� �� ������� �
		//		�������� ���� ����������� �� ������ ������������.
		//		� ������ ������������ ������� ����� ������������ ������� � ���������,
		//		� ���� ����� �������� �� �������� � ������ (��� ���� ��������) �����
		//		������� ������������. �.� ������ ����� � ��������� ������������
		//		������� � ���� ���������� ��������� �����, ����� ������� ����������
		//		��� ���������. ������� ��� ������� ����� ���������. (c) 
		//	*/
		//	// �.�. ��� ����� �������� ���� ����� ���������������� ��������, ������� ���� ���.
		//	::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN);
		//}	
		//else
		//{
		//	::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN);
		//	::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_VISIBLE );
		//}

		// ����� ������ ������, ����� ����� ���������������� ��� ����� � �������������� ������
		::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN);
		::SetWindowLongPtr(iMainHWND, GWL_STYLE, WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN);
	}
}


Application::~Application()
{

}
