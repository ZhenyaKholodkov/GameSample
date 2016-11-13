#ifndef GBASE_SYSTEM_H
#define GBASE_SYSTEM_H

#include "Utils.h"

class GBaseSystem
{
public:
	GBaseSystem();
	virtual ~GBaseSystem();

	virtual void update(int dt) = 0;
protected:
	static uint32 s_system_counter;
};

#endif //GBASE_SYSTEM_H

