#ifndef GCOLLISION_SYSTEM_H
#define GCOLLISION_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "Utils.h"

class GCollisionSystem : public GSystem<GCollisionSystem>, public sigslot::has_slots<>
{
public:
	GCollisionSystem();
	virtual ~GCollisionSystem();

	virtual void update(int dt);

public:/*slots*/
	void slot_CheckCollision(Entity checkEntity)
	{
		checkCollision(checkEntity);
	}

private:
	void checkCollision(Entity checkEntity);
private:
	GEntityManager* mEntityManager;
};

#endif //GCOLLISION_SYSTEM_H

