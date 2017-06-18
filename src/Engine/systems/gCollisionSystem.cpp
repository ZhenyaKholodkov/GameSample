#include "GCollisionSystem.h"
#include "GCollisionComponent.h"

GCollisionSystem::GCollisionSystem(std::shared_ptr<GEntityManager> manager) :
GSystem<GCollisionSystem>(manager)
{
}

GCollisionSystem::~GCollisionSystem()
{
}

void GCollisionSystem::checkCollision(Entity checkEntity)
{
	GCollisionComponent* checkCollision = mEntityManager->getComponent<GCollisionComponent>(checkEntity);
	GLocationComponent* checkLocation = mEntityManager->getComponent<GLocationComponent>(checkEntity);

	mEntityManager->each<GCollisionComponent, GRenderableComponent, GLocationComponent>(
		[&](Entity entity, GCollisionComponent& collision, GRenderableComponent& renderable, GLocationComponent& location)
	{
		if (checkEntity == entity)
			return;
		
		float dx = checkLocation->getX() - location.getX();
		float dy = checkLocation->getY() - location.getY();
		float distance = sqrt(dx * dx + dy * dy);

		if (abs(checkLocation->getX() - location.getX()) > (checkCollision->mRadius.x + collision.mRadius.x))
		{
			return;
		}
		else if (abs(checkLocation->getY() - location.getY()) > (checkCollision->mRadius.y + collision.mRadius.y))
		{
			return;
		}
		else
		{
			checkCollision->signal_Collisioned();
			checkCollision->signal_CollisionedWith(entity);
			checkCollision->signal_CollisionedWho(checkEntity);
			if (checkCollision->mDestroyAfterCollision)
				mEntityManager->destroyEntity(checkEntity);
		}
	});
}

void GCollisionSystem::update(int dt)
{
	if (isStoped())
		return;
	mEntityManager->each<GCollisionComponent>(
		[&](Entity entity, GCollisionComponent& collision)
	{
	//	if (collision.mNeedCheck)
			checkCollision(entity);
	});
}