#ifndef GLOCATIONCOMPONENT_H
#define GLOCATIONCOMPONENT_H

#include "gComponent.h"

class GLocationComponent : public GComponent<GLocationComponent>, public sigslot::has_slots<>
{
public:
	GLocationComponent() :  mX(0.0f), mY(0.0f), mZ(0.0f), mDefaultX(0.0f), mDefaultY(0.0f) {};
	GLocationComponent(float x, float y) : mX(x), mY(y), mZ(0.0f), mDefaultX(x), mDefaultY(y){};
	virtual ~GLocationComponent() {};

	void setX(float x) { mX = x; }
	void setY(float y) { mY = y; }
	void setZ(float z) { mZ = z; }
	void setXY(float x, float y) { mX = x; mY = y; }

	float getX() { return mX; }
	float getY() { return mY; }
	float getZ() { return mZ; }

	float getDefaultX() { return mDefaultX; }
	float getDefaultY() { return mDefaultY; }
	void setDefaultXY(float x, float y) { mDefaultX = x; mDefaultY = y; }

	void restoreLocation()
	{
		signal_LocationChangedWithDxDy(mDefaultX - mX, mDefaultY - mY);
		setXY(mDefaultX, mDefaultY);
	}

public:/*slots*/
	void slot_LocationChanged(float x, float y)
	{
		signal_LocationChangedWithDxDy(x - mX, y - mY);
		setXY(x, y);
	}
	void slot_LocationChangedWithDxDy(float dx, float dy)
	{
		signal_LocationChangedWithDxDy(dx,dy);
		setXY(mX + dx, mY + dy);
	}

public:/*signals*/
	sigslot::signal2<float, float> signal_LocationChangedWithDxDy;

private: 
	float mX;
	float mY;
	float mZ;

	float mDefaultX;
	float mDefaultY;
};

#endif //GLOCATIONCOMPONENT_H

