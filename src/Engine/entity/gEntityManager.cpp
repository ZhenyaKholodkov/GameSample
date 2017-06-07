#include "gEntityManager.h"

#include <algorithm>
/*
GEntityManager* GEntityManager::instance()
{
	static GEntityManager instance;
	return &instance;
}*/

GEntityManager::GEntityManager()
{
	for (uint32 index = 0; index < MAX_ENTITY_COUNT; ++index)
	{
		mAvailableEntities.push(index);
		mComponentIndexes.push_back(std::vector<size_t>(getComponentCount(), GBaseComponent::s_invalid_component_index()));
		mComponentMasks.push_back(ComponentMask());
	}

	mComponentPools.resize(getComponentCount());
}

GEntityManager::~GEntityManager()
{
}

Entity GEntityManager::createEntity()
{
	assert(mAvailableEntities.size() != 0);

	Entity newEntity = mAvailableEntities.front();
	mAvailableEntities.pop();

	/*if (mComponentIndexes.size() <= newEntity)
	{
		mComponentIndexes.push_back(std::vector<size_t>(getComponentCount(), GBaseComponent::s_invalid_component_index()));
	}*/

	return newEntity;
}

void GEntityManager::destroyEntity(Entity entity)
{
	if (entity == INVALID_ENTITY)
		return;

	mAvailableEntities.push(entity);
	mComponentMasks[entity].reset();
	std::fill(mComponentIndexes[entity].begin(), mComponentIndexes[entity].end(), GBaseComponent::s_invalid_component_index());

	for (uint32 index = 0; index < getComponentCount(); ++index)
	{
		mComponentPools[index]->destroy(mComponentIndexes[entity][index]);
	}
	GEntityManager::Iterator(shared_from_this(), ComponentMask(2), 0);
}

Entity GEntityManager::createPlainEntity(GSprite* sprite, float xPos, float yPos)
{
	Entity entity = createEntity();
	addComponentsToEntity<GLocationComponent>(entity, xPos, yPos);
	addComponentsToEntity<GRenderableComponent>(entity, sprite);
	return entity;

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
	auto renderable = getComponent<GRenderableComponent>(entity);

	float ang = renderable->getAngle() * DEGREE_TO_RAD;
	float c = cosf(ang), s = sinf(ang);
	localPoint.x = ( c*(point.x - location->getX()) + s*(point.y - location->getY())) / renderable->getXScale();
	localPoint.y = (-s*(point.x - location->getX()) + c*(point.y - location->getY())) / renderable->getXScale();
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
	parentLocation->signal_LocationChangedWithDxDy.connect(childLocation->slot_LocationChangedWithDxDy);

	auto parentRenderable = getComponent<GRenderableComponent>(parent);
	auto childRenderable = getComponent<GRenderableComponent>(child);
	if (parentRenderable && childRenderable)
	{
		parentRenderable->signal_ScaleChanged.connect(childRenderable->slot_ChangeScale);
		parentRenderable->signal_VisibilityChanged.connect(childRenderable->slot_VisibilityChanged);
	}
}