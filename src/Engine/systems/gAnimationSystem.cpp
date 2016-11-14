#include "gAnimationSystem.h"

#include "gAnimationComponent.h"

GAnimationSystem::GAnimationSystem()
{
	mEntityManager = GEntityManager::Instance();
}

GAnimationSystem::~GAnimationSystem()
{
}

void GAnimationSystem::update(int dt)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);

		GAnimationComponent* animation = mEntityManager->GetComponent<GAnimationComponent>(entity);
		if (!animation)
			continue;

		if (animation->mState == GAnimationComponent::STATE_WAIT)
			return;

		animation->mCurrentFrameTime += dt;
		if (animation->mCurrentFrameTime >= animation->mFrameTime)
		{
			if (animation->mCurrentFrame == (animation->mFrames.size() - 1))
			{
				if (animation->mIsLooped)
				{
					animation->mCurrentFrame = 0;
				}
				else
				{
					animation->mState = GAnimationComponent::STATE_WAIT;
				}
			}
			else
			{
				animation->mCurrentFrame++;
			}

			GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(entity);
			renderable->SetSprite(animation->mFrames[animation->mCurrentFrame]);
			animation->mCurrentFrameTime = 0;
		}
	}
}

void GAnimationSystem::ProcessActions()
{

}