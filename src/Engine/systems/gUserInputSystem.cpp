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

void GUserInputSystem::OnMouseDown(Pixel mousePos)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);

		GMouseDownEventComponent* mouseDown = mEntityManager->GetComponent<GMouseDownEventComponent>(entity);
		GRenderableComponent*     render    = mEntityManager->GetComponent<GRenderableComponent>(entity);

		if (!mouseDown || !render)
			continue;

		render->SetSprite(mouseDown->mSpriteDown);
		GActionComponent* actions = mEntityManager->GetComponent<GActionComponent>(mouseDown->mEntityToNotify);
		if(actions)
			actions->AddActions(mouseDown->mActionMask);
	}
}

void GUserInputSystem::OnMouseUp(Pixel mousePos)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);

		GMouseUpEventComponent*   upDown = mEntityManager->GetComponent<GMouseUpEventComponent>(entity);
		GRenderableComponent*     render = mEntityManager->GetComponent<GRenderableComponent>(entity);

		if (!upDown || !render)
			continue;

		render->SetSprite(upDown->mSpriteUp);
		GActionComponent* actions = mEntityManager->GetComponent<GActionComponent>(upDown->mEntityToNotify);
		if (actions)
		   actions->AddActions(upDown->mActionMask);
	}
}