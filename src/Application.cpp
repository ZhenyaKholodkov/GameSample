#include "Application.h"

Application*	Application::sInstance = NULL;

Application::Application() :
	Window(),
	mGame(NULL),
	mScreenWidth(1024),
	mScreenHeight(768), 
	mCurrentMousePressed(false)
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

	GRenderManager::Instance()->init(mWidth, mHeight);

	srand(time(0));

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

void Application::OnKeyDown(uint32 key)
{
	mGame->keyDown(convertToGKey(key));
}

void Application::OnKeyUp(uint32 key)
{
	mGame->keyUp(convertToGKey(key));
}

void Application::OnLMouseDown(Pixel  mouse_pos)
{
	mCurrentMousePressed = true;

	GCursor point;
	point.x = (float)mouse_pos.X();
	point.y = (float)mouse_pos.Y();
	point.mWasPressed = mCurrentMousePressed;

	mGame->OnMouseDown(point);
}

void Application::OnLMouseUp(Pixel  mouse_pos)
{
	mCurrentMousePressed = false;

	GCursor point; 
	point.x = (float)mouse_pos.X();
	point.y = (float)mouse_pos.Y();
	point.mWasPressed = mCurrentMousePressed;

	mGame->OnMouseUp(point);
}

void Application::OnMouseMove(Pixel mouse_pos)
{
	GCursor point;
	point.x = (float)mouse_pos.X();
	point.y = (float)mouse_pos.Y();
	point.mWasPressed = mCurrentMousePressed;

	mGame->OnMouseMove(point);
}

void Application::OnMouseWheelDown(Pixel mouse_pos)
{
	GCursor point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
}

void Application::OnMouseWheelUp(Pixel mouse_pos)
{
	GCursor point; point.x = (float)mouse_pos.X(); point.y = (float)mouse_pos.Y();
}

GKey Application::convertToGKey(uint32 key)
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
}

Application::~Application()
{

}
