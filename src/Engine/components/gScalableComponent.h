#ifndef GSCALABLECOMPONENT_H
#define GSCALABLECOMPONENT_H

#include "gComponent.h"
#include "gEasing.h"
#include "boost/signals2.hpp"

class GScalableComponent : public GComponent<GScalableComponent>
{
	friend class GScalableSystem;
public:
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_SCALE = BIT(2),
		STATE_REVERT_SCALE = BIT(3)
	};

	GScalableComponent(float beginXScale, float beginYScale, float endXScale, float endYScale, int time, GEasings::EasingType easing = GEasings::EasingType::NONE)
		: mBeginXScale(beginXScale), mBeginYScale(beginYScale), mEndXScale(endXScale), mEndYScale(endYScale),
		  mTime(time), mCurrentTime(0), mState(STATE_WAIT), mEasing(easing)
	{
		recalcDxDy();
	};
	virtual ~GScalableComponent() 
	{
	};

	float getCurrentXScale()
	{
		if (mState == STATE_SCALE)
			return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mBeginXScale, mDSaleX, static_cast<float>(mTime));
		else
			return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mEndXScale, -mDSaleX, static_cast<float>(mTime));
	}

	float getCurrentYScale()
	{
		if (mState == STATE_SCALE)
			return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mBeginYScale, mDSaleY, static_cast<float>(mTime));
		else
			return GEasings::calculateValueWithEasing(mEasing, static_cast<float>(mCurrentTime), mEndYScale, -mDSaleY, static_cast<float>(mTime));
	}

	void setBeginXScale(float scale) { mBeginXScale = scale; recalcDxDy(); }
	void setBeginYScale(float scale) { mBeginYScale = scale; recalcDxDy(); }
	void setEndXScale(float scale){ mEndXScale = scale; recalcDxDy(); }
	void setEndYScale(float scale){ mEndYScale = scale; recalcDxDy(); }

	void Reset() { mCurrentTime = 0; }
	void setState(uint32 state) 
	{ 
		if (state != STATE_WAIT)
			Reset();
		mState = state;
	}

public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Scale = boost::bind(&GScalableComponent::setState, this, GScalableComponent::STATE_SCALE);
	const boost::signals2::signal<void()>::slot_type slot_RevertScale = boost::bind(&GScalableComponent::setState, this, GScalableComponent::STATE_REVERT_SCALE);
public: /*signals*/
	boost::signals2::signal<void(float, float)>  signal_ScaleChanged;
	boost::signals2::signal<void()>              signal_ScaleChangingBegin;
	boost::signals2::signal<void()>              signal_ScaleChangingFinished;


private:
	void recalcDxDy()
	{
		mDSaleX = mEndXScale - mBeginXScale;
		mDSaleY = mEndYScale - mBeginYScale;
	}
private: 
	float mBeginXScale;
	float mBeginYScale;

	float mEndXScale;
	float mEndYScale;

	float mDSaleX;       //!< need x scale change
	float mDSaleY;		 //!< need y scale change

	int    mTime;
	int    mCurrentTime;
	uint32 mState;

	GEasings::EasingType mEasing;
};

#endif //GSCALABLECOMPONENT_H

