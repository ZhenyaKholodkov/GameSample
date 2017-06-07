#ifndef GSCALABLECOMPONENT_H
#define GSCALABLECOMPONENT_H

#include "gComponent.h"
#include "boost/signals2.hpp"

class GScalableComponent : public GComponent<GScalableComponent>
{
	friend class GScalableSystem;
public:
	enum
	{
		STATE_WAIT = BIT(1),
		STATE_SCALE = BIT(2)
	};

	GScalableComponent(float beginXScale, float beginYScale, float endXScale, float endYScale, int time)
		: mBeginXScale(beginXScale), mBeginYScale(beginYScale), mEndXScale(endXScale), mEndYScale(endYScale),
		  mTime(time), mCurrentTime(0), mState(STATE_WAIT)
	{
		recalcDxDy();
	};
	virtual ~GScalableComponent() 
	{
		signal_ScaleChanged.~signal();
		signal_ScaleChangingBegin.~signal();
		signal_ScaleChangingFinished.~signal();
	};

	float getCurrentXScale()
	{
		return mBeginXScale + mDSaleX * mCurrentTime / mTime;
	}
	float getCurrentYScale()
	{
		return mBeginYScale + mDSaleY * mCurrentTime / mTime;
	}

	void setBeginXScale(float scale) { mBeginXScale = scale; recalcDxDy(); }
	void setBeginYScale(float scale) { mBeginYScale = scale; recalcDxDy(); }
	void setEndXScale(float scale){ mEndXScale = scale; recalcDxDy(); }
	void setEndYScale(float scale){ mEndYScale = scale; recalcDxDy(); }

	void Reset() { mCurrentTime = 0; }
	void SetState(uint32 state) { mState = state; }

public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_Scale = boost::bind(&GScalableComponent::SetState, this, GScalableComponent::STATE_SCALE);
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

	int mTime;
	int mCurrentTime;
	uint32 mState;
};

#endif //GSCALABLECOMPONENT_H

