#include "g2048MechanicSystem.h"


G2048MechanicSystem::G2048MechanicSystem() 
{
	mEntityManager = GEntityManager::Instance();
}

G2048MechanicSystem::~G2048MechanicSystem() 
{
}

void G2048MechanicSystem::createField()
{
	for (uint32 i = 0; i < m2048Component->mRows; ++i)
	{
		for (uint32 j = 0; j < m2048Component->mCols; ++j)
		{

		}
	}
}

Entity G2048MechanicSystem::createTitle(uint32 row, uint32 col, Entity self)
{
	Entity titleEntity = mEntityManager->CreateEntity();
	int x = row * m2048Component->mTitleWidth / 2 + m2048Component->mTitleWidth / 2 - m2048Component->mFieldWidth / 2;
	int y = col * m2048Component->mTitleHieght / 2 + m2048Component->mTitleHieght / 2 - m2048Component->mFieldHieght / 2;
	mEntityManager->AddComponentsToEntity<GLocationComponent>(titleEntity, x, y);
	mEntityManager->AddComponentsToEntity<GRenderableComponent>(titleEntity, m2048Component->mTitleBackground, 0.0f);
	mEntityManager->AddComponentsToEntity<GScalableComponent>(titleEntity, 0.0f, 0.0f, 1.0f, 1.0f, 500);
	mEntityManager->AddComponentsToEntity<GMoveableComponent>(titleEntity);

	Entity numberEntity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(numberEntity, 0.0f, 0.0f);
	mEntityManager->AddComponentsToEntity<GRenderableComponent>(numberEntity);

	mEntityManager->setChildParentRelations(titleEntity, numberEntity);
	mEntityManager->setChildParentRelations(self, titleEntity);
}

void G2048MechanicSystem::update(int dt)
{
}