#ifndef GANIMATIONCOMPONENT_H
#define GANIMATIONCOMPONENT_H

#include "Types.h"
#include "GSprite.h"
#include "gComponent.h"
#include <vector>

class GAnimationComponent : public GComponent<GAnimationComponent>
{
	friend class GAnimationSystem;
public:

	enum
	{
		STATE_WAIT,
		STATE_RUN
	};

	GAnimationComponent() :mCurrentFrame(0), mIsLooped(false), mFrameTime(0), mCurrentFrameTime(0), mState(STATE_WAIT) {};

	GAnimationComponent(int frameTime, bool mIsLooped) : mCurrentFrame(0), mIsLooped(mIsLooped), mFrameTime(frameTime),
		                                                 mCurrentFrameTime(0), mState(STATE_WAIT) {};
	
	virtual ~GAnimationComponent() {};

	void AddFrame(GSprite* sprite) { mFrames.push_back(sprite); }
	virtual void SetState(uint32 state) { mState = state; }

	virtual void Reset() { mCurrentFrame = 0; mCurrentFrameTime = 0; };
private:
	vector<GSprite*> mFrames;
	uint32           mCurrentFrame;

	bool             mIsLooped;

	int              mFrameTime;
	int              mCurrentFrameTime;

	uint32           mState;
};

#endif //GANIMATIONCOMPONENT_H

