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

	return newEntity;
}

void GEntityManager::DestroyEntity(Entity entity)
{
	if (entity < 0)
		return;

	mAvailableEntities.push(entity);
	for (uint32 index = 0; index < GetComponentCount(); ++index)
	{
		mComponentPools[index]->destroy(entity);
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

void GEntityManager::removeParent(Entity child)
{
	GParentComponent* parentComponent = GetComponent<GParentComponent>(child);
	if (parentComponent)
	{
		Entity parent = parentComponent->getParent();
		GChildComponent*  childComponent = GetComponent<GChildComponent>(parent);

		parentComponent->setParent(-1);
		if (childComponent)
		{
			childComponent->setChild(-1);
			GLocationComponent* parentLocation = GetComponent<GLocationComponent>(parent);
			GLocationComponent* childLocation = GetComponent<GLocationComponent>(child);
			float newX = childLocation->getX() - parentLocation->getX();
			float newY = childLocation->getY() - parentLocation->getY();
			childLocation->setXY(newX, newY);
			parentLocation->signal_LocationChangedWithDxDy.disconnect(childLocation);
		}
	}
}

void GEntityManager::setChildParentRelations(Entity parent, Entity child)
{
	removeParent(child);
	GLocationComponent* parentLocation = GetComponent<GLocationComponent>(parent);
	GLocationComponent* childLocation = GetComponent<GLocationComponent>(child);

	GParentComponent* parentComponent = GetComponent<GParentComponent>(child);
	if (!parentComponent)
	{
		parentComponent = AddComponentsToEntity<GParentComponent>(child);
	}
	parentComponent->setParent(parent);

	GChildComponent* childComponent = GetComponent<GChildComponent>(parent);
	if (!childComponent)
	{
		childComponent = AddComponentsToEntity<GChildComponent>(parent);
	}
	childComponent->setChild(child);

	float newX = childLocation->getX() + parentLocation->getX();
	float newY = childLocation->getY() + parentLocation->getY();
	childLocation->setXY(newX, newY);
	childLocation->setDefaultXY(newX, newY);
	parentLocation->signal_LocationChangedWithDxDy.connect(childLocation, &GLocationComponent::slot_LocationChangedWithDxDy);

	GRenderableComponent* parentRenderable = GetComponent<GRenderableComponent>(parent);
	GRenderableComponent* childRenderable = GetComponent<GRenderableComponent>(child);
	if (parentRenderable && childRenderable)
	{
		parentRenderable->signal_ScaleChanged.connect(childRenderable, &GRenderableComponent::slot_ChangeScale);
		parentRenderable->signal_VisibilityChanged.connect(childRenderable, &GRenderableComponent::slot_VisibilityChanged);
	}
}