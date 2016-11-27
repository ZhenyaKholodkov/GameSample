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
	for (auto iter = mEntityManager->GetBeginPairComponent<GAnimationComponent>(); iter != mEntityManager->GetEndPairComponent<GAnimationComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GAnimationComponent* animation = (*iter)->second;

		if (!animation || !mEntityManager->DoesHaveComponent<GRenderableComponent>(entity))
			continue;

		if (animation->mState == GAnimationComponent::STATE_WAIT)
			continue;

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
					animation->mCurrentFrame = 0;
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