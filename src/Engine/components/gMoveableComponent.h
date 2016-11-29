#ifndef GMOVEABLECOMPONENT_H
#define GMOVEABLECOMPONENT_H

#include "gComponent.h"

class GMoveableComponent : public GComponent<GMoveableComponent>, public sigslot::has_slots<>
{
	friend class GMoveableSystem;
public:
	GMoveableComponent() : signal_Data(nullptr), mXDestination(0.0f), mYDestination(0.0f), mMovingTime(0), mCurrentTime(0), mState(STATE_WAIT), mDX(0.0f), mDY(0.0f) {};
	GMoveableComponent(float begX, float begY, float xDest, float yDest, int time) :
		signal_Data(nullptr), mBeginX(begX), mBeginY(begY), mXDestination(xDest),
		mYDestination(yDest), mMovingTime(time), mCurrentTime(0), mState(STATE_WAIT)
	{
		recalcDxDy();
	};
	virtual ~GMoveableComponent() {};

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

	void Reset() { mCurrentTime = 0; }


public:/*slots*/
	void slot_Move()
	{
		Reset();
		SetState(GMoveableComponent::STATE_MOVE);
	}

public: /*signals*/
	sigslot::signal2<float, float>     signal_LocationChanged;
	sigslot::signal1<Entity>           signal_MovingFinished;
	sigslot::signal2<Entity, void*>    signal_MovingFinishedWithData;

	void* signal_Data;

private:
	void SetState(uint32 state) { mState = state; }

	void recalcDxDy()
	{
		mDX = mXDestination - mBeginX;
		mDY = mYDestination - mBeginY;
	}
private: 
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_MOVE = BIT(2)
	};
	float mBeginX;
	float mBeginY;

	float mXDestination;
	float mYDestination;

	int mMovingTime;
	int mCurrentTime;

	uint32 mState;

	float mDX;
	float mDY;
};

#endif //GMOVEABLECOMPONENT_H

