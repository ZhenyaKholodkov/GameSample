#include "gEntityManager.h"


GEntityManager* GEntityManager::Instance()
{
	static GEntityManager instance;
	return &instance;
}

GEntityManager::GEntityManager()
{
	for (uint32 index = 0; index < MAX_ENTITY_COUNT; ++index)
	{
		mAvailableEntities.push(index);
	}

	mComponentPools.resize(GetComponentCount());
	for (uint32 index = 0; index < GetComponentCount(); ++index)
	{
		mComponentPools[index] = nullptr;
	}
}

GEntityManager::~GEntityManager()
{
	for (auto pool : mComponentPools)
	{
		delete pool;
	}
}

Entity GEntityManager::CreateEntity()
{
	Entity newEntity = mAvailableEntities.front();
	mAvailableEntities.pop();
	mActiveEntities.push_back(newEntity);

	return newEntity;
}

void GEntityManager::DestroyEntity(Entity entity)
{
	mAvailableEntities.push(entity);
	for (std::vector<Entity>::const_iterator iter = mActiveEntities.begin(); iter != mActiveEntities.end(); iter++)
	{
		if ((*iter) == entity)
			mActiveEntities.erase(iter);
	}
}

uint32 GEntityManager::GetComponentCount()
{
	return GBaseComponent::s_component_counter;
}

bool GEntityManager::IsInsideEntity(Entity entity, GCursor point)
{
	if (!DoesHaveComponent<GLocationComponent>(entity) ||
		!DoesHaveComponent<GRenderableComponent>(entity))
	{
		return false;
	}
	GCursor localPoint;       //localized relativly the entity
	LocalPoint(entity, point, localPoint);

	GRenderableComponent* renderable = GetComponent<GRenderableComponent>(entity);
	return renderable->IsPiontInsideWH(localPoint);
}

void GEntityManager::LocalPoint(Entity entity, GCursor& point, GCursor& localPoint)
{
	GLocationComponent* location = GetComponent<GLocationComponent>(entity);

	float ang = 0;                         
	float c = cosf(ang), s = sinf(ang);
	localPoint.x = ( c*(point.x - location->getX()) + s*(point.y - location->getY()));
	localPoint.y = (-s*(point.x - location->getX()) + c*(point.y - location->getY()));
}