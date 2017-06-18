#include "GMoveableAnimationSystem.h"


GMoveableAnimationSystem::GMoveableAnimationSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GMoveableAnimationSystem>(manager)
{
}

GMoveableAnimationSystem::~GMoveableAnimationSystem() 
{
}


void GMoveableAnimationSystem::update(int dt)
{
	if (isStoped())
		return;
	mEntityManager->each<GMoveableAnimationComponent, GLocationComponent>([&](Entity entity, GMoveableAnimationComponent& moveable, GLocationComponent& location)
	{
		if (moveable.mState == GMoveableAnimationComponent::STATE_WAIT)
			return;
	
		moveable.mCurrentTime += dt;
		if (moveable.mMovingTime > moveable.mCurrentTime)
		{
			moveable.signal_LocationChanged(moveable.getCurrentX(), moveable.getCurrentY());
			location.setX(moveable.getCurrentX());
			location.setY(moveable.getCurrentY());
		}
		else
		{
			location.setX(moveable.GetXDestination());
			location.setY(moveable.GetYDestination());
			if(moveable.mRepeat)
			{ 
				moveable.reset();
			}
			else
			{
				moveable.setState(GMoveableAnimationComponent::STATE_WAIT);
			}
			moveable.signal_LocationChanged(moveable.GetXDestination(), moveable.GetYDestination());
			moveable.signal_MovingFinished();
			moveable.signal_MovingFinishedEntity(entity);
			moveable.signal_MovingFinishedWithData(entity);
			if (moveable.destroyAfterFinished())
				mEntityManager->destroyEntity(entity);
		}
	});
}