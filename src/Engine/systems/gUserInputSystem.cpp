#include "gUserInputSystem.h"

#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gMouseMoveEventComponent.h"
#include "gRenderableComponent.h"
#include "gKeyUpEventComponent.h"
#include "gKeyDownEventComponent.h"

GUserInputSystem::GUserInputSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GUserInputSystem::~GUserInputSystem()
{
}

void GUserInputSystem::OnMouseDown(GCursor point)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GMouseDownEventComponent>(); iter != mEntityManager->GetEndPairComponent<GMouseDownEventComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GMouseDownEventComponent* mouseDown = (*iter)->second;

		if (!mouseDown)
			continue;

		if (mEntityManager->IsInsideEntity(entity, point))
		{
			mouseDown->signal_MouseDown.emit();
			mouseDown->signal_MouseDownOnEntity.emit(entity);
			mouseDown->signal_MouseDownNewSprite.emit(entity, mouseDown->mSpriteDown);
		}
	}
}

void GUserInputSystem::OnMouseUp(GCursor point)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GMouseUpEventComponent>(); iter != mEntityManager->GetEndPairComponent<GMouseUpEventComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GMouseUpEventComponent* upDown = (*iter)->second;

		if (!upDown)
			continue;

		if (mEntityManager->IsInsideEntity(entity, point))
		{
			upDown->signal_MouseUp.emit();
			upDown->signal_MouseUpOnEntity.emit(entity);
			upDown->signal_MouseUpNewSprite.emit(entity, upDown->mSpriteUp);
		}
	}
}

void GUserInputSystem::OnMouseMove(GCursor point)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GMouseMoveEventComponent>(); iter != mEntityManager->GetEndPairComponent<GMouseMoveEventComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GMouseMoveEventComponent* moveMove = (*iter)->second;

		if (!moveMove)
			continue;

		if (mEntityManager->IsInsideEntity(entity, point))
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
	for (auto iter = mEntityManager->GetBeginPairComponent<GKeyUpEventComponent>(); iter != mEntityManager->GetEndPairComponent<GKeyUpEventComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GKeyUpEventComponent* keyUp = (*iter)->second;
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
	for (auto iter = mEntityManager->GetBeginPairComponent<GKeyDownEventComponent>(); iter != mEntityManager->GetEndPairComponent<GKeyDownEventComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GKeyDownEventComponent* keyDown = (*iter)->second;
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