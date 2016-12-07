#ifndef GMOVEABLECOMPONENT_H
#define GMOVEABLECOMPONENT_H

#include "gComponent.h"

class GMoveableComponent : public GComponent<GMoveableComponent>, public sigslot::has_slots<>
{
	friend class GMoveableSystem;
public:
	GMoveableComponent(float dx, float dy) :
		mDX(dx), mDY(dy), mState(STATE_STOP)
	{	};
	virtual ~GMoveableComponent() {};

	float GetDX() { return mDX; }
	void  SetDX(float x) { mDX = x; }
	float GetDY() { return mDY; }
	void  SetDY(float y) { mDY = y; }


public:/*slots*/
	void slot_MoveDxRevert()
	{
		mState = STATE_MOVE_DX_REVERT;
	}
	void slot_MoveDyRevert()
	{
		mState = STATE_MOVE_DY_REVERT;
	}
	void slot_MoveDx()
	{
		mState = STATE_MOVE_DX;
	}
	void slot_MoveDy()
	{
		mState = STATE_MOVE_DY;
	}
	void slot_Stop()
	{
		mState = STATE_STOP;
	}

public: /*signals*/
	sigslot::signal2<float, float>     signal_LocationChanged;
	sigslot::signal1<Entity>           signal_Moved;

private: 
	typedef enum MoveableState
	{
		STATE_STOP = BIT(1),
		STATE_MOVE_DX = BIT(2),
		STATE_MOVE_DY = BIT(3),
		STATE_MOVE_DX_REVERT = BIT(4),
		STATE_MOVE_DY_REVERT = BIT(5)
	}MoveableState;

	float mDX;
	float mDY;

	MoveableState mState;
};

#endif //GMOVEABLECOMPONENT_H

