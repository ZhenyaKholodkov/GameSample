#include "GRenderSystem.h"


GRenderSystem::GRenderSystem(std::shared_ptr<GEntityManager> manager) :
	GSystem<GRenderSystem>(manager),
	mBackgroundColor(0x000000)
{
	mRenderer = GRenderManager::Instance();
}

GRenderSystem::~GRenderSystem() 
{
}

void GRenderSystem::update(int dt)
{
	mRenderer->setClearColor(mBackgroundColor);
	mRenderer->startFrame();

	mRenderer->save();
	mRenderer->scale(0.75f, 0.75f);

	mEntityManager->each<GRenderableComponent, GLocationComponent>([this](Entity entity, GRenderableComponent& renderable, GLocationComponent& location)
	{
		if (!renderable.isVisible())
			return;

		GSprite* sprite = renderable.GetSprite();
		if (!sprite)
			return;

		mRenderer->save();
		mRenderer->translate(location.getX() / 0.75f, location.getY() / 0.75f);
		mRenderer->rotate(renderable.getAngle());
		mRenderer->scale(renderable.getXScale(), renderable.getYScale());

		mRenderer->drawSprite(sprite);

		mRenderer->restore();
	});
	mRenderer->restore();

	mRenderer->endFrame();
}