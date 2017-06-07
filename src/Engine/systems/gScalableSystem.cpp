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
	mEntityManager->each<GScalableComponent, GRenderableComponent>([&](Entity entity, GScalableComponent& scalable, GRenderableComponent& renderable)
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
			renderable.setXScale(scalable.getCurrentXScale());
			renderable.setYScale(scalable.getCurrentYScale());
		}
		else
		{
			scalable.signal_ScaleChanged(scalable.mEndXScale, scalable.mEndYScale);
			renderable.setXScale(scalable.mEndXScale);
			renderable.setYScale(scalable.mEndYScale);
			scalable.signal_ScaleChangingFinished();
			scalable.SetState(GScalableComponent::STATE_WAIT);
		}
	});
}