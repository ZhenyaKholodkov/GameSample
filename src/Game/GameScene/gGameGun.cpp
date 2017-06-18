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
		else if (i < lastBagIndex)
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
		mTimeToNextEntity = sprite->getHeight() * 2000 / mVelocity;
	}
	else if (mSequence[mCurrentIndex] == 2)
	{
		sprite = GResManager::Instance()->GetSprite("bag.png");
		mTimeToNextEntity = sprite->getHeight() * 4000 / mVelocity;
	}

	if (!sprite)
		return;
	srand(time(0));
	int rnd = rand() % 3;
	float x = WIDTH / 6 + rnd * WIDTH / 3;
	float y = -sprite->getHeight() / 2;
	float yDestination = HEIGHT + sprite->getHeight() / 2;
	mYBorderForNextEntity = sprite->getHeight();

	Entity entity = mEntityManager->createPlainEntity(sprite, x, y);

	uint32 movingeTime = (yDestination - y) * 1000 / mVelocity;
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(entity, x, y, x, yDestination,
		                                                                               movingeTime, false, GEasings::NONE, true);

	if (mSequence[mCurrentIndex] == 1)
		moveable->signal_MovingFinished.connect([&]() { signal_Lost(); });

	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);

	auto collision = mEntityManager->addComponentsToEntity<GCollisionComponent>(entity, sprite->getWidth() / 3, sprite->getHeight() / 3, true);
	if (mSequence[mCurrentIndex] == 1)
		collision->signal_Collisioned.connect([&]() { signal_GotCoin(); });
	else if (mSequence[mCurrentIndex] == 2)
		collision->signal_Collisioned.connect([&]() { signal_Lost(); });

	mEntityManager->setChildParentRelations(mGun, entity);
	++mCurrentIndex;
}
