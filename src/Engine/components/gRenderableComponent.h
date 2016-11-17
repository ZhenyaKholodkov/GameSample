#ifndef GRENDERABLE_COMPONENT_H
#define GRENDERABLE_COMPONENT_H

#include "gComponent.h"
#include "gSprite.h"

class GRenderableComponent : public GComponent<GRenderableComponent>
{
public:
	GRenderableComponent() : mSprite(nullptr) {};
	GRenderableComponent(GSprite* sprite) : mSprite(sprite) {};
	virtual ~GRenderableComponent() {};

	GSprite* GetSprite() { return mSprite;	}
	void SetSprite(GSprite* sprite) { mSprite = sprite; }

	bool IsPiontInsideWH(GPoint localPoint)
	{
		if ((localPoint.x < -mSprite->GetPivotX()) || (localPoint.y < -mSprite->GetPivotY())
			|| (localPoint.x >= mSprite->GetWidth()) || (localPoint.y >= mSprite->GetHeight()))
		{
			return false;
		}
		return true;
	}

private:
	GSprite* mSprite;
};

#endif //GRENDERABLE_COMPONENT_H

