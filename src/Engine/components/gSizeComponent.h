#ifndef GSIZECOMPONENT_H
#define GSIZECOMPONENT_H

#include "gComponent.h"

class GSizeComponent : public GComponent<GSizeComponent>
{
public:
	GSizeComponent() : mWidth(0.0f), mHeight(0.0f) {};
	GSizeComponent(float width, float height) : mWidth(width), mHeight(height) {};
	virtual ~GSizeComponent() {};

	void setWidth(float width) { mWidth = width; }
	void setHeight(float height) { mHeight = height; }
	void setWH(float width, float height) { mWidth = width; mHeight = height; }

	float getWidth() { return mWidth; }
	float getHeight() { return mHeight; }

private: 
	float mWidth;
	float mHeight;
};

#endif //GSIZECOMPONENT_H

