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

		GLocationComponent* location = mEntityManager->GetComponent<GLocationComponent>(entity);	
		moveable->mCurrentTime += dt;
		if (moveable->mMovingTime > moveable->mCurrentTime)
		{
			float dx = moveable->GetXDestination() - location->getDefaultX();
			float dy = moveable->GetYDestination() - location->getDefaultY();
			location->setX(location->getDefaultX() + dx*moveable->mCurrentTime / moveable->mMovingTime);
			location->setY(location->getDefaultY() + dy*moveable->mCurrentTime / moveable->mMovingTime);
		}
		else
		{
			location->setX(moveable->GetXDestination());
			location->setY(moveable->GetYDestination());
			moveable->SetState(GMoveableComponent::STATE_WAIT);
		}
	}
}