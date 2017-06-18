#ifndef GCOLLISONCOMPONENT_H
#define GCOLLISONCOMPONENT_H

#include "gComponent.h"

class GCollisionComponent : public GComponent<GCollisionComponent>
{
	friend class GCollisionSystem;
public:
	GCollisionComponent(int xRadius, int yRadius, bool destroyAfterCollision = false)
	{
		mRadius.x = (float)xRadius;
		mRadius.y = (float)yRadius;
		mNeedCheck = false;
		mDestroyAfterCollision = destroyAfterCollision;
	};
	virtual ~GCollisionComponent() 
	{
		signal_Collisioned.disconnect_all_slots();
		signal_CollisionedWith.disconnect_all_slots();
		signal_CollisionedWho.disconnect_all_slots();
	};

private:
	void checkCollision() { mNeedCheck = true; }
public: /*slots*/
	const boost::signals2::signal<void()>::slot_type slot_CheckCollision = boost::bind(&::GCollisionComponent::checkCollision, this);
public:/*signals*/
	boost::signals2::signal<void()> signal_Collisioned;
	boost::signals2::signal<void(Entity with)> signal_CollisionedWith;
	boost::signals2::signal<void(Entity who)> signal_CollisionedWho;

private: 
	GVector2 mRadius;
	bool     mNeedCheck;
	bool     mDestroyAfterCollision;

	std::list<uint32> mCellIndexes;
};

#endif //GCOLLISONCOMPONENT_H

