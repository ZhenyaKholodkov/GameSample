#ifndef GRENDERABLE_COMPONENT_H
#define GRENDERABLE_COMPONENT_H

#include "gComponent.h"
#include "gSprite.h"

class GRenderableComponent : GComponent
{
public:
	GRenderableComponent();
	~GRenderableComponent();

private:
	GSprite* mSprite;
};

#endif //GRENDERABLE_COMPONENT_H

