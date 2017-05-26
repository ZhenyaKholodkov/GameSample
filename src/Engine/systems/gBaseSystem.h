#ifndef GBASE_SYSTEM_H
#define GBASE_SYSTEM_H

#include <memory>

#include "Utils.h"
#include "GEntityManager.h"

class GBaseSystem
{
	friend class GSystemManager;
public:
	GBaseSystem(std::shared_ptr<GEntityManager> manager);
	virtual ~GBaseSystem();

	virtual void update(int dt) = 0;
protected:
	static uint32 s_system_counter;

	std::shared_ptr<GEntityManager> mEntityManager;
};

#endif //GBASE_SYSTEM_H

