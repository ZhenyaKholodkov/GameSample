#ifndef GMOVEABLEANIMATIONCOMPONENT_H
#define GMOVEABLEANIMATIONCOMPONENT_H

#include "gComponent.h"
#include "gEasing.h"

#include <math.h>

class GMoveableAnimationComponent : public GComponent<GMoveableAnimationComponent>
{
	friend class GMoveableAnimationSystem;
public:
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_MOVE = BIT(2)
	};

	GMoveableAnimationComponent() : mXDestination(0.0f), mYDestination(0.0f), mMovingTime(0), mCurrentTime(0), mState(STATE_WAIT), mDX(0.0f), mDY(0.0f) {};
	GMoveableAnimationComponent(float begX, float begY, float xDest, float yDest, int time, bool repeat = false, GEasings::EasingType easing = GEasings::EasingType::NONE, bool destroyAfterFinished = false) :
		mBeginX(begX), mBeginY(begY), mXDestination(xDest), mYDestination(yDest), mMovingTime(time),
		mCurrentTime(0), mRepeat(repeat), mState(STATE_WAIT), mEasing(easing), mDestroyAfterFinished(destroyAfterFinished)
	{
		recalcDxDy();
	};
	virtual ~GMoveableAnimationComponent() 
	{
		signal_LocationChanged.disconnect_all_slots();
		signal_MovingFinished.disconnect_all_slots();
		signal_MovingFinishedEntity.disconnect_all_slots();
		signal_MovingFinishedWithData.disconnect_all_slots();
	};

	float getCurrentX()
	{
		return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mBeginX, mDX, static_cast<float>(mMovingTime));
	}
	float getCurrentY()
	{
		return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mBeginY, mDY, static_cast<float>(mMovingTime));
	}

	float GetXDestination() { return mXDestination; }
	void  SetXDestination(float xDest) { mXDestination = xDest; recalcDxDy(); }
	float GetYDestination() { return mYDestination; }
	void  SetYDestination(float yDest) { mYDestination = yDest; recalcDxDy(); }

	float GetBeginX() { return mBeginX; }
	void  SetBeginX(float x) { mBeginX = x; recalcDxDy(); }
	float GetBeginY() { return mBeginY; }
	void  SetBeginY(float y) { mBeginY = y; recalcDxDy(); }

	int   GetMvingTimne() { return mMovingTime; }
	void  SetMovingTime(int time) { mMovingTime = time; }

	void reset() { mCurrentTime = 0; }
	void setState(uint32 state) { mState = state; }
	uint32 getState() const { return mState; }

	bool destroyAfterFinished() const { return mDestroyAfterFinished; }
public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Move = boost::bind(&GMoveableAnimationComponent::setState, this, GMoveableAnimationComponent::STATE_MOVE);

public: /*signals*/
	boost::signals2::signal<void(float, float)>     signal_LocationChanged;
	boost::signals2::signal<void()>                 signal_MovingFinished;
	boost::signals2::signal<void(Entity)>           signal_MovingFinishedEntity;
	boost::signals2::signal<void(Entity)>           signal_MovingFinishedWithData;

private:
	void recalcDxDy()
	{
		mDX = mXDestination - mBeginX;
		mDY = mYDestination - mBeginY;
	}
private: 
	float mBeginX;
	float mBeginY;

	float mXDestination;
	float mYDestination;

	int mMovingTime;
	int mCurrentTime;

	bool mRepeat;
	bool mDestroyAfterFinished;    // destroy entity after finishing the moving

	uint32 mState;

	float mDX;
	float mDY;
	GEasings::EasingType mEasing;
};

#endif //GMOVEABLECOMPONENT_H

