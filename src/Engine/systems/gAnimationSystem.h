#ifndef GANIMATION_SYSTEM_H
#define GANIMATION_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "Utils.h"

class GAnimationSystem : public GSystem<GAnimationSystem>
{
public:
	GAnimationSystem();
	virtual ~GAnimationSystem();

	virtual void update(int dt);
private:
	void ProcessActions(Entity entity);

private:
	GEntityManager* mEntityManager;
};

#endif //GANIMATION_SYSTEM_H

