#ifndef GLOCATIONCOMPONENT_H
#define GLOCATIONCOMPONENT_H

#include "gComponent.h"

class GLocationComponent : public GComponent<GLocationComponent>, public sigslot::has_slots<>
{
public:
	GLocationComponent() :  mPosition(0.0f, 0.0f), mDefaultX(0.0f), mDefaultY(0.0f) {};
	GLocationComponent(float x, float y) : mPosition(x, y), mDefaultX(x), mDefaultY(y), mLastX(x), mLastY(y) {};
	virtual ~GLocationComponent() {};

	void setX(float x) { mLastX = mPosition.x; mPosition.x = x; }
	void setY(float y) { mLastY = mPosition.y; mPosition.y = y;  }
	void setXY(float x, float y) { mLastX = mPosition.x; mLastY = mPosition.y; mPosition.x = x; mPosition.y = y; 	}

	float getX() { return mPosition.x; }
	float getY() { return mPosition.y; }

	float getDefaultX() { return mDefaultX; }
	float getDefaultY() { return mDefaultY; }
	void setDefaultXY(float x, float y) { mDefaultX = x; mDefaultY = y; }

	void restoreLocation()
	{
		signal_LocationChangedWithDxDy(mDefaultX - mPosition.x, mDefaultY - mPosition.y);
		setXY(mDefaultX, mDefaultY);
	}

public:/*slots*/
	void slot_LocationChanged(float x, float y)
	{
		signal_LocationChangedWithDxDy(x - mPosition.x, y - mPosition.y);
		setXY(x, y);
	}
	void slot_LocationChangedWithDxDy(float dx, float dy)
	{
		signal_LocationChangedWithDxDy(dx,dy);
		setXY(mPosition.x + dx, mPosition.y + dy);
	}
	void slot_LocationRestoreToLast()
	{
		signal_LocationChangedWithDxDy(mPosition.x- mLastX,mPosition.y - mLastY);
		setXY(mLastX, mLastY);
	}

public:/*signals*/
	sigslot::signal2<float, float> signal_LocationChangedWithDxDy;

private: 
	GVector2 mPosition;

	float mDefaultX;
	float mDefaultY;

	float mLastX;
	float mLastY;
};

#endif //GLOCATIONCOMPONENT_H

