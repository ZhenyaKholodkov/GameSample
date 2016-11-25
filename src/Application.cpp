#include "Application.h"

Application*	Application::sInstance = NULL;

Application::Application() :
	Window(),
	mGame(NULL),
	mScreenWidth(1024),
	mScreenHeight(768)
{
	sInstance = this;
}

int32 Application::OnCreate()
{
	if (!CreateGLContext())
	{
		return 0;
	}

	int aScreenWidth = mScreenWidth;
	int aScreenHeight = mScreenHeight;

	int mWidth = (aScreenWidth > aScreenHeight) ? aScreenWidth : aScreenHeight;
	int mHeight = (aScreenWidth > aScreenHeight) ? aScreenHeight : aScreenWidth;

	ResDictionary dict;
	int32 hash1 = dict.hash("/temp/", "image.png");
	int32 hash2 = dict.hash("/temp/", "image.png");
	GRenderManager::Instance()->init(mWidth, mHeight);

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
	GPoint point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
	mGame->OnMouseDown(point);
}

void Application::OnLMouseUp(Pixel  mouse_pos)
{
	GPoint point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
	mGame->OnMouseUp(point);
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
	GPoint point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
}

void Application::OnMouseWheelUp(Pixel mouse_pos)
{
	GPoint point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
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
