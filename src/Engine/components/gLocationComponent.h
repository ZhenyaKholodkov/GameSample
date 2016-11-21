#ifndef GLOCATIONCOMPONENT_H
#define GLOCATIONCOMPONENT_H

#include "gComponent.h"

class GLocationComponent : public GComponent<GLocationComponent>
{
public:
	GLocationComponent() :  mX(0.0f), mY(0.0f), mDefaultX(0.0f), mDefaultY(0.0f) {};
	GLocationComponent(float x, float y) : mX(x), mY(y), mDefaultX(x), mDefaultY(y) {};
	virtual ~GLocationComponent() {};

	void setX(float x) { mX = x; }
	void setY(float y) { mY = y; }
	void setXY(float x, float y) { mX = x; mY = y; }

	float getX() { return mX; }
	float getY() { return mY; }

	float getDefaultX() { return mDefaultX; }
	float getDefaultY() { return mDefaultY; }

private: 
	float mX;
	float mY;

	float mDefaultX;
	float mDefaultY;
};

#endif //GLOCATIONCOMPONENT_H

