#ifndef GCOLLISONCOMPONENT_H
#define GCOLLISONCOMPONENT_H

#include "gComponent.h"

class GCollisionComponent : public GComponent<GCollisionComponent>
{
	friend class GCollisionSystem;
public:
	GCollisionComponent(uint32 xRadius, uint32 yRadius)
	{
		mRadius.x = xRadius;
		mRadius.y = yRadius;
	};
	virtual ~GCollisionComponent() {};
	
public:/*signals*/
	sigslot::signal0<> signal_Collisioned;

private: 
	GVector2 mRadius;

	std::list<uint32> mCellIndexes;
};

#endif //GCOLLISONCOMPONENT_H

