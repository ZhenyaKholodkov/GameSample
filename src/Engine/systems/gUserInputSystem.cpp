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

		render->SetSprite(mouseDown->mSpriteDown);
		GActionComponent* actions = mEntityManager->GetComponent<GActionComponent>(entity);
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

		render->SetSprite(upDown->mSpriteDown);
		GActionComponent* actions = mEntityManager->GetComponent<GActionComponent>(entity);
		actions->AddActions(upDown->mActionMask);
	}
}