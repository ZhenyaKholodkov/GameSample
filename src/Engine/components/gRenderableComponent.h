#ifndef GRENDERABLE_COMPONENT_H
#define GRENDERABLE_COMPONENT_H

#include "gComponent.h"
#include "gSprite.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GRenderableComponent : public GComponent<GRenderableComponent>
{
public:
	GRenderableComponent() : mSprite(nullptr), mCurrentXScale(0.0f), mCurrentYScale(0.0f), mIsVisible(true){};
	GRenderableComponent(GSprite* sprite, float xScale = 1.0f, float yScale = 1.0f, float angle = 0.0f) :
		mSprite(sprite), mCurrentXScale(xScale), mCurrentYScale(yScale), mAngle(angle), mIsVisible(true)
	{
		mSprite = sprite;
	};

	virtual ~GRenderableComponent()
	{
		signal_ScaleChanged.~signal();
		signal_VisibilityChanged.~signal();
	};

	GSprite* GetSprite() const { return mSprite;	}
	void SetSprite(GSprite* sprite) { mSprite = sprite; }

	float getXScale() const { return mCurrentXScale; }
	void  setXScale(float xScale) { mCurrentXScale = xScale; }
	float getYScale() const { return mCurrentYScale; }
	void  setYScale(float yScale) { mCurrentYScale = yScale; }

	float getAngle() const { return mAngle; }
	void  setAngle(float angle) { mAngle = angle; }

	bool isVisible() const { return mIsVisible; }
	void setVisible(bool isVisible) { signal_VisibilityChanged(isVisible); mIsVisible = isVisible; }

	bool IsPiontInsideWH(GCursor localPoint) const
	{
		if (!mSprite || ((localPoint.x < -mSprite->getPivotX()) || (localPoint.y < -mSprite->getPivotY()) ||
			(localPoint.x >= mSprite->getWidth()) ||
			(localPoint.y >= mSprite->getHeight())))
		{
			return false;
		}
		return true;
	}

	void changeScale(float xScale, float yScale)
	{
		signal_ScaleChanged(xScale, yScale);
		mCurrentXScale = xScale; mCurrentYScale = yScale;
	}
public:/*slots*/
	 const boost::signals2::signal<void(GSprite*)>::slot_type         slot_ChangeSprite      = boost::bind(&GRenderableComponent::SetSprite, this, _1);
	 const boost::signals2::signal<void(float, float)>::slot_type     slot_ChangeScale       = boost::bind(&GRenderableComponent::changeScale, this, _1, _2);
	 const boost::signals2::signal<void(bool)>::slot_type             slot_VisibilityChanged = boost::bind(&GRenderableComponent::setVisible, this, _1);
	 const boost::signals2::signal<void()>::slot_type                 slot_SetInvisible      = boost::bind(&GRenderableComponent::setVisible, this, false);
	 const boost::signals2::signal<void()>::slot_type                 slot_SetVisible        = boost::bind(&GRenderableComponent::setVisible, this, true);

public:/*signals*/
	boost::signals2::signal<void(float, float)> signal_ScaleChanged;
	boost::signals2::signal<void(bool)>         signal_VisibilityChanged;

private:
	GSprite* mSprite;

	float mCurrentXScale;
	float mCurrentYScale;

	float mAngle;

	bool mIsVisible;
};

#endif //GRENDERABLE_COMPONENT_H

