#ifndef GROTABLECOMPONENT_H
#define GROTABLECOMPONENT_H

#include "gComponent.h"
#include "boost/signals2.hpp"
#include "gEasing.h" 

class GRotableComponent : public GComponent<GRotableComponent>
{
	friend class GRotableSystem;
public:
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_ROTATE = BIT(2)
	};
	GRotableComponent(float begVelocity, float endVelocity, int time = 0, bool continueRot = true, GEasings::EasingType easing = GEasings::EasingType::NONE)
		: mBegVelocity(begVelocity), mEndVelocity(endVelocity), mContinueRotation(continueRot), mEasing(easing), mState(STATE_WAIT), mTime(time), mCurrentTime(0), mCurrentVelocity(begVelocity)
	{

	}

	virtual ~GRotableComponent()
	{
	};

	float getCurrentVelocity()
	{
		return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mBegVelocity, mEndVelocity - mBegVelocity, static_cast<float>(mTime));
	}

	void setBegVelocity(float velocity) { mBegVelocity = velocity; }
	float getBegVelocity() const { return mBegVelocity; }
	void setEndVelocity(float velocity) { mEndVelocity = velocity; }
	float getEndVelocity() const { return mEndVelocity; }

	void reset() { mCurrentTime = 0; }
	void setState(uint32 state) { mState = state; }

	void setEasing(GEasings::EasingType type) { mEasing = type; }

public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Rotate = boost::bind(&GRotableComponent::setState, this, GRotableComponent::STATE_ROTATE);
public: /*signals*/
	boost::signals2::signal<void(float)>         signal_AngleChanged;
	boost::signals2::signal<void()>              signal_AngleChangingBegin;
	boost::signals2::signal<void()>              signal_AngleChangingFinished;
	boost::signals2::signal<void(Entity)>        signal_AngleChangingFinishedEntity;
	boost::signals2::signal<void(Entity)>        signal_VelocityChangingFinishedEntity;

private: 
	float mBegVelocity;
	float mEndVelocity;
	float mCurrentVelocity;

	bool mContinueRotation;

	int mTime;
	int mCurrentTime;
	uint32 mState;

	GEasings::EasingType mEasing;
};

#endif //GROTABLECOMPONENT_H

