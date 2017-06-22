#ifndef GMOVEABLECOMPONENT_H
#define GMOVEABLECOMPONENT_H

#include "gComponent.h"

class GMoveableComponent : public GComponent<GMoveableComponent>
{
	friend class GMoveableSystem;
public:
	typedef enum MoveableState
	{
		STATE_STOP = BIT(1),
		STATE_MOVE_DX = BIT(2),
		STATE_MOVE_DY = BIT(3),
		STATE_MOVE_DX_REVERT = BIT(4),
		STATE_MOVE_DY_REVERT = BIT(5)
	}MoveableState;

	GMoveableComponent(float dx, float dy, GVector2 leftTopPoint, GVector2 rightBottomPoint) :
		mDX(dx), mDY(dy), mLeftTopPoint(leftTopPoint), mRightBottomPoint(rightBottomPoint), mState(STATE_STOP)
	{	};
	virtual ~GMoveableComponent() 
	{
		signal_LocationChanged.disconnect_all_slots();
		signal_Moved.disconnect_all_slots();
	};

	float GetDX() { return mDX; }
	void  SetDX(float x) { mDX = x; }
	float GetDY() { return mDY; }
	void  SetDY(float y) { mDY = y; }

	bool isInsideRectangle(GVector2 point)
	{
		if (point.x < mLeftTopPoint.x || point.y < mLeftTopPoint.y ||
			point.x >= mRightBottomPoint.x || point.y >= mRightBottomPoint.y)
		{
			return false;
		}
		return true;
	}

private:
	void setState(MoveableState state) { mState = state; }

public:/*slots*/
	const boost::signals2::signal<void()>::slot_type slot_MoveDxRevert = boost::bind(&GMoveableComponent::setState, this, STATE_MOVE_DX_REVERT); 
	const boost::signals2::signal<void()>::slot_type slot_MoveDyRevert = boost::bind(&GMoveableComponent::setState, this, STATE_MOVE_DY_REVERT);	
	const boost::signals2::signal<void()>::slot_type slot_MoveDx	      = boost::bind(&GMoveableComponent::setState, this, STATE_MOVE_DX);	
	const boost::signals2::signal<void()>::slot_type slot_MoveDy	      = boost::bind(&GMoveableComponent::setState, this, STATE_MOVE_DY);	
	const boost::signals2::signal<void()>::slot_type slot_Stop         = boost::bind(&GMoveableComponent::setState, this, STATE_STOP);	

public: /*signals*/
	boost::signals2::signal<void(float, float)>     signal_LocationChanged;
	boost::signals2::signal<void()>                 signal_Moved;

private: 
	float mDX;
	float mDY;

	GVector2 mLeftTopPoint;
	GVector2 mRightBottomPoint;

	MoveableState mState;
};

#endif //GMOVEABLECOMPONENT_H

