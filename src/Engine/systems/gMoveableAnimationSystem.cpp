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
			moveable.signal_LocationChanged(moveable.GetXDestination(), moveable.GetYDestination());
			location.setX(moveable.GetXDestination());
			location.setY(moveable.GetYDestination());
			moveable.signal_MovingFinished(entity);
			moveable.signal_MovingFinishedWithData(entity);
			if(moveable.mRepeat)
			{ 
				moveable.reset();
			}
			else
			{
				moveable.setState(GMoveableAnimationComponent::STATE_WAIT);
			}
		}
	});
}