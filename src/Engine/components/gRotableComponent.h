#ifndef GROTABLECOMPONENT_H
#define GROTABLECOMPONENT_H

#include "gComponent.h"
#include "boost/signals2.hpp"

class GRotableComponent : public GComponent<GRotableComponent>
{
	friend class GRotableSystem;
public:
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_ROTATE = BIT(2)
	};

	GRotableComponent(float beginAngle, float endAngle, int time, bool isInfinity)
		: mBeginAngle(beginAngle), mEndAngle(endAngle), mIsInfinity(isInfinity),
		  mTime(time), mCurrentTime(0), mState(STATE_WAIT)
	{
		recalcDAngle();
	};
	virtual ~GRotableComponent()
	{
		signal_AngleChanged.~signal();
		signal_AngleChangingBegin.~signal();
		signal_AngleChangingFinished.~signal();
	};

	float getCurrentAngle()
	{
		return mBeginAngle + mDAngle * mCurrentTime / mTime;
	}

	void setBeginXScale(float angle) { mBeginAngle = angle; recalcDAngle(); }
	void setEndXScale(float angle){ mEndAngle = angle; recalcDAngle(); }

	void reset() { mCurrentTime = 0; }
	void setState(uint32 state) { mState = state; }

public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Rotate = boost::bind(&GRotableComponent::setState, this, GRotableComponent::STATE_ROTATE);
public: /*signals*/
	boost::signals2::signal<void(float)>         signal_AngleChanged;
	boost::signals2::signal<void()>              signal_AngleChangingBegin;
	boost::signals2::signal<void()>              signal_AngleChangingFinished;


private:
	void recalcDAngle()
	{
		mDAngle = mEndAngle - mBeginAngle;
	}
private: 
	float mBeginAngle;
	float mEndAngle;
	float mDAngle;
	bool  mIsInfinity;

	int mTime;
	int mCurrentTime;
	uint32 mState;
};

#endif //GROTABLECOMPONENT_H

