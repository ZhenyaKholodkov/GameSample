#include "GMoveableSystem.h"


GMoveableSystem::GMoveableSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GMoveableSystem>(manager)
{
}

GMoveableSystem::~GMoveableSystem()
{
}


void GMoveableSystem::update(int dt)
{
	if (isStoped())
		return;
	mEntityManager->each<GMoveableComponent, GLocationComponent>([&](Entity entity, GMoveableComponent& moveable, GLocationComponent& location)
	{
		if (moveable.mState == GMoveableComponent::STATE_STOP)
			return;

		switch (moveable.mState)
		{
		case GMoveableComponent::STATE_MOVE_DX:
			if (moveable.isInsideRectangle(GVector2(location.getX() + moveable.mDX, location.getY()))) 
			{
				location.setX(location.getX() + moveable.mDX);
				moveable.signal_LocationChanged(moveable.mDX, 0.0f);
			}
			break;
		case GMoveableComponent::STATE_MOVE_DY:
			if (moveable.isInsideRectangle(GVector2(location.getX(), location.getY() + moveable.mDY)))
			{
				location.setY(location.getY() + moveable.mDY);
				moveable.signal_LocationChanged(0.0f, moveable.mDY);
			}
			break;
		case GMoveableComponent::STATE_MOVE_DX_REVERT:
			if (moveable.isInsideRectangle(GVector2(location.getX() - moveable.mDX, location.getY())))
			{
				location.setX(location.getX() - moveable.mDX);
				moveable.signal_LocationChanged(-moveable.mDX, 0.0f);
			}
			break;
		case GMoveableComponent::STATE_MOVE_DY_REVERT:
			if (moveable.isInsideRectangle(GVector2(location.getX(), location.getY() - moveable.mDY)))
			{
				location.setY(location.getY() - moveable.mDY);
				moveable.signal_LocationChanged(0.0f, -moveable.mDY);
			}
			break;
		}
		moveable.signal_Moved();
		moveable.mState = GMoveableComponent::STATE_STOP;
	});
}