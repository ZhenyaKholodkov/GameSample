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
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		for (uint32 i = 0; i < mCurrentMechanic->mRows; ++i)
		{
			for (uint32 j = 0; j < mCurrentMechanic->mCols; ++j)
			{
				mCurrentMechanic->mLogicalNet[i][j] = -1;
				mCurrentMechanic->mTitles[i][j] = createTitle(i, j, (*iter)->first);
				mCurrentMechanic->mAvailableEntities.push_back(mCurrentMechanic->mTitles[i][j]);
			}
		}
	}
}

Entity G2048MechanicSystem::createTitle(uint32 row, uint32 col, Entity self)
{
	//title
	Entity titleEntity = mEntityManager->CreateEntity();
	int x = col * mCurrentMechanic->mTitleWidth  + mCurrentMechanic->mTitleWidth / 2 - mCurrentMechanic->mFieldWidth / 2;
	int y = row * mCurrentMechanic->mTitleHieght  + mCurrentMechanic->mTitleHieght / 2 - mCurrentMechanic->mFieldHieght / 2;
	GLocationComponent* titleLocation = mEntityManager->AddComponentsToEntity<GLocationComponent>(titleEntity, x, y);
	GRenderableComponent* titleRenderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(titleEntity, mCurrentMechanic->mTitleBackground, 1.0f);
	titleRenderable->setVisible(false);
	GScalableComponent* scalble = mEntityManager->AddComponentsToEntity<GScalableComponent>(titleEntity, 0.0f, 0.0f, 1.0f, 1.0f, 500);
	mEntityManager->AddComponentsToEntity<GCounterComponent>(titleEntity, 0);
	GMoveableComponent* moveable = mEntityManager->AddComponentsToEntity<GMoveableComponent>(titleEntity);

	moveable->signal_LocationChanged.connect(titleLocation, &GLocationComponent::slot_LocationChanged);
	moveable->signal_MovingFinished.connect(mCurrentMechanic, &G2048MechanicComponent::slot_ReportMovingFinished);
	scalble->signal_ScaleChanged.connect(titleRenderable, &GRenderableComponent::slot_ChangeScale);

	//number
	Entity numberEntity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(numberEntity, 0.0f, 0.0f);
	GRenderableComponent* numberRenderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(numberEntity, mCurrentMechanic->mTitleSprites[0]);
	numberRenderable->setVisible(false);

	mEntityManager->setChildParentRelations(self, titleEntity);
	mEntityManager->setChildParentRelations(titleEntity, numberEntity);

	return titleEntity;
}

void G2048MechanicSystem::runGame()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		showRandomTitle(0 , 0);
		showRandomTitle(0, 2);
		showRandomTitle(2, 1);
		showRandomTitle(3, 0);
		showRandomTitle(3, 1);
		showRandomTitle(3, 2);
		showRandomTitle(3, 3);
	}
}

void G2048MechanicSystem::showRandomTitle(int i , int j)
{
	int indexI = i;//randInt(0, component->mRows - 1);
	int indexJ = j;//randInt(0, component->mCols - 1);

	Entity title = mCurrentMechanic->mTitles[indexI][indexJ];

	GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(title);
	renderable->setVisible(true);
	GCounterComponent* counter = mEntityManager->GetComponent<GCounterComponent>(title);

	GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(title);
	GRenderableComponent* numberRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
	if (randInt(1, 100) <= 10)
	{
		numberRenderable->SetSprite(mCurrentMechanic->mTitleSprites[1]);
		mCurrentMechanic->mLogicalNet[indexI][indexJ] = 2;
	}
	else
	{
		numberRenderable->SetSprite(mCurrentMechanic->mTitleSprites[0]);
		mCurrentMechanic->mLogicalNet[indexI][indexJ] = 1;
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
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
				
		for (int i = mCurrentMechanic->mRows - 1; i >= 0 ; i--)
		{

			int moveToIndex = mCurrentMechanic->mCols - 1;
			int movedIndex = mCurrentMechanic->mCols - 2;
			while(movedIndex != -1)
			{
				if (mCurrentMechanic->mLogicalNet[i][movedIndex] == -1)
				{
					movedIndex--;
				}
				else
				{
					if (mCurrentMechanic->mLogicalNet[i][movedIndex] == mCurrentMechanic->mLogicalNet[i][moveToIndex])
					{
						mCurrentMechanic->mLogicalNet[i][movedIndex] = -1;
						mCurrentMechanic->mLogicalNet[i][moveToIndex]++;
						moveTitleToTitle(mCurrentMechanic->mTitles[i][movedIndex], mCurrentMechanic->mTitles[i][moveToIndex], mCurrentMechanic->mLogicalNet[i][moveToIndex]);
					}
					else if (mCurrentMechanic->mLogicalNet[i][moveToIndex] == -1)
					{
						mCurrentMechanic->mLogicalNet[i][moveToIndex] = mCurrentMechanic->mLogicalNet[i][movedIndex];
						mCurrentMechanic->mLogicalNet[i][movedIndex] = -1;
						moveTitleToTitle(mCurrentMechanic->mTitles[i][movedIndex], mCurrentMechanic->mTitles[i][moveToIndex], mCurrentMechanic->mLogicalNet[i][moveToIndex]);
					}
					else 
					{
						moveToIndex--;
						if (movedIndex != moveToIndex)
						{
							mCurrentMechanic->mLogicalNet[i][moveToIndex] = mCurrentMechanic->mLogicalNet[i][movedIndex];
							mCurrentMechanic->mLogicalNet[i][movedIndex] = -1;
							moveTitleToTitle(mCurrentMechanic->mTitles[i][movedIndex], mCurrentMechanic->mTitles[i][moveToIndex], mCurrentMechanic->mLogicalNet[i][moveToIndex]);
						}
					}
					movedIndex--;
				}

			}
		}
	}
}

