#ifndef GMOVEABLEANIMATIONSYSTEM_H
#define GMOVEABLEANIMATIONSYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gLocationComponent.h"
#include "gMoveableAnimationComponent.h"

class GMoveableAnimationSystem : public GSystem<GMoveableAnimationSystem>
{
public:
	GMoveableAnimationSystem();
	~GMoveableAnimationSystem();

	virtual void update(int dt);
private:	
	GEntityManager* mEntityManager;
};

#endif //GMOVEABLEANIMATIONSYSTEM_H

