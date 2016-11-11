#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "gBaseComponent.h"

typedef enum ComponentType
{
	LOCATION_COMPONENT_TYPE   = 1,
	RENDERABLE_COMPONENT_TYPE = 2
};

template<typename Component> 
class GComponent : public GBaseComponent
{
public:
	GComponent() {};
	virtual ~GComponent() {};

	static uint32 getComponentIndex();
};


template<typename C>
uint32 GComponent<C>::getComponentIndex()
{
	static uint32 index = s_component_counter++;
	return index;
}
#endif //GCOMPONENT_H

