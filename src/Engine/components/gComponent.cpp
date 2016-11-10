#include "gComponent.h"


template<typename C>
GComponent<C>::GComponent() {}

template<typename C>
GComponent<C>::~GComponent() {}

template<typename C>
uint32 GComponent<C>::getComponentIndex()
{
	static uint32 index = s_component_counter++;
	return index;
}