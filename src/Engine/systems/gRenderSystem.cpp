#include "GRenderSystem.h"


GRenderSystem::GRenderSystem() :
	mBackgroundColor(0x000000)
{
	mEntityManager = GEntityManager::instance();
	mRenderer = GRenderManager::Instance();
}

GRenderSystem::~GRenderSystem() 
{
}

void GRenderSystem::update(int dt)
{
	mRenderer->setClearColor(mBackgroundColor);
	mRenderer->startFrame();

	for(auto pair : *mEntityManager->getComponentPool<GRenderableComponent>())
	{
		Entity entity = pair->first;
		GRenderableComponent* renderable = pair->second;
		if (!renderable->isVisible())
			continue;

		GLocationComponent* location = mEntityManager->getComponent<GLocationComponent>(entity);
		GSprite* sprite = renderable->GetSprite();
		if (!sprite)
			continue;

		mRenderer->save();
		mRenderer->translate(location->getX(), location->getY());
		mRenderer->scale(renderable->getXScale(), renderable->getYScale());

		mRenderer->drawSprite(sprite);

		mRenderer->restore();
	}

	mRenderer->endFrame();
}