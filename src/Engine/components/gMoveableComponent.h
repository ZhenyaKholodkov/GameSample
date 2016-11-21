#ifndef GMOVEABLECOMPONENT_H
#define GMOVEABLECOMPONENT_H

#include "gComponent.h"

class GMoveableComponent : public GComponent<GMoveableComponent>, public sigslot::has_slots<>
{
	friend class GMoveableSystem;
public:
	GMoveableComponent() : mXDestination(0.0f), mYDestination(0.0f), mMovingTime(0), mCurrentTime(0), mState(STATE_WAIT) {};
	GMoveableComponent(float xDest, float yDest, int time) : mXDestination(xDest), mYDestination(yDest), mMovingTime(time), mCurrentTime(0), mState(STATE_WAIT){};
	virtual ~GMoveableComponent() {};

	float GetXDestination() { return mXDestination; }
	void  SetXDestination(float xDest) { mXDestination = xDest; }
	float GetYDestination() { return mYDestination; }
	void  SetYDestination(float yDest) { mYDestination = yDest; }

	int   GetMvingTimne() { return mMovingTime; }
	void  SetMvingTimne(int time) { mMovingTime = time; }

	void Reset() { mCurrentTime = 0; }

public:/*slots*/
	void slot_Move()
	{
		SetState(GMoveableComponent::STATE_MOVE);
	}

public: /*signals*/
	sigslot::signal0<>                 signal_MovingFinished;

private:
	void SetState(uint32 state) { mState = state; }

private: 
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_MOVE = BIT(2)
	};

	float mXDestination;
	float mYDestination;

	int mMovingTime;
	int mCurrentTime;

	uint32 mState;
};

#endif //GMOVEABLECOMPONENT_H

