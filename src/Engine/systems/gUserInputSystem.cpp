#include "gUserInputSystem.h"

#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gMouseMoveEventComponent.h"
#include "gRenderableComponent.h"

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
				moveMove->signal_MouseMovedOutEntity.emit(entity, moveMove->mSpriteMoveOut);
			}
		}
	}
}

void GUserInputSystem::OnKeyUp(uint32 msKey)
{

}

void GUserInputSystem::OnKeyDown(uint32 msKey)
{

}