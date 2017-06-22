
#ifndef GGAME_H
#define GGAME_H

#include "GGameWindow.h"
#include "gMainScene.h"
#include "gGameScene.h"

#include "boost/signals2.hpp"

class GGame: public GGameWindow
{
public:
	GGame();
	~GGame();

	virtual bool Create();
	virtual void onTimer(int);

	virtual int32  onCreate();
	virtual void   onClose();

	bool LoadResources();
public:/*slots*/
public:/*signals*/
	boost::signals2::signal<void()> signals_GamePause;
	boost::signals2::signal<void()> signals_GameContinue;
private:
	void CreateGame();
private:
	bool            isGameOver;
	bool            mIsPause;
	bool mSwitchNextScreen;

	std::unique_ptr<GMainScene> mMainScene;
	std::unique_ptr<GGameScene> mGameScene;

	std::vector<boost::signals2::connection> mConnections;
};

#endif

