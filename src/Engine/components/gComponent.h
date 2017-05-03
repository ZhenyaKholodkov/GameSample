#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "gBaseComponent.h"

template<typename Component> 
class GComponent : public GBaseComponent
{
public:
	GComponent() : GBaseComponent() { };
	virtual ~GComponent() {};

	static const uint32 getComponentId();
private:
	static const uint32 s_component_id; // identifier of the component class.
};

template<typename C>
const uint32 GComponent<C>::s_component_id = GBaseComponent::s_component_counter++;

template<typename C>
const uint32 GComponent<C>::getComponentId()
{
	return s_component_id;
}
#endif //GCOMPONENT_H

