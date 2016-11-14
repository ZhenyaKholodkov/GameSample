#ifndef GACTIONCOMPONENT_H
#define GACTIONCOMPONENT_H

#include "gComponent.h"

class GActionComponent : public GComponent<GActionComponent>
{
public:
	GActionComponent() {};
	virtual ~GActionComponent() {};

	void AddActions(int actionMask) { mActionMask |= actionMask; }
	void Reset() { mActionMask = 0; }

private: 
	int mActionMask;
};

#endif //GACTIONCOMPONENT_H

