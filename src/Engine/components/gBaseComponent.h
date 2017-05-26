#ifndef GBASECOMPONENT_H
#define GBASECOMPONENT_H

#include "Types.h"
#include "sigslot.h"

class GBaseComponent
{
	friend class GEntityManager;
public:
	GBaseComponent() {};
	virtual ~GBaseComponent() {};
	static uint32 s_invalid_component_index() {	return s_component_counter + 1;	}
	static const uint32 s_get_component_count() {	return s_component_counter; }
protected:
	static uint32 s_component_counter;
};

#endif //GBASECOMPONENT_H

