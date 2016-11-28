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
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		G2048MechanicComponent* m2048Component = static_cast<G2048MechanicComponent*>((*iter)->second);
		for (uint32 i = 0; i < m2048Component->mRows; ++i)
		{
			for (uint32 j = 0; j < m2048Component->mCols; ++j)
			{
				m2048Component->mTitles[i][j] = createTitle(i, j, (*iter)->first, m2048Component);
				m2048Component->mAvailableEntities.push_back(m2048Component->mTitles[i][j]);
			}
		}
	}
}

Entity G2048MechanicSystem::createTitle(uint32 row, uint32 col, Entity self, G2048MechanicComponent* m2048Component)
{
	//title
	Entity titleEntity = mEntityManager->CreateEntity();
	int x = row * m2048Component->mTitleWidth  + m2048Component->mTitleWidth / 2 - m2048Component->mFieldWidth / 2;
	int y = col * m2048Component->mTitleHieght  + m2048Component->mTitleHieght / 2 - m2048Component->mFieldHieght / 2;
	mEntityManager->AddComponentsToEntity<GLocationComponent>(titleEntity, x, y);
	GRenderableComponent* titleRenderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(titleEntity, m2048Component->mTitleBackground, 1.0f);
	titleRenderable->setVisible(false);
	GScalableComponent* scalble = mEntityManager->AddComponentsToEntity<GScalableComponent>(titleEntity, 0.0f, 0.0f, 1.0f, 1.0f, 1000);
	mEntityManager->AddComponentsToEntity<GMoveableComponent>(titleEntity);
	scalble->signal_ScaleChanged.connect(titleRenderable, &GRenderableComponent::slot_ChangeScale);

	//number
	Entity numberEntity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(numberEntity, 0.0f, 0.0f);
	GRenderableComponent* numberRenderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(numberEntity, m2048Component->mTitleSprites[0]);
	numberRenderable->setVisible(false);

	mEntityManager->setChildParentRelations(self, titleEntity);
	mEntityManager->setChildParentRelations(titleEntity, numberEntity);

	return titleEntity;
}

void G2048MechanicSystem::runGame()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		G2048MechanicComponent* component = static_cast<G2048MechanicComponent*>((*iter)->second);
		showRandomTitle(component);
		showRandomTitle(component);
	}
}

void G2048MechanicSystem::showRandomTitle(G2048MechanicComponent* component)
{
	int index = randInt(0, component->mAvailableEntities.size() - 1);

	Entity title = component->mAvailableEntities[index];
	component->mAvailableEntities.erase(component->mAvailableEntities.begin() + index);

	GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(title);
	renderable->setVisible(true);

	GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(title);
	GRenderableComponent* numberRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
	if (randInt(1, 100) <= 10)
	{
		numberRenderable->SetSprite(component->mTitleSprites[1]);
	}
	else
	{
		numberRenderable->SetSprite(component->mTitleSprites[0]);
	}

	GScalableComponent* scalable = mEntityManager->GetComponent<GScalableComponent>(title);
	scalable->Reset();
	scalable->setBeginXScale(0.0f);
	scalable->setBeginYScale(0.0f);
	scalable->setEndXScale(1.0f);
	scalable->setEndYScale(1.0f);
	scalable->SetState(GScalableComponent::STATE_SCALE);
}

void G2048MechanicSystem::slot_MoveLeft()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		G2048MechanicComponent* component = static_cast<G2048MechanicComponent*>((*iter)->second);

		Entity movedEntity = -1;
		Entity currentEntity = -1;
		uint32 movedIndex = component->mCols - 1;

		for (uint32 i = component->mRows - 1; i > 0 ; --i)
		{
			for (uint32 j = component->mCols - 1; j > 0; --j)
			{
				currentEntity = component->mTitles[i][j];
				GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(currentEntity);
				if (!renderable->isVisible())
				{
					continue;
				}
				else
				{
					if (movedEntity == -1)
					{
						movedEntity = currentEntity;
						movedIndex = j;
					}
					else
					{
						GMoveableComponent* moveable = mEntityManager->GetComponent<GMoveableComponent>(movedEntity);
						GLocationComponent* toLocation = mEntityManager->GetComponent<GLocationComponent>(currentEntity);
						moveable->SetXDestination(toLocation->getX());
						moveable->SetYDestination(toLocation->getY());
						moveable->SetMovingTime(500);

						GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(currentEntity);
						GScalableComponent* scalable = mEntityManager->GetComponent<GScalableComponent>(currentEntity);
						scalable->Reset();
						scalable->setBeginXScale(1.0f);
						scalable->setBeginYScale(1.0f);
						scalable->setEndXScale(0.0f);
						scalable->setEndYScale(0.0f);
						scalable->SetState(GScalableComponent::STATE_SCALE);

						scalable->signal_ScaleChangingFinished.disconnect_all();
						scalable->signal_ScaleChangingFinished.connect(renderable, &GRenderableComponent::slot_SetInvisible);

						movedEntity = -1; 
						currentEntity = -1;

						j = movedIndex;
					}
				}
			}
		}
	}
}

void G2048MechanicSystem::update(int dt)
{
}