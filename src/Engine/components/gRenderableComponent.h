#ifndef GRENDERABLE_COMPONENT_H
#define GRENDERABLE_COMPONENT_H

#include "gComponent.h"
#include "gSprite.h"

class GRenderableComponent : GComponent<GRenderableComponent>
{
public:
	GRenderableComponent() : mSprite(nullptr) {};
	GRenderableComponent(GSprite* sprite) : mSprite(sprite) {};
	~GRenderableComponent() {};

	GSprite* getSprite() { return mSprite;	}
	void setSprite(GSprite* sprite) { mSprite = sprite; }

private:
	GSprite* mSprite;
};

#endif //GRENDERABLE_COMPONENT_H

