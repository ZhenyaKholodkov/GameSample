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
	GRenderSystem(std::shared_ptr<GEntityManager> manager);
	~GRenderSystem();

	virtual void update(int dt);
private:
	void setParentParams(Entity parent);
private:
	unsigned int mBackgroundColor;

	GRenderManager* mRenderer;
};

#endif //GRENDER_SYSTEM_H

