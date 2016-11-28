#include "GRenderSystem.h"


GRenderSystem::GRenderSystem() :
	mBackgroundColor(0x000000)
{
	mRenderer = GRenderManager::Instance();
	mEntityManager = GEntityManager::Instance();
}

GRenderSystem::~GRenderSystem() 
{
}


void GRenderSystem::update(int dt)
{
	mRenderer->setClearColor(mBackgroundColor);
	mRenderer->clear();
	mRenderer->startFrame();

	for (auto iter = mEntityManager->GetBeginPairComponent<GRenderableComponent>(); iter != mEntityManager->GetEndPairComponent<GRenderableComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		GRenderableComponent* renderable = (*iter)->second;
		if (!renderable->isVisible())
			continue;

		GLocationComponent* location = mEntityManager->GetComponent<GLocationComponent>(entity);
		GSprite* sprite = renderable->GetSprite();
		if (!sprite)
			continue;

		mRenderer->save();
		mRenderer->translate(location->getX(), location->getY(), location->getZ());
		mRenderer->scale(renderable->getXScale(), renderable->getYScale());

		mRenderer->drawImage(sprite->mTextureHandle.getTextureGLId(), sprite->mTextureHandle.getTextureWidth(), 
			sprite->mTextureHandle.getTextureHeight(), sprite->GetXPos(), sprite->GetYPos(), sprite->GetWidth(),
			sprite->GetHeight(), -sprite->GetPivotX(), -sprite->GetPivotY(), 0.0f);

		mRenderer->restore();
	}

	mRenderer->endFrame();
}