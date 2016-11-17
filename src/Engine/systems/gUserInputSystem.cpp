#include "gUserInputSystem.h"

#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gActionComponent.h"
#include "gRenderableComponent.h"

GUserInputSystem::GUserInputSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GUserInputSystem::~GUserInputSystem()
{
}

void GUserInputSystem::OnMouseDown(GPoint point)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);

		GMouseDownEventComponent* mouseDown = mEntityManager->GetComponent<GMouseDownEventComponent>(entity);
		GRenderableComponent*     render    = mEntityManager->GetComponent<GRenderableComponent>(entity);

		if (!mouseDown || !render)
			continue;

		if (mEntityManager->IsInsideEntity(entity, point))
		{
			mouseDown->signal_MouseDown.emit();
			mouseDown->signal_MouseDownOnEntity.emit(entity);
			mouseDown->signal_MouseDownNewSprite.emit(entity, mouseDown->mSpriteDown);
		}
	}
}

void GUserInputSystem::OnMouseUp(GPoint point)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);

		GMouseUpEventComponent*   upDown = mEntityManager->GetComponent<GMouseUpEventComponent>(entity);
		GRenderableComponent*     render = mEntityManager->GetComponent<GRenderableComponent>(entity);

		if (!upDown || !render)
			continue;

		if (mEntityManager->IsInsideEntity(entity, point))
		{
			upDown->signal_MouseUp.emit();
			upDown->signal_MouseUpOnEntity.emit(entity);
			upDown->signal_MouseUpNewSprite.emit(entity, upDown->mSpriteUp);
		}
	}
}