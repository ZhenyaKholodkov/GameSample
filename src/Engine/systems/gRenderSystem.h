#ifndef GRENDER_SYSTEM_H
#define GRENDER_SYSTEM_H

#include "gBaseSystem.h"
#include "IGRender.h"

class GRenderSystem : GBaseSystem
{
public:
	GRenderSystem();
	~GRenderSystem();

	virtual void update(int dt);
};

#endif //GRENDER_SYSTEM_H

