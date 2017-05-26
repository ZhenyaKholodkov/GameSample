#ifndef GMOVEABLEANIMATIONSYSTEM_H
#define GMOVEABLEANIMATIONSYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gLocationComponent.h"
#include "gMoveableAnimationComponent.h"

class GMoveableAnimationSystem : public GSystem<GMoveableAnimationSystem>
{
public:
	GMoveableAnimationSystem(std::shared_ptr<GEntityManager> manager);
	~GMoveableAnimationSystem();

	virtual void update(int dt);
};

#endif //GMOVEABLEANIMATIONSYSTEM_H

