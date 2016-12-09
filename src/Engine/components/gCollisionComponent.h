#ifndef GCOLLISONCOMPONENT_H
#define GCOLLISONCOMPONENT_H

#include "gComponent.h"

class GCollisionComponent : public GComponent<GCollisionComponent>
{
	friend class GCollisionSystem;
public:
	GCollisionComponent(int xRadius, int yRadius)
	{
		mRadius.x = (float)xRadius;
		mRadius.y = (float)yRadius;
	};
	virtual ~GCollisionComponent() 
	{
		signal_Collisioned.disconnect_all();
	};
	
public:/*signals*/
	sigslot::signal0<> signal_Collisioned;

private: 
	GVector2 mRadius;

	std::list<uint32> mCellIndexes;
};

#endif //GCOLLISONCOMPONENT_H

