#ifndef GSYSTEM_H
#define GSYSTEM_H

#include "Utils.h"
#include "gBaseSystem.h"

template<class System>
class GSystem : public GBaseSystem
{
public:
	GSystem() {};
	virtual ~GSystem() {};

	virtual void update(int dt) = 0;
	static uint32 getSystemIndex();
};


template<typename C>
uint32 GSystem<C>::getSystemIndex()
{
	static uint32 index = s_system_counter++;
	return index;
}

#endif //GBASE_SYSTEM_H

