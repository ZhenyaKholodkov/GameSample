#include "gGameGun.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

#include <time.h>
#include <algorithm>
#include <random>

GGameGun::GGameGun(std::shared_ptr<GEntityManager> manager) :
	mEntityManager(manager),
	mLines(3),
    mSetCount(40),
    mVelocity(250),
	mDefaultSpaceCount(50),
    mDefaultCoinCount(40),
    mDefaultBagCount(10),
    mSpaceCount(50),
    mCoinCount(40),
    mBagCount(10),
    mLimitSpaceCount(10),
    mLimitCoinCount(65),
    mLimitBagCount(25),
    mChangeSpaceCount(-8),
    mChangeCoinCount(5),
    mChangeBagCount(3),
	mCurrentIndex(0),
	mYBorderForNextEntity(0.0f),
	mTimeToNextEntity(0),
	mGun(INVALID_ENTITY)
{

}
GGameGun::~GGameGun()
{
	unload();
}

void GGameGun::unload()
{
	signal_Lost.disconnect_all_slots();
	signal_GotCoin.disconnect_all_slots();
	mEntityManager->destroyEntity(mGun);
}

void GGameGun::start()
{
	mSpaceCount = mDefaultSpaceCount;
	mCoinCount = mDefaultCoinCount;
	mBagCount = mDefaultBagCount;
	mGun = mEntityManager->createEntity();
	generate();
	createNextEntity();
}

void GGameGun::update(int dt)
{
	mCurrentTimeToNextEntity -= dt;
	if (mCurrentTimeToNextEntity <= 0) {
		createNextEntity();
	}
}
void GGameGun::generate()
{
	mSequence.clear();
	uint32 lastSpaceIndex = mSetCount * mSpaceCount / 100;
	uint32 lastCoinIndex = mSetCount * (mSpaceCount + mCoinCount) / 100;
	uint32 lastBagIndex = mSetCount * (mSpaceCount + mCoinCount + mBagCount) / 100;
	for (uint32 i = 0; i < mSetCount; ++i)
	{
		if (i < lastSpaceIndex)
			mSequence.push_back(0);
		else if (i < lastCoinIndex)
			mSequence.push_back(1);
		else 
			mSequence.push_back(2);
	}
	auto engine = std::default_random_engine{};
	std::shuffle(mSequence.begin(), mSequence.end(), engine);
	mSequence[0] = 1;
	mCurrentIndex = 0;

	mSpaceCount = mSpaceCount < mLimitSpaceCount ? mSpaceCount + mChangeSpaceCount : mLimitSpaceCount;
	mCoinCount = mCoinCount < mLimitCoinCount ? mCoinCount + mChangeCoinCount : mLimitCoinCount;
	mBagCount = mBagCount < mLimitBagCount ? mBagCount + mChangeBagCount : mLimitBagCount;
}

void GGameGun::createNextEntity()
{
	mCurrentTimeToNextEntity = mTimeToNextEntity;
	if (mCurrentIndex == mSetCount)
		generate();
	GSprite* sprite = nullptr;
	if (mSequence[mCurrentIndex] == 0)
	{
		mYBorderForNextEntity *= 2;
		++mCurrentIndex;
		return;
	}
	else if (mSequence[mCurrentIndex] == 1)
	{
		sprite = GResManager::Instance()->GetSprite("coin.png");
		mTimeToNextEntity = static_cast<int>(sprite->getHeight()) * 2000 / mVelocity;
	}
	else if (mSequence[mCurrentIndex] == 2)
	{
		sprite = GResManager::Instance()->GetSprite("bag.png");
		mTimeToNextEntity = static_cast<int>(sprite->getHeight()) * 4000 / mVelocity;
	}

	if (!sprite)
		return;
	srand(time(0));
	float rnd = static_cast<float>(rand() % 3);
	float x = static_cast<float>(WIDTH) / 6.0f + rnd * static_cast<float>(WIDTH) / 3.0f;
	float y = -sprite->getHeight() / 2.0f;
	float yDestination = static_cast<float>(HEIGHT) + sprite->getHeight() / 2;
	mYBorderForNextEntity = sprite->getHeight();

	Entity entity = mEntityManager->createPlainEntity(sprite, x, y, 1);

	int movingeTime = static_cast<int>(yDestination - y) * 1000 / mVelocity;
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(entity, x, y, x, yDestination,
		                                                                               movingeTime, false, GEasings::NONE, true);

	if (mSequence[mCurrentIndex] == 1)
		moveable->signal_MovingFinished.connect([&]() { signal_Lost(); });

	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);

	auto collision = mEntityManager->addComponentsToEntity<GCollisionComponent>(entity, sprite->getWidth() / 3.0f, sprite->getHeight() / 3.0f, true);
	bool res = collision->signal_Collisioned.empty();
	if (mSequence[mCurrentIndex] == 1)
		collision->signal_Collisioned.connect(slot_coinColided);
	else if (mSequence[mCurrentIndex] == 2)
		collision->signal_Collisioned.connect([&]() { signal_Lost(); });

	mEntityManager->setChildParentRelations(mGun, entity);
	++mCurrentIndex;
}

void GGameGun::coinColided()
{
	signal_GotCoin();
}