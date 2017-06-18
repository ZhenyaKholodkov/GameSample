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
	if (isStoped())
		return;
	mEntityManager->each<GScalableComponent, GRenderableComponent>([&](Entity entity, GScalableComponent& scalable, GRenderableComponent& renderable)
	{
		if (scalable.mState == GScalableComponent::STATE_WAIT)
			return;

		if (scalable.mCurrentTime == 0)
		{
			scalable.signal_ScaleChangingBegin();
		}

		scalable.mCurrentTime += dt;
		scalable.signal_ScaleChanged(scalable.getCurrentXScale(), scalable.getCurrentYScale());
		renderable.setXScale(scalable.getCurrentXScale());
		renderable.setYScale(scalable.getCurrentYScale());
		if (scalable.mTime <= scalable.mCurrentTime)
		{
			scalable.signal_ScaleChangingFinished();
			scalable.setState(GScalableComponent::STATE_WAIT);
		}
	});
}