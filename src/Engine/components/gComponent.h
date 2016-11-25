#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "gBaseComponent.h"

template<typename Component> 
class GComponent : public GBaseComponent
{
public:
	GComponent() {};
	virtual ~GComponent() {};

	virtual void Reset() {};

	static uint32 GetComponentId();
private:
	static uint32 s_component_id;
};

template<typename C>
uint32 GComponent<C>::s_component_id = GBaseComponent::s_component_counter++;

template<typename C>
uint32 GComponent<C>::GetComponentId()
{
//	static uint32 index = s_component_counter++;
	return s_component_id;
}
#endif //GCOMPONENT_H

