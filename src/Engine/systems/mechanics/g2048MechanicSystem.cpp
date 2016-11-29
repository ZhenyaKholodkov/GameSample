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
				m2048Component->mLogicalNet[i][j] = -1;
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
	int x = col * m2048Component->mTitleWidth  + m2048Component->mTitleWidth / 2 - m2048Component->mFieldWidth / 2;
	int y = row * m2048Component->mTitleHieght  + m2048Component->mTitleHieght / 2 - m2048Component->mFieldHieght / 2;
	GLocationComponent* titleLocation = mEntityManager->AddComponentsToEntity<GLocationComponent>(titleEntity, x, y);
	GRenderableComponent* titleRenderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(titleEntity, m2048Component->mTitleBackground, 1.0f);
	titleRenderable->setVisible(true);
	GScalableComponent* scalble = mEntityManager->AddComponentsToEntity<GScalableComponent>(titleEntity, 0.0f, 0.0f, 1.0f, 1.0f, 500);
	mEntityManager->AddComponentsToEntity<GCounterComponent>(titleEntity, 0);
	GMoveableComponent* moveable = mEntityManager->AddComponentsToEntity<GMoveableComponent>(titleEntity);
	moveable->signal_LocationChanged.connect(titleLocation, &GLocationComponent::slot_LocationChanged);
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
		showRandomTitle(component,0 , 0);
		showRandomTitle(component, 0, 2);
		//showRandomTitle(component, 2, 1);
		//showRandomTitle(component, 3, 1);
	}
}

void G2048MechanicSystem::showRandomTitle(G2048MechanicComponent* component, int i , int j)
{
	int indexI = i;//randInt(0, component->mRows - 1);
	int indexJ = j;//randInt(0, component->mCols - 1);

	Entity title = component->mTitles[indexI][indexJ];

	GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(title);
	renderable->setVisible(true);
	GCounterComponent* counter = mEntityManager->GetComponent<GCounterComponent>(title);

	GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(title);
	GRenderableComponent* numberRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
	if (randInt(1, 100) <= 10)
	{
		numberRenderable->SetSprite(component->mTitleSprites[1]);
		component->mLogicalNet[indexI][indexJ] = 2;
	}
	else
	{
		numberRenderable->SetSprite(component->mTitleSprites[0]);
		component->mLogicalNet[indexI][indexJ] = 1;
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
		Entity moveToEntity = -1;
		Entity currentEntity = -1;
	//	uint32 movedIndex = component->mCols - 1;
		
		for (int i = component->mRows - 1; i >= 0 ; i--)
		{

			int moveToIndex = component->mCols - 1;
			int movedIndex = component->mCols - 2;
			//moveToEntity = component->mTitles[i][component->mCols - 1];
			while(movedIndex != -1)
			{
				//currentEntity = component->mTitles[i][j];
				//GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(currentEntity);

				if (component->mLogicalNet[i][movedIndex] == -1)
				{
					movedIndex--;
				}
				else
				{
					if (component->mLogicalNet[i][movedIndex] == component->mLogicalNet[i][moveToIndex])
					{
						moveTitleToTile(component->mTitles[i][movedIndex], component->mTitles[i][moveToIndex]);
						component->mLogicalNet[i][moveToIndex] *= 2;
					}
					else if (component->mLogicalNet[i][moveToIndex] == -1)
					{
						moveTitleToTile(component->mTitles[i][movedIndex], component->mTitles[i][moveToIndex]);
						component->mLogicalNet[i][moveToIndex] = component->mLogicalNet[i][movedIndex];
						component->mLogicalNet[i][movedIndex]  = -1;
					}
					else 
					{
						moveToIndex--;
						if (movedIndex != moveToIndex)
						{
							moveTitleToTile(component->mTitles[i][movedIndex], component->mTitles[i][moveToIndex]);
							component->mLogicalNet[i][moveToIndex] = component->mLogicalNet[i][movedIndex];
							component->mLogicalNet[i][movedIndex] = -1;
						}
					}
					//moveToIndex--;
					movedIndex--;
				}

			}
		}
	}
}

void G2048MechanicSystem::moveTitleToTile(Entity movedTitle, Entity toTitle)
{
	GMoveableComponent* movedMoveable = mEntityManager->GetComponent<GMoveableComponent>(movedTitle);
	GLocationComponent* movedLocation = mEntityManager->GetComponent<GLocationComponent>(movedTitle);
	GLocationComponent* toLocation = mEntityManager->GetComponent<GLocationComponent>(toTitle);
	movedMoveable->SetBeginX(movedLocation->getX());
	movedMoveable->SetBeginY(movedLocation->getY());
	movedMoveable->SetXDestination(toLocation->getX());
	movedMoveable->SetYDestination(toLocation->getY());
	movedMoveable->SetMovingTime(500);
	movedMoveable->slot_Move();

	//GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(toTitle);
	//GScalableComponent* scalable = mEntityManager->GetComponent<GScalableComponent>(toTitle);
	//scalable->Reset();
	//scalable->setBeginXScale(1.0f);
	//scalable->setBeginYScale(1.0f);
	//scalable->setEndXScale(0.0f);
	//scalable->setEndYScale(0.0f);
	//scalable->slot_Scale();
	//
	//scalable->signal_ScaleChangingFinished.disconnect_all();
	//scalable->signal_ScaleChangingFinished.connect(renderable, &GRenderableComponent::slot_SetInvisible);
}

void G2048MechanicSystem::update(int dt)
{
}