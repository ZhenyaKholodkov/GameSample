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
				mCurrentMechanic->mLogicalNet[i*mCurrentMechanic->mRows + j] = -1;
				mCurrentMechanic->mTitles[i*mCurrentMechanic->mRows + j] = createTitle(i, j, (*iter)->first);
				mCurrentMechanic->mAvailableEntities.push_back(mCurrentMechanic->mTitles[i*mCurrentMechanic->mRows + j]);
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

	Entity title = mCurrentMechanic->mTitles[indexI*mCurrentMechanic->mRows+indexJ];

	GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(title);
	renderable->setVisible(true);
	GCounterComponent* counter = mEntityManager->GetComponent<GCounterComponent>(title);

	GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(title);
	GRenderableComponent* numberRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
	if (randInt(1, 100) <= 10)
	{
		numberRenderable->SetSprite(mCurrentMechanic->mTitleSprites[1]);
		mCurrentMechanic->mLogicalNet[indexI*mCurrentMechanic->mRows+indexJ] = 2;
	}
	else
	{
		numberRenderable->SetSprite(mCurrentMechanic->mTitleSprites[0]);
		mCurrentMechanic->mLogicalNet[indexI*mCurrentMechanic->mRows+indexJ] = 1;
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
		moveField(DIRECTION_LEFT);
	}
}

void G2048MechanicSystem::slot_MoveRight()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		moveField(DIRECTION_RIGHT);
	}
}

void G2048MechanicSystem::slot_MoveTop()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		moveField(DIRECTION_TOP);
	}
}

void G2048MechanicSystem::slot_MoveBottom()
{
	for (auto iter = mEntityManager->GetBeginPairComponent<G2048MechanicComponent>(); iter != mEntityManager->GetEndPairComponent<G2048MechanicComponent>(); iter++)
	{
		mCurrentMechanic = static_cast<G2048MechanicComponent*>((*iter)->second);
		moveField(DIRECTION_BOTTOM);
	}
}

void G2048MechanicSystem::moveField(uint32 direction)
{
	int incremental = 1;

	int rowBeg = 0;
	int rowEnd = mCurrentMechanic->mRows - 1;
	int colBeg = 0;
	int colEnd = mCurrentMechanic->mCols - 1;

	if (direction == DIRECTION_LEFT || direction == DIRECTION_TOP)
	{
		rowBeg = mCurrentMechanic->mRows - 1;
		rowEnd = -1;
		colBeg = mCurrentMechanic->mCols - 1;
		colEnd = -1;
		incremental = -1;
	}

	int row = rowBeg;
	int col = colBeg;

	int movedIndex = rowBeg;
	int moveToIndex = rowBeg;
	while (row != rowEnd)
	{
		col = colBeg;
		moveToIndex = row * mCurrentMechanic->mRows + col;
		while (col != colEnd)
		{
			movedIndex = row * mCurrentMechanic->mRows + col;

			if (mCurrentMechanic->mLogicalNet[movedIndex] != -1)
			{
				if (mCurrentMechanic->mLogicalNet[movedIndex] == mCurrentMechanic->mLogicalNet[moveToIndex])
				{
					mCurrentMechanic->mLogicalNet[movedIndex] = -1;
					mCurrentMechanic->mLogicalNet[moveToIndex]++;
					moveTitleToTitle(mCurrentMechanic->mTitles[movedIndex], mCurrentMechanic->mTitles[moveToIndex], mCurrentMechanic->mLogicalNet[moveToIndex]);
				}
				else
				{
					if (mCurrentMechanic->mLogicalNet[moveToIndex] != -1)
					{
						moveToIndex += incremental;
					}
					mCurrentMechanic->mLogicalNet[moveToIndex] = mCurrentMechanic->mLogicalNet[movedIndex];
					mCurrentMechanic->mLogicalNet[movedIndex] = -1;
					moveTitleToTitle(mCurrentMechanic->mTitles[movedIndex], mCurrentMechanic->mTitles[moveToIndex], mCurrentMechanic->mLogicalNet[moveToIndex]);
				}
			}
			col += incremental;
		}
		row += incremental;
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
			entity = mCurrentMechanic->mTitles[i*mCurrentMechanic->mRows+j];
			value = mCurrentMechanic->mLogicalNet[i*mCurrentMechanic->mRows+j];

			GLocationComponent*   location = mEntityManager->GetComponent<GLocationComponent>(entity);
			GRenderableComponent* renderable = mEntityManager->GetComponent<GRenderableComponent>(entity);
			location->restoreLocation();

			if (value != -1)
			{
				renderable->setVisible(true);
				GChildComponent* child = mEntityManager->GetComponent<GChildComponent>(entity);
				GRenderableComponent* childRenderable = mEntityManager->GetComponent<GRenderableComponent>(child->getChild());
				childRenderable->SetSprite(mCurrentMechanic->mTitleSprites[value]);
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