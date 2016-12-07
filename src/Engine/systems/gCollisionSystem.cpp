#include "GCollisionSystem.h"
#include "GCollisionComponent.h"

GCollisionSystem::GCollisionSystem() 
{
	mEntityManager = GEntityManager::Instance();
}

GCollisionSystem::~GCollisionSystem()
{
}

void GCollisionSystem::checkCollision(Entity checkEntity)
{
	if (!mEntityManager->DoesHaveComponent<GCollisionComponent>(checkEntity))
		return;
	GCollisionComponent* checkCollision = mEntityManager->GetComponent<GCollisionComponent>(checkEntity);
	GLocationComponent* checkLocation = mEntityManager->GetComponent<GLocationComponent>(checkEntity);

	for (auto iter = mEntityManager->GetBeginPairComponent<GCollisionComponent>(); iter != mEntityManager->GetEndPairComponent<GCollisionComponent>(); iter++)
	{
		Entity entity = (*iter)->first;
		if (checkEntity == entity)
			continue;

		if (mEntityManager->DoesHaveComponent<GRenderableComponent>(entity))
		{
			GRenderableComponent*  renderable = mEntityManager->GetComponent<GRenderableComponent>(entity);
			if (!renderable->isVisible())
				continue;
		}

		GCollisionComponent* collision = (*iter)->second;
		GLocationComponent*  location = mEntityManager->GetComponent<GLocationComponent>(entity);

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