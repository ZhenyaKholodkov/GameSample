#include "GMoveableAnimationSystem.h"


GMoveableAnimationSystem::GMoveableAnimationSystem()
{
	mEntityManager = GEntityManager::instance();
}

GMoveableAnimationSystem::~GMoveableAnimationSystem() 
{
}


void GMoveableAnimationSystem::update(int dt)
{
	auto componentPool = mEntityManager->getComponentPool<GMoveableAnimationComponent>();
	if (!componentPool)
		return;
	for (auto pair : *componentPool)
	{
		Entity entity = pair->first;
		GMoveableAnimationComponent* moveable = pair->second;

		if (!mEntityManager->doesHaveComponent<GLocationComponent>(entity) || !mEntityManager->doesHaveComponent<GMoveableAnimationComponent>(entity))
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