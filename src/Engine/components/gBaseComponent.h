#ifndef GBASECOMPONENT_H
#define GBASECOMPONENT_H

#include "Types.h"

class GBaseComponent
{
	friend class GEntityManager;
public:
	GBaseComponent() {};
	virtual ~GBaseComponent() {};
	virtual void SetState(uint32 state) = 0;
	virtual void Reset() = 0;
protected:
	static uint32 s_component_counter;
};

#endif //GBASECOMPONENT_H

