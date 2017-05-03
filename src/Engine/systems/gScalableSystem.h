#ifndef GSCALABLE_SYSTEM_H
#define GSCALABLE_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gScalableComponent.h"

class GScalableSystem : public GSystem<GScalableSystem>
{
public:
	GScalableSystem();
	~GScalableSystem();

	virtual void update(int dt);
private:	
	const GEntityManager* mEntityManager;
};

#endif //GSCALABLE_SYSTEM_H

