#ifndef G_GAME_SCENE
#define G_GAME_SCENE

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

#include <queue>
#include "gGameExitWindow.h"
#include "gGameReplayWindow.h"
#include "gGameGun.h"

class GGameScene
{
public:
	GGameScene(std::shared_ptr<GEntityManager> entityManager);
	~GGameScene();

	void start();
	void unload();
	void update(int dt);

	bool isStarted() const { return mIsStarted; }
private:
	void createPauseButton();
	void createMill();
	void addCoin();

	void replay();
	void disconnectSignals();
public: /*lots*/
	boost::signals2::signal<void()>::slot_type slot_replay = boost::bind(&GGameScene::replay, this);
	boost::signals2::signal<void()>::slot_type slot_addCoin = boost::bind(&GGameScene::addCoin, this);
public:/*signals*/
	boost::signals2::signal<void()>    signal_NextScreen;
	boost::signals2::signal<void()>    signal_Pause;
	boost::signals2::signal<void()>    signal_Continue;
	boost::signals2::signal<void(int)> signal_ChangeVelocity;
private:
	const std::shared_ptr<GEntityManager> mEntityManager;

	Entity mMill;
	Entity mPauseButton;
	Entity mBackGround;
	Entity mTextScore;
	Entity mControls;

	GGameExitWindow mExitWindow;
	GGameReplayWindow mReplayWindow;
	GGameGun          mGameGun;

	bool mIsStarted;
	bool mPause;

	uint32 mScore;
};
#endif// G_GAME_SCENE