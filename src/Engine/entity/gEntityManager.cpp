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
		mComponentIndexes.push_back(std::vector<size_t>(getComponentCount(), 0));
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

	auto  childComponent = getComponent<GChildComponent>(entity);
	if (childComponent)
	{
		std::vector<Entity> childs;
		childComponent->getChilds(childs);
		for(Entity child : childs)
		{
			destroyEntity(child);
		}
	}
	removeParent(entity);
	for (uint32 index = 0; index < getComponentCount(); ++index)
	{
		if (mComponentMasks[entity].contains(index))
			mComponentPools[index]->destroy(mComponentIndexes[entity][index]);
	}
	mComponentMasks[entity].reset();
	mAvailableEntities.push(entity);
	std::fill(mComponentIndexes[entity].begin(), mComponentIndexes[entity].end(), 0);
	GEntityManager::Iterator(shared_from_this(), ComponentMask(2), 0);
}

void GEntityManager::destroyAllEntites()
{
	for (Entity entity = 0; entity < MAX_ENTITY_COUNT; ++entity)
	{
		if (!mComponentMasks[entity].empty())
		{
			destroyEntity(entity);
		}
	}
}

Entity GEntityManager::createPlainEntity(GSprite* sprite, float xPos, float yPos)
{
	Entity entity = createEntity();
	addComponentsToEntity<GLocationComponent>(entity, xPos, yPos);
	addComponentsToEntity<GRenderableComponent>(entity, sprite);
	return entity;

}

Entity GEntityManager::createButtonEntity(GSprite* normal, GSprite* move, GSprite* down, float xPos, float yPos)
{
	Entity entity = createEntity();
	addComponentsToEntity<GLocationComponent>(entity, xPos, yPos);
	auto render = addComponentsToEntity<GRenderableComponent>(entity, normal);

	auto moveable = addComponentsToEntity<GMouseMoveEventComponent>(entity, move, normal);
	auto downable = addComponentsToEntity<GMouseDownEventComponent>(entity, down);
	auto upable = addComponentsToEntity<GMouseUpEventComponent>(entity, normal);

	moveable->signal_MouseMovedInEntity.connect(render->slot_ChangeSprite);
	moveable->signal_MouseMovedOutEntity.connect(render->slot_ChangeSprite);
	downable->signal_MouseDownNewSprite.connect(render->slot_ChangeSprite);
	upable->signal_MouseUpNewSprite.connect(render->slot_ChangeSprite);

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

	if (renderable->getXScale() == 0 || renderable->getYScale() == 0)
		return;

	float ang = renderable->getAngle() * DEGREE_TO_RAD;
	float c = cosf(ang), s = sinf(ang);
	if (!doesHaveComponent<GParentComponent>(entity))
	{
		localPoint.x = (c*(point.x - location->getX()) + s*(point.y - location->getY())) / renderable->getXScale();
		localPoint.y = (-s*(point.x - location->getX()) + c*(point.y - location->getY())) / renderable->getYScale();
	}
	else
	{
		auto parent = getComponent<GParentComponent>(entity);
		GCursor parentLocal;
		getLocalPoint(parent->getParent(), point, parentLocal);
		localPoint.x = (c*(parentLocal.x - location->getX()) + s*(parentLocal.y - location->getY())) / renderable->getXScale();
		localPoint.y = (-s*(parentLocal.x - location->getX()) + c*(parentLocal.y - location->getY())) / renderable->getYScale();
	}
}

void GEntityManager::removeParent(Entity child)
{
	if (doesHaveComponent<GParentComponent>(child))
	{
		auto parentComponent = getComponent<GParentComponent>(child);
		Entity parent = parentComponent->getParent();
		auto  childComponent = getComponent<GChildComponent>(parent);

		parentComponent->setParent(-1);
		if (childComponent)
		{
			childComponent->removeChild(child);
			/*auto parentLocation = getComponent<GLocationComponent>(parent);
			auto childLocation = getComponent<GLocationComponent>(child);
			float newX = childLocation->getX() - parentLocation->getX();
			float newY = childLocation->getY() - parentLocation->getY();
			childLocation->setXY(newX, newY);
			parentLocation->signal_LocationChangedWithDxDy.disconnect(childLocation);*/

		/*	auto parentRenderable = getComponent<GRenderableComponent>(parent);
			auto childRenderable = getComponent<GRenderableComponent>(child);
			parentRenderable->signal_ScaleChanged.disconnect(childRenderable);
			parentRenderable->signal_VisibilityChanged.disconnect(childRenderable);*/
		}
	}
}


void GEntityManager::setChildParentRelations(Entity parent, Entity child)
{
	removeParent(child);
	/*auto parentLocation = getComponent<GLocationComponent>(parent);
	auto childLocation = getComponent<GLocationComponent>(child);*/

	auto parentComponent = doesHaveComponent<GParentComponent>(child) ? getComponent<GParentComponent>(child) : addComponentsToEntity<GParentComponent>(child);
	parentComponent->setParent(parent);

	auto childComponent = doesHaveComponent<GChildComponent>(parent) ? getComponent<GChildComponent>(parent) : addComponentsToEntity<GChildComponent>(parent);
	childComponent->addChild(child);

	/*float newX = childLocation->getX() + parentLocation->getX();
	float newY = childLocation->getY() + parentLocation->getY();
	childLocation->setXY(newX, newY);
	childLocation->setDefaultXY(newX, newY);
	parentLocation->signal_LocationChangedWithDxDy.connect(childLocation->slot_LocationChangedWithDxDy);*/

	/*auto parentRenderable = getComponent<GRenderableComponent>(parent);
	auto childRenderable = getComponent<GRenderableComponent>(child);
	childRenderable->setXYScale(childRenderable->getXScale() * parentRenderable->getXScale(),
		                        childRenderable->getYScale() * parentRenderable->getYScale());
	if (parentRenderable && childRenderable)
	{
		parentRenderable->signal_ScaleChanged.connect(childRenderable->slot_ChangeScale);
		parentRenderable->signal_VisibilityChanged.connect(childRenderable->slot_VisibilityChanged);
	}*/
}