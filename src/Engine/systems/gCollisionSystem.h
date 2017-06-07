#ifndef GCOLLISION_SYSTEM_H
#define GCOLLISION_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "Utils.h"

class GCollisionSystem : public GSystem<GCollisionSystem>
{
public:
	GCollisionSystem(std::shared_ptr<GEntityManager> manager);
	virtual ~GCollisionSystem();

	virtual void update(int dt);
private:
	void checkCollision(Entity checkEntity);
};

#endif //GCOLLISION_SYSTEM_H

