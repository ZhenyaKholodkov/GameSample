#ifndef GMOVEABLEANIMATIONCOMPONENT_H
#define GMOVEABLEANIMATIONCOMPONENT_H

#include "gComponent.h"

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
	GMoveableAnimationComponent(float begX, float begY, float xDest, float yDest, int time, bool repeat = false) :
		mBeginX(begX), mBeginY(begY), mXDestination(xDest),
		mYDestination(yDest), mMovingTime(time), mCurrentTime(0), mRepeat(repeat), mState(STATE_WAIT)
	{
		recalcDxDy();
	};
	virtual ~GMoveableAnimationComponent() 
	{
		signal_LocationChanged.disconnect_all_slots();
		signal_MovingFinished.disconnect_all_slots();
		signal_MovingFinishedWithData.disconnect_all_slots();
	};

	float getCurrentX()
	{
		return mBeginX + mDX * mCurrentTime / mMovingTime;
	}
	float getCurrentY()
	{
		return mBeginY + mDY * mCurrentTime / mMovingTime;
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


public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Move = boost::bind(&GMoveableAnimationComponent::setState, this, GMoveableAnimationComponent::STATE_MOVE);

public: /*signals*/
	boost::signals2::signal<void(float, float)>     signal_LocationChanged;
	boost::signals2::signal<void(Entity)>           signal_MovingFinished;
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

	uint32 mState;

	float mDX;
	float mDY;
};

#endif //GMOVEABLECOMPONENT_H

