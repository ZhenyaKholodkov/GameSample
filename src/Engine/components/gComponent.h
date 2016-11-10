#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "gBaseComponent.h"

typedef enum ComponentType
{
	LOCATION_COMPONENT_TYPE   = 1,
	RENDERABLE_COMPONENT_TYPE = 2
};

template<typename Component> 
class GComponent : GBaseComponent
{
public:
	GComponent();
	virtual ~GComponent();

	static uint32 getComponentIndex();
};

#endif //GCOMPONENT_H

