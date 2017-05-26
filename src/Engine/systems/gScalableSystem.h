#ifndef GSCALABLE_SYSTEM_H
#define GSCALABLE_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gScalableComponent.h"

class GScalableSystem : public GSystem<GScalableSystem>
{
public:
	GScalableSystem(std::shared_ptr<GEntityManager> manager);
	~GScalableSystem();

	virtual void update(int dt);
};

#endif //GSCALABLE_SYSTEM_H

