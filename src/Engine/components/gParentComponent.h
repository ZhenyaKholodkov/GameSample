#ifndef GPARENTCOMPONENT_H
#define GPARENTCOMPONENT_H

#include "gComponent.h"

class GParentComponent : public GComponent<GParentComponent>
{
public:
	GParentComponent() {};
	virtual ~GParentComponent() {};

	void setParent(Entity entity) { mParent = entity; }
	Entity getParent() { return mParent; }

private: 
	Entity mParent;
};

#endif //GLOCATIONCOMPONENT_H

