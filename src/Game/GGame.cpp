#include "GGame.h"
#include "GResManager.h"
#include "gRenderManager.h"
#include "GSprite.h"
#include "boost/bind.hpp"

#include <time.h>


GGame::GGame():
	GGameWindow(),
	isGameOver(false),
	mIsPause(false),
	mSwitchNextScreen(false)
{
	mMainScene = std::make_unique<GMainScene>(mEntityManager);
	mGameScene = std::make_unique<GGameScene>(mEntityManager);
}

GGame::~GGame()
{
}


bool GGame::Create()
{
	if (!LoadResources())
	{
		return false;
	}
	mSystemManager->registerSystem<GUserInputSystem>(mEntityManager);
	mSystemManager->registerSystem<GRenderSystem>(mEntityManager);
	auto animSystem = mSystemManager->registerSystem<GAnimationSystem>(mEntityManager);
	auto moveAnimSystem = mSystemManager->registerSystem<GMoveableAnimationSystem>(mEntityManager);
	auto moveSystem = mSystemManager->registerSystem<GMoveableSystem>(mEntityManager);
	auto scaleSystem = mSystemManager->registerSystem<GScalableSystem>(mEntityManager);
	auto collisionSystem = mSystemManager->registerSystem<GCollisionSystem>(mEntityManager);
	auto rotSystem = mSystemManager->registerSystem<GRotableSystem>(mEntityManager);

	signals_GamePause.connect(animSystem->slot_Stop);
	signals_GamePause.connect(moveAnimSystem->slot_Stop);
	signals_GamePause.connect(moveSystem->slot_Stop);
	signals_GamePause.connect(collisionSystem->slot_Stop);
	signals_GamePause.connect(rotSystem->slot_Stop);

	signals_GameContinue.connect(animSystem->slot_Continue);
	signals_GameContinue.connect(moveAnimSystem->slot_Continue);
	signals_GameContinue.connect(moveSystem->slot_Continue);
	signals_GameContinue.connect(collisionSystem->slot_Continue);
	signals_GameContinue.connect(rotSystem->slot_Continue);

	CreateGame();

	return true;
}

void GGame::onTimer(int dt)
{
	GGameWindow::onTimer(dt);
	if (mSwitchNextScreen)
	{
		if (mMainScene->isStarted())
		{
			mMainScene->unload();
			mGameScene->start();
		}
		else if (mGameScene->isStarted())
		{
			mGameScene->unload();
			mMainScene->start();
		}
		mSwitchNextScreen = false;
	}

	if (mGameScene->isStarted())
	{
		mGameScene->update(dt);
	}
}

void GGame::CreateGame()
{
	mGameScene->signal_NextScreen.connect([this] {
		mSwitchNextScreen = true;
		mIsPause = false;
		signals_GameContinue();
	});
	mGameScene->signal_Pause.connect([this] {
		mIsPause = true;
		signals_GamePause();
	});
	mGameScene->signal_Continue.connect([this] {
		mIsPause = false;
		signals_GameContinue();
	});

	mMainScene->signal_NextScreen.connect([this]{
		mSwitchNextScreen = true;
	});
	mMainScene->start();
}

bool GGame::LoadResources()
{
	GLog::printLog("LoadResources()\n");
	GResManager* resManager = GResManager::Instance();
	return resManager->LoadResources("resources/textures/texture_config.xml"); // load resources
}

int32 GGame::onCreate()
{
	if (!Create())
		return 0;
	else
		return 1;
}

void GGame::onClose()
{
	GGameWindow::onClose();
}

void GGame::onLMouseDown(GCursor  point)
{
	if (isGameOver)
		return;
	std::shared_ptr<GUserInputSystem> inputSystem = std::static_pointer_cast<GUserInputSystem>(mSystemManager->getSystem<GUserInputSystem>());
	inputSystem->OnMouseDown(point);
}

void GGame::onLMouseUp(GCursor  point)
{
	if (isGameOver)
		return;
	std::shared_ptr<GUserInputSystem> inputSystem = std::static_pointer_cast<GUserInputSystem>(mSystemManager->getSystem<GUserInputSystem>());
	inputSystem->OnMouseUp(point);
}

void GGame::onMouseMove(GCursor point)
{
	if (isGameOver)
		return;
	std::shared_ptr<GUserInputSystem> inputSystem = std::static_pointer_cast<GUserInputSystem>(mSystemManager->getSystem<GUserInputSystem>());
	inputSystem->OnMouseMove(point);
}

void GGame::onKeyUp(GKey key)
{
	if (isGameOver)
		return;
	std::shared_ptr<GUserInputSystem> inputSystem = std::static_pointer_cast<GUserInputSystem>(mSystemManager->getSystem<GUserInputSystem>());
	inputSystem->OnKeyUp(key);
}

void GGame::onKeyDown(GKey key)
{
	if (isGameOver)
		return;
	std::shared_ptr<GUserInputSystem> inputSystem = std::static_pointer_cast<GUserInputSystem>(mSystemManager->getSystem<GUserInputSystem>());
	inputSystem->OnKeyDown(key);
}