#include "Application.h"

Application*	Application::_instance = NULL;

Application::Application() :
	Window(),
	mGame(NULL),
	_screenWidth(1024),
	_screenHeight(768)
{
	_instance = this;
}

int32 Application::OnCreate()
{
	if (!CreateGLContext())
	{
		return 0;
	}

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
	return 1;
}

void Application::OnClose()
{
	DestroyGLContext();
}

void Application::OnTimer(int32 dt)
{
	mGame->Update(dt);
	mRenderContext->endFrame();
	UpdateContext();
}

void Application::OnKeyDown(uint32 msKey)
{
}

void Application::OnKeyUp(uint32 msKey)
{
}

void Application::OnLMouseDown(Pixel  mouse_pos)
{
	mGame->OnMouseDown(mouse_pos);
}

void Application::OnLMouseUp(Pixel  mouse_pos)
{
	mGame->OnMouseUp(mouse_pos);
}

void Application::OnMouseMove(Pixel mouse_pos)
{
	static int border = 10;
	//if (_currMouse.press)
	//{
	//	if (														
	//		(_currMouse.x<border) ||
	//		(_currMouse.y<border) ||
	//		(_currMouse.x>(_ctx->width() - border)) ||
	//		(_currMouse.y>(_ctx->height() - border))
	//		)
	//	{
	//		_currMouse.press = false;
	//	}
	//	else
	//}
}

void Application::OnMouseWheelDown(Pixel mouse_pos)
{
	GMouse m; m.x = (float)mouse_pos.X(); m.y = (float)mouse_pos.Y(); m.press = false;
}

void Application::OnMouseWheelUp(Pixel mouse_pos)
{
	GMouse m; m.x = (float)mouse_pos.X(); m.y = (float)mouse_pos.Y(); m.press = false;
}

void Application::onActivate()
{
}

void Application::onDeactivate()
{
}

void Application::onMinimize()
{
}

void Application::onMaximize()
{
}

Application::~Application()
{

}
