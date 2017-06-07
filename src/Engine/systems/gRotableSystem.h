#ifndef GROTABLE_SYSTEM_H
#define GROTABLE_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"

class GRotableSystem : public GSystem<GRotableSystem>
{
public:
	GRotableSystem(std::shared_ptr<GEntityManager> manager);
	~GRotableSystem();

	virtual void update(int dt);
};

#endif //GROTABLE_SYSTEM_H

