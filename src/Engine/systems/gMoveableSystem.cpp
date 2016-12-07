#include "GMoveableSystem.h"


GMoveableSystem::GMoveableSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GMoveableSystem::~GMoveableSystem()
{
}


void GMoveableSystem::update(int dt)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GMoveableComponent>(); iter != mEntityManager->GetEndPairComponent<GMoveableComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GMoveableComponent* moveable = (*iter)->second;

		if (!mEntityManager->DoesHaveComponent<GLocationComponent>(entity))
			continue;

		if (moveable->mState == GMoveableComponent::STATE_STOP)
			continue;

		switch (moveable->mState)
		{
		case GMoveableComponent::STATE_MOVE_DX:
			moveable->signal_LocationChanged(moveable->mDX, 0.0f);
			break;
		case GMoveableComponent::STATE_MOVE_DY:
			moveable->signal_LocationChanged(0.0f, moveable->mDY);
			break;
		case GMoveableComponent::STATE_MOVE_DX_REVERT:
			moveable->signal_LocationChanged(-moveable->mDX, 0.0f);
			break;
		case GMoveableComponent::STATE_MOVE_DY_REVERT:
			moveable->signal_LocationChanged(0.0f, -moveable->mDY);
			break;
		}
		moveable->signal_Moved(entity);
		//moveable->mState = GMoveableComponent::STATE_STOP;
	}
}