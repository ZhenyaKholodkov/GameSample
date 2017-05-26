#include "gScalableSystem.h"


GScalableSystem::GScalableSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GScalableSystem>(manager)
{
}

GScalableSystem::~GScalableSystem()
{
}


void GScalableSystem::update(int dt)
{
	mEntityManager->each<GScalableComponent>([&](Entity entity, GScalableComponent& scalable)
	{
		if (scalable.mState == GScalableComponent::STATE_WAIT)
			return;

		if (scalable.mCurrentTime == 0)
		{
			scalable.signal_ScaleChangingBegin();
		}

		scalable.mCurrentTime += dt;
		if (scalable.mTime > scalable.mCurrentTime)
		{
			scalable.signal_ScaleChanged(scalable.getCurrentXScale(), scalable.getCurrentYScale());
		}
		else
		{
			scalable.signal_ScaleChanged(scalable.mEndXScale, scalable.mEndYScale);
			scalable.signal_ScaleChangingFinished();
			scalable.SetState(GScalableComponent::STATE_WAIT);
		}
	});
}