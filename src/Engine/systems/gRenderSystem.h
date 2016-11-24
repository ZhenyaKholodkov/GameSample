#ifndef GRENDER_SYSTEM_H
#define GRENDER_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "gRenderableComponent.h"
#include "gLocationComponent.h"
#include "gRenderManager.h"

class GRenderSystem : public GSystem<GRenderSystem>
{
public:
	GRenderSystem();
	~GRenderSystem();

	virtual void update(int dt);
private:
	unsigned int mBackgroundColor;

	GRenderManager* mRenderer;
	GEntityManager* mEntityManager;
};

#endif //GRENDER_SYSTEM_H

