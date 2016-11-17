#ifndef GLOCATIONCOMPONENT_H
#define GLOCATIONCOMPONENT_H

#include "gComponent.h"

class GLocationComponent : public GComponent<GLocationComponent>
{
public:
	GLocationComponent() :  mX(0.0f), mY(0.0f) {};
	GLocationComponent(float x, float y) : mX(x), mY(y) {};
	virtual ~GLocationComponent() {};

	void setX(float x) { mX = x; }
	void setY(float y) { mY = y; }
	void setXY(float x, float y) { mX = x; mY = y; }

	float getX() { return mX; }
	float getY() { return mY; }

	void LocalPoint(GPoint& point, GPoint& localPoint)
	{
		localPoint.x = ((point.x - mX) + (point.y - mY));
		localPoint.y = ((point.x - mX) + (point.y - mY));
	}

private: 
	float mX;
	float mY;
};

#endif //GLOCATIONCOMPONENT_H

