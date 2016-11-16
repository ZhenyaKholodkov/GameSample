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
	static uint32 getSystemId();
private:
	static uint32 s_system_id;
};

template<typename C>
uint32 GSystem<C>::s_system_id = GBaseSystem::s_system_counter++;

template<typename C>
uint32 GSystem<C>::getSystemId()
{
	return s_system_id;
}

#endif //GBASE_SYSTEM_H

