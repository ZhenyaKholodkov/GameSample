#ifndef GMOVEABLE_SYSTEM_H
#define GMOVEABLE_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "GMoveableSystem.h"
#include "gLocationComponent.h"
#include "gMoveableComponent.h"

class GMoveableSystem : public GSystem<GMoveableSystem>
{
public:
	GMoveableSystem();
	~GMoveableSystem();

	virtual void update(int dt);
private:	
	GEntityManager* mEntityManager;
};

#endif //GMOVEABLE_SYSTEM_H
