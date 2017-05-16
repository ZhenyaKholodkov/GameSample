#include "gAnimationSystem.h"

#include "gAnimationComponent.h"

GAnimationSystem::GAnimationSystem()
{
	mEntityManager = GEntityManager::instance();
}

GAnimationSystem::~GAnimationSystem()
{
}

void GAnimationSystem::update(int dt)
{

	for (auto pair : *mEntityManager->getComponentPool<GAnimationComponent>())
	{
		Entity entity = pair->first;
		GAnimationComponent* animation = pair->second;

		if (!animation || !mEntityManager->doesHaveComponent<GRenderableComponent>(entity))
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
					animation->signal_AnimationFinished();
				}
			}
			else
			{
				animation->mCurrentFrame++;
			}

			GRenderableComponent* renderable = mEntityManager->getComponent<GRenderableComponent>(entity);
			renderable->SetSprite(animation->mFrames[animation->mCurrentFrame]);
			animation->mCurrentFrameTime = 0;
		}
	}
}