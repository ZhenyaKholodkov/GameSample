#include "gMoveableSystem.h"


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

		if (!mEntityManager->DoesHaveComponent<GLocationComponent>(entity) || !mEntityManager->DoesHaveComponent<GMoveableComponent>(entity))
			continue;
		
		if (moveable->mState == GMoveableComponent::STATE_WAIT)
			continue;
	
		moveable->mCurrentTime += dt;
		if (moveable->mMovingTime > moveable->mCurrentTime)
		{
			moveable->signal_LocationChanged(moveable->getCurrentX(), moveable->getCurrentY());
		}
		else
		{
			moveable->signal_LocationChanged(moveable->GetXDestination(), moveable->GetYDestination());
			moveable->signal_MovingFinished();
			moveable->SetState(GMoveableComponent::STATE_WAIT);
		}
	}
}