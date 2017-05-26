#include "gAnimationSystem.h"

#include "gAnimationComponent.h"

GAnimationSystem::GAnimationSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GAnimationSystem>(manager)
{
}

GAnimationSystem::~GAnimationSystem()
{
}

void GAnimationSystem::update(int dt)
{
	mEntityManager->each<GAnimationComponent, GRenderableComponent>([&](Entity entity, GAnimationComponent& animation, GRenderableComponent& renderable)
	{
		if (animation.mState == GAnimationComponent::STATE_WAIT)
			return;

		animation.mCurrentFrameTime += dt;
		if (animation.mCurrentFrameTime >= animation.mFrameTime)
		{
			if (animation.mCurrentFrame == (animation.mFrames.size() - 1))
			{
				if (animation.mIsLooped)
				{
					animation.mCurrentFrame = 0;
				}
				else
				{
					animation.mCurrentFrame = 0;
					animation.mState = GAnimationComponent::STATE_WAIT;
					animation.signal_AnimationFinished();
				}
			}
			else
			{
				animation.mCurrentFrame++;
			}

			renderable.SetSprite(animation.mFrames[animation.mCurrentFrame]);
			animation.mCurrentFrameTime = 0;
		}
	});
}