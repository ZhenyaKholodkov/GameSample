#include "gUserInputSystem.h"

#include "gMouseDownEventComponent.h"

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

		GBaseComponent* componentToChange = mEntityManager->GetComponent(entity, mouseDown->mComponentIndex);

		componentToChange->SetState(mouseDown->mStateToChange);
	}
}

void GUserInputSystem::OnMouseUp(Pixel mousePos)
{

}