#include "gUserInputSystem.h"


GUserInputSystem::GUserInputSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GUserInputSystem>(manager)
{
}

GUserInputSystem::~GUserInputSystem()
{
}

void GUserInputSystem::OnMouseDown(GCursor point)
{
	mEntityManager->each<GMouseDownEventComponent>([&](Entity entity, GMouseDownEventComponent& mouseDown)
	{
		if (mEntityManager->isInsideEntity(entity, point))
		{
			mouseDown.signal_MouseDown();
			mouseDown.signal_MouseDownOnEntity(entity);
			mouseDown.signal_MouseDownNewSprite(mouseDown.mSpriteDown);
		}
	});
}

void GUserInputSystem::OnMouseUp(GCursor point)
{
	mEntityManager->each<GMouseUpEventComponent>([&](Entity entity, GMouseUpEventComponent& upDown)
	{
		if (mEntityManager->isInsideEntity(entity, point))
		{
			upDown.signal_MouseUp();
			upDown.signal_MouseUpOnEntity(entity);
			upDown.signal_MouseUpNewSprite(upDown.mSpriteUp);
		}
	});
}

void GUserInputSystem::OnMouseMove(GCursor point)
{
	mEntityManager->each<GMouseMoveEventComponent>([&](Entity entity, GMouseMoveEventComponent& mouseMove)
	{
		if (mEntityManager->isInsideEntity(entity, point))
		{
			if (point.mWasPressed)
			{
				mouseMove.signal_PressedMouseMovedIn(point.x, point.y);
			}
			mouseMove.signal_MouseMovedInEntity(mouseMove.mSpriteMoveIn);
			mouseMove.mMovedIn = true;
		}
		else
		{
			if (mouseMove.mMovedIn)
			{
				if (point.mWasPressed)
				{
					mouseMove.signal_PressedMouseMovedOut(point.x, point.y);
				}
				mouseMove.signal_MouseMovedOutEntity(mouseMove.mSpriteMoveOut);
			}
		}
	});
}

void GUserInputSystem::OnKeyUp(GKey key)
{
	mEntityManager->each<GKeyUpEventComponent>([&](Entity entity, GKeyUpEventComponent& keyUp)
	{
		switch (key)
		{
		case KEY_LEFT:
			keyUp.signal_KeyLeft();
			break;
		case KEY_UP:
			keyUp.signal_KeyUp();
			break;
		case KEY_RIGHT:
			keyUp.signal_KeyRight();
			break;
		case KEY_DOWN:
			keyUp.signal_KeyDown();
			break;
		}
	});
}

void GUserInputSystem::OnKeyDown(GKey key)
{
	mEntityManager->each<GKeyDownEventComponent>([&](Entity entity, GKeyDownEventComponent& keyDown)
	{
		switch (key)
		{
		case KEY_LEFT:
			keyDown.signal_KeyLeft();
			break;
		case KEY_UP:
			keyDown.signal_KeyUp();
			break;
		case KEY_RIGHT:
			keyDown.signal_KeyRight();
			break;
		case KEY_DOWN:
			keyDown.signal_KeyDown();
			break;
		}
	});
}