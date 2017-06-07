#include "gRotableSystem.h"
#include "gRotableComponent.h"


GRotableSystem::GRotableSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GRotableSystem>(manager)
{
}

GRotableSystem::~GRotableSystem()
{
}


void GRotableSystem::update(int dt)
{
	mEntityManager->each<GRotableComponent, GRenderableComponent>([&](Entity entity, GRotableComponent& rotable, GRenderableComponent& renderable)
	{
		if (rotable.mState == GRotableComponent::STATE_WAIT)
			return;

		if (rotable.mCurrentTime == 0)
		{
			rotable.signal_AngleChangingBegin();
		}

		rotable.mCurrentTime += dt;
		if (rotable.mTime > rotable.mCurrentTime)
		{
			rotable.signal_AngleChanged(rotable.getCurrentAngle());
			renderable.setAngle(rotable.getCurrentAngle());
		}
		else
		{
			rotable.signal_AngleChanged(rotable.mEndAngle);
			renderable.setAngle(rotable.mEndAngle);
			rotable.signal_AngleChangingFinished();
			if (rotable.mIsInfinity)
			{
				rotable.reset();
			}
			else
			{
				rotable.setState(GRotableComponent::STATE_WAIT);
			}
		}
	});
}