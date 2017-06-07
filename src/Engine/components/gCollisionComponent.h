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
		mNeedCheck = false;
	};
	virtual ~GCollisionComponent() 
	{
		signal_Collisioned.disconnect_all_slots();
	};

private:
	void checkCollision() { mNeedCheck = true; }
public: /*slots*/
	const boost::signals2::signal<void()>::slot_type slot_CheckCollision = boost::bind(&::GCollisionComponent::checkCollision, this);
public:/*signals*/
	boost::signals2::signal<void()> signal_Collisioned;

private: 
	GVector2 mRadius;
	bool     mNeedCheck;

	std::list<uint32> mCellIndexes;
};

#endif //GCOLLISONCOMPONENT_H

