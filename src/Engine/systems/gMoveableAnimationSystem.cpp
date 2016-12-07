#include "GMoveableAnimationSystem.h"


GMoveableAnimationSystem::GMoveableAnimationSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GMoveableAnimationSystem::~GMoveableAnimationSystem() 
{
}


void GMoveableAnimationSystem::update(int dt)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GMoveableAnimationComponent>(); iter != mEntityManager->GetEndPairComponent<GMoveableAnimationComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GMoveableAnimationComponent* moveable = (*iter)->second;

		if (!mEntityManager->DoesHaveComponent<GLocationComponent>(entity) || !mEntityManager->DoesHaveComponent<GMoveableAnimationComponent>(entity))
			continue;
		
		if (moveable->mState == GMoveableAnimationComponent::STATE_WAIT)
			continue;
	
		moveable->mCurrentTime += dt;
		if (moveable->mMovingTime > moveable->mCurrentTime)
		{
			moveable->signal_LocationChanged(moveable->getCurrentX(), moveable->getCurrentY());
		}
		else
		{
			moveable->signal_LocationChanged(moveable->GetXDestination(), moveable->GetYDestination());
			moveable->signal_MovingFinished(entity);
			moveable->signal_MovingFinishedWithData(entity, moveable->signal_Data);
			moveable->SetState(GMoveableAnimationComponent::STATE_WAIT);
		}
	}
}