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
	if (isStoped())
		return;
	mEntityManager->each<GRotableComponent, GRenderableComponent>([&](Entity entity, GRotableComponent& rotable, GRenderableComponent& renderable)
	{
		if (rotable.mState == GRotableComponent::STATE_WAIT)
			return;

		if (rotable.mCurrentTime == 0)
		{
			rotable.signal_AngleChangingBegin();
		}

		if (rotable.mCurrentTime  < rotable.mTime)
		{
			rotable.mCurrentTime += dt;
			float newAngle = renderable.getAngle() + rotable.getCurrentVelocity();
			renderable.setAngle(newAngle > 360.0f ? newAngle - 360.0f : newAngle);
			rotable.signal_AngleChanged(renderable.getAngle());
			if (rotable.mCurrentTime >= rotable.mTime)
			{
				rotable.signal_AngleChangingFinished();
				rotable.signal_VelocityChangingFinishedEntity(entity);
			}
		}
		else
		{
			if (rotable.mContinueRotation)
			{
				float newAngle = renderable.getAngle() + rotable.getEndVelocity();
				renderable.setAngle(newAngle > 360.0f ? newAngle - 360.0f : newAngle);
				rotable.signal_AngleChanged(newAngle);
			}
			else
			{
				rotable.signal_AngleChangingFinished();
				rotable.signal_AngleChangingFinishedEntity(entity);
				rotable.setState(GRotableComponent::STATE_WAIT);
			}
		}
	});
}