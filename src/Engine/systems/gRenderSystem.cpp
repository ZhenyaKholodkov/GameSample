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
	if (isStoped())
		return;
	mRenderer->setClearColor(mBackgroundColor);
	mRenderer->startFrame();
	//mRenderer->beginTextRendering();
	mEntityManager->each<GRenderableComponent, GLocationComponent>([this](Entity entity, GRenderableComponent& renderable, GLocationComponent& location)
	{
		if (!renderable.isVisible())
			return;

		GSprite* sprite = renderable.GetSprite();
		/**if (!sprite)
			return;*/

		mRenderer->save();

		if (mEntityManager->doesHaveComponent<GParentComponent>(entity))
			setParentParams(mEntityManager->getComponent<GParentComponent>(entity)->getParent());

		mRenderer->translate(location.getX(), location.getY());
		mRenderer->rotate(renderable.getAngle());
		mRenderer->scale(renderable.getXScale(), renderable.getYScale());
		if (sprite)
		{
			mRenderer->drawSprite(sprite);
		}

	    std:string text = renderable.getText();
		if (!text.empty())
		{
			mRenderer->drawText(text, renderable.getTextColor(), renderable.getFontSize());
		}
		mRenderer->restore();
	});

	//mRenderer->endTextRendering();
	mRenderer->endFrame();

}

void GRenderSystem::setParentParams(Entity parent)
{
	if (mEntityManager->doesHaveComponent<GParentComponent>(parent)) //if parent has a parent
	{
		auto parentComponent = mEntityManager->getComponent<GParentComponent>(parent);
		setParentParams(parentComponent->getParent());
	}

	if (mEntityManager->doesHaveComponent<GLocationComponent>(parent))
	{
		auto parentLocation = mEntityManager->getComponent<GLocationComponent>(parent);
		mRenderer->translate(parentLocation->getX(), parentLocation->getY());
	}

	if (mEntityManager->doesHaveComponent<GRenderableComponent>(parent))
	{
		auto parentRenderable = mEntityManager->getComponent<GRenderableComponent>(parent);
		mRenderer->rotate(parentRenderable->getAngle());
		mRenderer->scale(parentRenderable->getXScale(), parentRenderable->getYScale());
	}
}