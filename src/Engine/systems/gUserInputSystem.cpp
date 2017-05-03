#include "gUserInputSystem.h"


GUserInputSystem::GUserInputSystem()
{
	mEntityManager = GEntityManager::instance();
}

GUserInputSystem::~GUserInputSystem()
{
}

void GUserInputSystem::OnMouseDown(GCursor point)
{
	for (auto pair : mEntityManager->getComponentPool<GMouseDownEventComponent>())
	{
		Entity entity = pair->first;
		GMouseDownEventComponent* mouseDown = pair->second;

		if (!mouseDown)
			continue;

		if (mEntityManager->isInsideEntity(entity, point))
		{
			mouseDown->signal_MouseDown.emit();
			mouseDown->signal_MouseDownOnEntity.emit(entity);
			mouseDown->signal_MouseDownNewSprite.emit(entity, mouseDown->mSpriteDown);
		}
	}
}

void GUserInputSystem::OnMouseUp(GCursor point)
{
	for (auto pair : mEntityManager->getComponentPool<GMouseUpEventComponent>())
	{
		Entity entity = pair->first;
		GMouseUpEventComponent* upDown = pair->second;

		if (!upDown)
			continue;

		if (mEntityManager->isInsideEntity(entity, point))
		{
			upDown->signal_MouseUp.emit();
			upDown->signal_MouseUpOnEntity.emit(entity);
			upDown->signal_MouseUpNewSprite.emit(entity, upDown->mSpriteUp);
		}
	}
}

void GUserInputSystem::OnMouseMove(GCursor point)
{
	for (auto pair : mEntityManager->getComponentPool<GMouseMoveEventComponent>())
	{
		Entity entity = pair->first;
		GMouseMoveEventComponent* moveMove = pair->second;

		if (!moveMove)
			continue;

		if (mEntityManager->isInsideEntity(entity, point))
		{
			if (point.mWasPressed) 
			{
				moveMove->signal_PressedMouseMovedIn.emit(point.x, point.y);
			}
			moveMove->signal_MouseMovedInEntity.emit(entity, moveMove->mSpriteMoveIn);
			moveMove->mMovedIn = true;
		}
		else
		{
			if (moveMove->mMovedIn)
			{
				if (point.mWasPressed)
				{
					moveMove->signal_PressedMouseMovedOut.emit(point.x, point.y);
				}
				//moveMove->signal_MouseMovedOutEntity.emit(entity, moveMove->mSpriteMoveOut);
			}
		}
	}
}

void GUserInputSystem::OnKeyUp(GKey key)
{
	for (auto pair : mEntityManager->getComponentPool<GKeyUpEventComponent>())
	{
		Entity entity = pair->first;
		GKeyUpEventComponent* keyUp = pair->second;
		switch (key)
		{
		case KEY_LEFT:
			keyUp->signal_KeyLeft();
			break;
		case KEY_UP:
			keyUp->signal_KeyUp();
			break;
		case KEY_RIGHT:
			keyUp->signal_KeyRight();
			break;
		case KEY_DOWN:
			keyUp->signal_KeyDown();
			break;
		}
	}
}

void GUserInputSystem::OnKeyDown(GKey key)
{
	for (auto pair : mEntityManager->getComponentPool<GKeyDownEventComponent>())
	{
		Entity entity = pair->first;
		GKeyDownEventComponent* keyDown = pair->second;
		switch (key)
		{
		case KEY_LEFT:
			keyDown->signal_KeyLeft();
			break;
		case KEY_UP:
			keyDown->signal_KeyUp();
			break;
		case KEY_RIGHT:
			keyDown->signal_KeyRight();
			break;
		case KEY_DOWN:
			keyDown->signal_KeyDown();
			break;
		}
	}
}