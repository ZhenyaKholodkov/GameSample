#ifndef GCHILDCOMPONENT_H
#define GCHILDCOMPONENT_H

#include "gComponent.h"

class GChildComponent : public GComponent<GChildComponent>
{
public:
	GChildComponent() {};
	virtual ~GChildComponent() {};

	void setChild(Entity entity) { mChild = entity; }
	Entity getChild() { return mChild; }

private: 
	Entity mChild;
};

#endif //GLOCATIONCOMPONENT_H

