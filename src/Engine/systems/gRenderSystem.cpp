#include "GRenderSystem.h"


GRenderSystem::GRenderSystem() :
	mBackgroundColor(0xff0000ff)
{
	mRenderer = IGRender::Instance();
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

		GLocationComponent* location = mEntityManager->GetComponent<GLocationComponent>(entity);
		GSprite* sprite = renderable->GetSprite();

		mRenderer->save();
		mRenderer->translate(location->getX(), location->getY());

		mRenderer->drawImage(sprite->mTextureHandle.getTextureGLId(), sprite->mTextureHandle.getTextureWidth(), 
			sprite->mTextureHandle.getTextureHeight(), sprite->GetXPos(), sprite->GetYPos(), sprite->GetWidth(),
			sprite->GetHeight(), -sprite->GetPivotX(), -sprite->GetPivotY(), 0.0f);

		mRenderer->restore();
	}

	mRenderer->endFrame();
}