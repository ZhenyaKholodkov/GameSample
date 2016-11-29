#ifndef GRENDERABLE_COMPONENT_H
#define GRENDERABLE_COMPONENT_H

#include "gComponent.h"
#include "gSprite.h"

class GRenderableComponent : public GComponent<GRenderableComponent>, public sigslot::has_slots<>
{
public:
	GRenderableComponent() : mSprite(nullptr), mCurrentXScale(0.0f), mCurrentYScale(0.0f), mIsVisible(true){};
	GRenderableComponent(GSprite* sprite, float currentScalem = 1.0f, float currentScale = 1.0f) :
		mSprite(sprite), mCurrentXScale(currentScalem), mCurrentYScale(currentScale), mIsVisible(true) {};

	virtual ~GRenderableComponent() {};

	GSprite* GetSprite() { return mSprite;	}
	void SetSprite(GSprite* sprite) { mSprite = sprite; }

	float getXScale() { return mCurrentXScale; }
	void  setXScale(float xScale) { mCurrentXScale = xScale; }
	float getYScale() { return mCurrentYScale; }
	void  setYScale(float yScale) { mCurrentYScale = yScale; }

	bool isVisible() { return mIsVisible; }
	void setVisible(bool isVisible) { signal_VisibilityChanged(isVisible); mIsVisible = isVisible; }

	bool IsPiontInsideWH(GCursor localPoint)
	{
		if (!mSprite || ((localPoint.x < -mSprite->GetPivotX()) || (localPoint.y < -mSprite->GetPivotY()) ||
			(localPoint.x >= mSprite->GetWidth()) || 
			(localPoint.y >= mSprite->GetHeight())))
		{
			return false;
		}
		return true;
	}

public:/*slots*/
	void slot_ChangeSprite(Entity entity, GSprite* newSprite) { mSprite = newSprite; };
	void slot_ChangeScale(float xScale, float yScale) 
	{
		signal_ScaleChanged(xScale, yScale);
		mCurrentXScale = xScale; mCurrentYScale = yScale;
	}
	void slot_VisibilityChanged(bool isVisible)
	{
		setVisible(isVisible);
	}

	void slot_SetInvisible(Entity who)
	{
		setVisible(false);
	}

	void slot_SetVisible(Entity who)
	{
		setVisible(true);
	}

	void slot_ChangeSprite(Entity entity, void* data) 
	{
		GSprite* newSprite = static_cast<GSprite*>(data);
		mSprite = newSprite; 
	};

public:/*signals*/
	sigslot::signal2<float, float> signal_ScaleChanged;
	sigslot::signal1<bool>         signal_VisibilityChanged;

private:
	GSprite* mSprite;

	float mCurrentXScale;
	float mCurrentYScale;

	bool mIsVisible;
};

#endif //GRENDERABLE_COMPONENT_H

