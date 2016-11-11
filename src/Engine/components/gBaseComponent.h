#ifndef GBASECOMPONENT_H
#define GBASECOMPONENT_H

#include "Types.h"

class GBaseComponent
{
	friend class GEntityManager;
public:
	GBaseComponent() {};
	virtual ~GBaseComponent() {};
protected:
	static uint32 s_component_counter;
};

#endif //GBASECOMPONENT_H