void G2048MechanicSystem::recalFieldAfterAnimation()
{
	Entity entity = -1;
	int value = -1;
	for (uint32 i = 0; i < mCurrentMechanic->mRows; ++i)
	{
		for (uint32 j = 0; j < mCurrentMechanic->mCols; ++j)
		{
			entity = mCurrentMechanic->mTitles[i][j];
			value = mCurrentMechanic->mLogicalNet[i][j];

			GLocationComponent*   location = mEntityManager->GetComponent<GLocationComponent>(entity);
			GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(entity);
			location->restoreLocation();

			if (value != -1)
			{
				renderable->setVisible(true);
				GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(entity);
				GRenderableComponent* childRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
				childRenderable->SetSprite(mCurrentMechanic->mTitleSprites[value - 1]);
			}
			else
			{
				renderable->setVisible(false);
			}
		}
	}
}

void G2048MechanicSystem::moveTitleToTitle(Entity movedTitle, Entity toTitle, int newValue)
{
	GMoveableComponent*   movedMoveable = mEntityManager->GetComponent<GMoveableComponent>(movedTitle);
	GLocationComponent*   movedLocation = mEntityManager->GetComponent<GLocationComponent>(movedTitle);
	GRenderableComponent* movedRenderable = mEntityManager->GetComponent<GRenderableComponent>(movedTitle);
	GLocationComponent*   toLocation = mEntityManager->GetComponent<GLocationComponent>(toTitle);
	GRenderableComponent* toRenderable = mEntityManager->GetComponent<GRenderableComponent>(toTitle);

	movedMoveable->SetBeginX(movedLocation->getX());
	movedMoveable->SetBeginY(movedLocation->getY());
	movedMoveable->SetXDestination(toLocation->getX());
	movedMoveable->SetYDestination(toLocation->getY());
	movedMoveable->SetMovingTime(500);
	movedMoveable->slot_Move();

	mCurrentMechanic->mTitlesInMoving++;

	//movedMoveable->signal_MovingFinished.disconnect_all();
	//movedMoveable->signal_MovingFinished.connect(movedLocation, &GLocationComponent::slot_RestoreLocation);
	//movedMoveable->signal_MovingFinished.connect(movedRenderable, &GRenderableComponent::slot_SetInvisible);
	//movedMoveable->signal_MovingFinished.connect(toRenderable, &GRenderableComponent::slot_SetVisible);
	//
	//if (newValue != -1)
	//{
	//	GChildComponent*      toChild = mEntityManager->GetComponent<GChildComponent>(movedTitle);
	//	GRenderableComponent* toChildRenderable = mEntityManager->GetComponent<GRenderableComponent>(toChild->getChild());
	//
	//	movedMoveable->signal_Data = (void*)mCurrentMechanic->mTitleSprites[newValue - 1];
	//	movedMoveable->signal_MovingFinishedWithData.connect(toRenderable, &GRenderableComponent::slot_ChangeSprite);
	//}
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

	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		if (mCurrentMechanic->mState == G2048MechanicComponent::STATE_RECALC_MATRIX)
		{
			recalFieldAfterAnimation();
			mCurrentMechanic->mState = G2048MechanicComponent::STATE_WAIT;
		}
	}
}