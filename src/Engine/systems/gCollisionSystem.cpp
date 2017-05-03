#include "GCollisionSystem.h"
#include "GCollisionComponent.h"

GCollisionSystem::GCollisionSystem()
{
	mEntityManager = GEntityManager::instance();
}

GCollisionSystem::~GCollisionSystem()
{
}

void GCollisionSystem::checkCollision(Entity checkEntity)
{
	if (!mEntityManager->doesHaveComponent<GCollisionComponent>(checkEntity))
		return;
	GCollisionComponent* checkCollision = mEntityManager->getComponent<GCollisionComponent>(checkEntity);
	GLocationComponent* checkLocation = mEntityManager->getComponent<GLocationComponent>(checkEntity);

	for (auto pair : mEntityManager->getComponentPool<GCollisionComponent>())
	{
		Entity entity = pair->first;
		if (checkEntity == entity)
			continue;

		if (mEntityManager->doesHaveComponent<GRenderableComponent>(entity))
		{
			GRenderableComponent*  renderable = mEntityManager->getComponent<GRenderableComponent>(entity);
			if (!renderable->isVisible())
				continue;
		}

		GCollisionComponent* collision = pair->second;
		GLocationComponent*  location = mEntityManager->getComponent<GLocationComponent>(entity);

		float dx = checkLocation->getX() - location->getX();
		float dy = checkLocation->getY() - location->getY();
		float distance = sqrt(dx * dx + dy * dy);

		if (abs(checkLocation->getX() - location->getX()) > (checkCollision->mRadius.x + collision->mRadius.x))
		{
			continue;
		}
		else if (abs(checkLocation->getY() - location->getY()) > (checkCollision->mRadius.y + collision->mRadius.y))
		{
			continue;
		}
		else
		{
			checkCollision->signal_Collisioned();
			collision->signal_Collisioned();
		}
	}
}

void GCollisionSystem::update(int dt)
{
}