#ifndef GLOCATIONCOMPONENT_H
#define GLOCATIONCOMPONENT_H

#include "gComponent.h"

class GLocationComponent : public GComponent<GLocationComponent>
{
public:
	GLocationComponent() : GComponent(), mPosition(0.0f, 0.0f), mDefaultX(0.0f), mDefaultY(0.0f) {};
	GLocationComponent(float x, float y) : mPosition(x, y), mDefaultX(x), mDefaultY(y), mLastX(x), mLastY(y)
	{
		mDefaultX = 10;
		bool d = true;
	};
	virtual ~GLocationComponent()
	{
		signal_LocationChangedWithDxDy.disconnect_all_slots();
	};

	void setX(float x) { mLastX = mPosition.x; mPosition.x = x; }
	void setY(float y) { mLastY = mPosition.y; mPosition.y = y; }
	void setXY(float x, float y) { mLastX = mPosition.x; mLastY = mPosition.y; mPosition.x = x; mPosition.y = y; }

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
private:
	void locationChanged(float x, float y)
	{
		signal_LocationChangedWithDxDy(x - mPosition.x, y - mPosition.y);
		setXY(x, y);
	}
	void locationChangedWithDxDy(float dx, float dy)
	{
		signal_LocationChangedWithDxDy(dx, dy);
		setXY(mPosition.x + dx, mPosition.y + dy);
	}
	void locationRestoreToLast()
	{
		signal_LocationChangedWithDxDy(mPosition.x - mLastX, mPosition.y - mLastY);
		setXY(mLastX, mLastY);
	}
public:/*slots*/
	const boost::signals2::signal<void(float, float)>::slot_type slot_LocationChanged         = boost::bind(&GLocationComponent::locationChanged, this, _1, _2);
	const boost::signals2::signal<void(float, float)>::slot_type slot_LocationChangedWithDxDy = boost::bind(&GLocationComponent::locationChangedWithDxDy, this, _1, _2);
	const boost::signals2::signal<void()>::slot_type             slot_LocationRestoreToLast   = boost::bind(&GLocationComponent::locationRestoreToLast, this);
public:/*signals*/
	boost::signals2::signal<void(float, float)> signal_LocationChangedWithDxDy;

private:
	GVector2 mPosition;

	float mDefaultX;
	float mDefaultY;

	float mLastX;
	float mLastY;
};
//
//class GLocationComponent : public GComponent<GLocationComponent>, public sigslot::has_slots<>
//{
//public:
//	GLocationComponent() : GComponent(), mPosition(0.0f, 0.0f), mDefaultX(0.0f), mDefaultY(0.0f) {};
//	GLocationComponent(float x, float y) : mPosition(x, y), mDefaultX(x), mDefaultY(y), mLastX(x), mLastY(y)
//	{
//		mDefaultX = 10;
//		bool d = true;
//	};
//	virtual ~GLocationComponent() 
//	{
//		signal_LocationChangedWithDxDy.disconnect_all();
//	};
//
//	void setX(float x) { mLastX = mPosition.x; mPosition.x = x; }
//	void setY(float y) { mLastY = mPosition.y; mPosition.y = y;  }
//	void setXY(float x, float y) { mLastX = mPosition.x; mLastY = mPosition.y; mPosition.x = x; mPosition.y = y; 	}
//
//	float getX() { return mPosition.x; }
//	float getY() { return mPosition.y; }
//
//	float getDefaultX() { return mDefaultX; }
//	float getDefaultY() { return mDefaultY; }
//	void setDefaultXY(float x, float y) { mDefaultX = x; mDefaultY = y; }
//
//	void restoreLocation()
//	{
//		signal_LocationChangedWithDxDy(mDefaultX - mPosition.x, mDefaultY - mPosition.y);
//		setXY(mDefaultX, mDefaultY);
//	}
//
//	void locationChanged(float x, float y)
//	{
//		signal_LocationChangedWithDxDy(x - mPosition.x, y - mPosition.y);
//		setXY(x, y);
//	}
//	void locationChangedWithDxDy(float dx, float dy)
//	{
//		signal_LocationChangedWithDxDy(dx, dy);
//		setXY(mPosition.x + dx, mPosition.y + dy);
//	}
//	void locationRestoreToLast()
//	{
//		signal_LocationChangedWithDxDy(mPosition.x - mLastX, mPosition.y - mLastY);
//		setXY(mLastX, mLastY);
//	}
//public:/*slots*/
//	boost::function<void(float, float)> slot_LocationChanged()         const { return boost::bind(&GLocationComponent::locationChanged, this, _1, _2); }
//	boost::function<void(float, float)> slot_LocationChangedWithDxDy() const { return boost::bind(&GLocationComponent::locationChangedWithDxDy, this, _1, _2); }
//	boost::function<void()>             slot_LocationRestoreToLast()   const { return boost::bind(&GLocationComponent::locationRestoreToLast, this); }
//public:/*signals*/
//	boost::signals2::signal<void(float, float)> signal_LocationChangedWithDxDy;
//
//private: 
//	GVector2 mPosition;
//
//	float mDefaultX;
//	float mDefaultY;
//
//	float mLastX;
//	float mLastY;
//};

#endif //GLOCATIONCOMPONENT_H

