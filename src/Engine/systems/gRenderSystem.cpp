#include "GRenderSystem.h"


GRenderSystem::GRenderSystem()
{
	mRenderer = IGRender::Instance();
	mEntityManager = GEntityManager::Instance();
}

GRenderSystem::~GRenderSystem() 
{
}


void GRenderSystem::update(int dt)
{
	for (auto iter = mEntityManager->GetActiveEntitiesBegin(); iter != mEntityManager->GetActiveEntitiesEnd(); iter++)
	{
		Entity entity = (*iter);
		GLocationComponent* location = mEntityManager->GetComponent<GLocationComponent>(entity);
		GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(entity);
		GSprite* sprite = renderable->getSprite();

		IGRender::Instance()->drawImage(sprite->mTextureHandle.getTextureGLId(), sprite->mTextureHandle.getTextureWidth(), sprite->mTextureHandle.getTextureHeight(),
			sprite->GetXPos(), sprite->GetYPos(), sprite->GetWidth(), sprite->GetHeight(), 0.0f, 0.0f, 0.0f);
	//	mRenderer->drawImage(sprite->mTextureHandle.getTextureGLId(), sprite->mTextureHandle.getTextureWidth(), sprite->mTextureHandle.getTextureHeight(),
	//		sprite->GetXPos(), sprite->GetYPos(), sprite->GetWidth(), sprite->GetHeight(), location->getX(), location->getY(), 0.0f, 0.0f, 0.0f, 0.0f);
	}
}