#ifndef GMOVEABLE_SYSTEM_H
#define GMOVEABLE_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gLocationComponent.h"
#include "gMoveableComponent.h"

class GMoveableSystem : public GSystem<GMoveableSystem>
{
public:
	GMoveableSystem(std::shared_ptr<GEntityManager> manager);
	~GMoveableSystem();

	virtual void update(int dt);
};

#endif //GMOVEABLE_SYSTEM_H

