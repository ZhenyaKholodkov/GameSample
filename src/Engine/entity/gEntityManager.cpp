#include "gEntityManager.h"

GEntityManager* GEntityManager::instance()
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

	mComponentPools.resize(getComponentCount());
}

GEntityManager::~GEntityManager()
{
}

Entity GEntityManager::createEntity()
{
	Entity newEntity = mAvailableEntities.front();
	mAvailableEntities.pop();

	return newEntity;
}

void GEntityManager::destroyEntity(Entity entity)
{
	if (entity < 0)
		return;

	mAvailableEntities.push(entity);
	/*for (uint32 index = 0; index < getComponentCount(); ++index)
	{
		mComponentPools[index].destroy(entity);
	}*/
}

uint32 GEntityManager::getComponentCount()
{
	return GBaseComponent::s_component_counter;
}

bool GEntityManager::isInsideEntity(Entity entity, GCursor point) const
{
	if (!doesHaveComponent<GLocationComponent>(entity) ||
		!doesHaveComponent<GRenderableComponent>(entity))
	{
		return false;
	}
	GCursor localPoint;       //localized relativly the entity
	getLocalPoint(entity, point, localPoint);

	auto renderable = getComponent<GRenderableComponent>(entity);
	return renderable->IsPiontInsideWH(localPoint);
}

void GEntityManager::getLocalPoint(Entity entity, GCursor& point, GCursor& localPoint) const
{
	auto location = getComponent<GLocationComponent>(entity);

	float ang = 0;                         
	float c = cosf(ang), s = sinf(ang);
	localPoint.x = ( c*(point.x - location->getX()) + s*(point.y - location->getY()));
	localPoint.y = (-s*(point.x - location->getX()) + c*(point.y - location->getY()));
}

void GEntityManager::removeParent(Entity child)
{
	auto parentComponent = getComponent<GParentComponent>(child);
	if (parentComponent)
	{
		Entity parent = parentComponent->getParent();
		auto  childComponent = getComponent<GChildComponent>(parent);

		parentComponent->setParent(-1);
		if (childComponent)
		{
			childComponent->setChild(-1);
			auto parentLocation = getComponent<GLocationComponent>(parent);
			auto childLocation = getComponent<GLocationComponent>(child);
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
	auto parentLocation = getComponent<GLocationComponent>(parent);
	auto childLocation = getComponent<GLocationComponent>(child);

	auto parentComponent = getComponent<GParentComponent>(child);
	if (!parentComponent)
	{
		parentComponent = addComponentsToEntity<GParentComponent>(child);
	}
	parentComponent->setParent(parent);

	auto childComponent = getComponent<GChildComponent>(parent);
	if (!childComponent)
	{
		childComponent = addComponentsToEntity<GChildComponent>(parent);
	}
	childComponent->setChild(child);

	float newX = childLocation->getX() + parentLocation->getX();
	float newY = childLocation->getY() + parentLocation->getY();
	childLocation->setXY(newX, newY);
	childLocation->setDefaultXY(newX, newY);
	parentLocation->signal_LocationChangedWithDxDy.connect(childLocation, &GLocationComponent::slot_LocationChangedWithDxDy);

	auto parentRenderable = getComponent<GRenderableComponent>(parent);
	auto childRenderable = getComponent<GRenderableComponent>(child);
	if (parentRenderable && childRenderable)
	{
		parentRenderable->signal_ScaleChanged.connect(childRenderable, &GRenderableComponent::slot_ChangeScale);
		parentRenderable->signal_VisibilityChanged.connect(childRenderable, &GRenderableComponent::slot_VisibilityChanged);
	}
}