#ifndef GCHILDCOMPONENT_H
#define GCHILDCOMPONENT_H

#include "gComponent.h"

class GChildComponent : public GComponent<GChildComponent>
{
public:
	GChildComponent() {};
	virtual ~GChildComponent() {};

	void addChild(Entity entity) { mChilds.push_back(entity); }
	void getChilds(std::vector<Entity> &vector) { vector = mChilds;/* std::copy(mChilds.begin(), mChilds.end(), vector.begin());*/ }
	void removeChild(Entity entity) { mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), entity), mChilds.end()); }

private: 
	std::vector<Entity> mChilds;
};

#endif //GLOCATIONCOMPONENT_H

