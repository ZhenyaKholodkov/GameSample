#ifndef G_GAME_GUN
#define G_GAME_GUN

#include <memory>
#include <vector>
#include <queue>

#include "gEntityManager.h"
#include "gGameEntityGroup.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"


class GGameGun
{
public:
	GGameGun(std::shared_ptr<GEntityManager> manager);
	~GGameGun();
public:
	void start();
	void update(int dt);
	void unload();

	uint32 getLines() const { return mLines; }
	void setLines(uint32 lines)  {  mLines = lines; }

	uint32 getSetCount() const { return  mSetCount; }
	void setSetCount(uint32 count) { mSetCount = count; }

	uint32 getVelocity() const { return  mVelocity; }
	void setVelocity(uint32 velocity) { mVelocity = velocity; }

	uint32 getDefaultSpaceCount() const { return  mDefaultSpaceCount; }
	void setDefaultSpaceCount(uint32 count) { mDefaultSpaceCount = count; }

	uint32 getDefaultCoinCount() const { return   mDefaultCoinCount; }
	void setDefaultCoinCount(uint32 count) { mDefaultCoinCount = count; }

	uint32 getDefaultBagCount() const { return  mDefaultBagCount; }
	void setDefaultBagCount(uint32 count) { mDefaultBagCount = count; }

	uint32 getLimitSpaceCount() const { return mLimitSpaceCount; }
	void setLimitSpaceCount(uint32 count) { mLimitSpaceCount = count; }

	uint32 getLimitCoinCount() const { return  mLimitCoinCount; }
	void setLimitCoinCount(uint32 count) { mLimitCoinCount = count; }

	uint32 getLimitBagCount() const { return mLimitBagCount; }
	void setLimitBagCount(uint32 count) { mLimitBagCount = count; }

	int    getChangeSpaceCount() const { return mChangeSpaceCount; }
	void setChangeSpaceCount(int count) { mChangeSpaceCount = count; }

	int    getChangeCoinCount() const { return mChangeCoinCount; }
	void setChangeCoinCount(int count) { mChangeCoinCount = count; }

	int    getChangeBagCount() const { return  mChangeBagCount; }
	void setChangeBagCount(int count) { mChangeBagCount = count; }
private:
	void generate();
	void createNextEntity();
public:/*signals*/
	boost::signals2::signal<void()> signal_Lost;
	boost::signals2::signal<void()> signal_GotCoin;
private:
	std::shared_ptr<GEntityManager> mEntityManager;

	std::vector<uint32> mSequence;

	uint32 mLines;
	uint32 mSetCount;
	uint32 mVelocity;

	uint32 mDefaultSpaceCount;
	uint32 mDefaultCoinCount;
	uint32 mDefaultBagCount;

	uint32 mSpaceCount;
	uint32 mCoinCount;
	uint32 mBagCount;
	uint32 mLimitSpaceCount;
	uint32 mLimitCoinCount;
	uint32 mLimitBagCount;
	int    mChangeSpaceCount;
	int    mChangeCoinCount;
	int    mChangeBagCount;

	uint32 mCurrentIndex;

	float mYBorderForNextEntity;

	int mTimeToNextEntity;
	int mCurrentTimeToNextEntity;

	Entity mGun;
};
#endif// G_GAME_GUN