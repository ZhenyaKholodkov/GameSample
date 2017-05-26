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
	mEntityManager->each<GMoveableComponent, GLocationComponent>([&](Entity entity, GMoveableComponent& moveable, GLocationComponent& location)
	{
		if (moveable.mState == GMoveableComponent::STATE_STOP)
			return;

		switch (moveable.mState)
		{
		case GMoveableComponent::STATE_MOVE_DX:
			moveable.signal_LocationChanged(moveable.mDX, 0.0f);
			break;
		case GMoveableComponent::STATE_MOVE_DY:
			moveable.signal_LocationChanged(0.0f, moveable.mDY);
			break;
		case GMoveableComponent::STATE_MOVE_DX_REVERT:
			moveable.signal_LocationChanged(-moveable.mDX, 0.0f);
			break;
		case GMoveableComponent::STATE_MOVE_DY_REVERT:
			moveable.signal_LocationChanged(0.0f, -moveable.mDY);
			break;
		}
		moveable.signal_Moved(entity);
		//moveable.mState = GMoveableComponent::STATE_STOP;
	});
}