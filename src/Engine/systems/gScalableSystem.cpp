#include "gScalableSystem.h"


GScalableSystem::GScalableSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GScalableSystem::~GScalableSystem()
{
}


void GScalableSystem::update(int dt)
{
	for (auto iter = mEntityManager->GetBeginPairComponent<GScalableComponent>(); iter != mEntityManager->GetEndPairComponent<GScalableComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GScalableComponent* scalable = (*iter)->second;
				
		if (scalable->mState == GScalableComponent::STATE_WAIT)
			continue;

		if (scalable->mCurrentTime == 0)
		{
			scalable->signal_ScaleChangingBegin();
		}

		scalable->mCurrentTime += dt;
		if (scalable->mTime > scalable->mCurrentTime)
		{
			scalable->signal_ScaleChanged(scalable->getCurrentXScale(), scalable->getCurrentYScale());
		}
		else
		{
			scalable->signal_ScaleChanged(scalable->mEndXScale, scalable->mEndYScale);
			scalable->signal_ScaleChangingFinished();
			scalable->SetState(GScalableComponent::STATE_WAIT);
		}
	}
}