#include "GGame.h"
#include "GResManager.h"
#include "gRenderManager.h"
#include "GSprite.h"
#include "boost/bind.hpp"

#include <time.h>


GGame::GGame():
	GGameWindow(),
	isGameOver(false)
{
	mMainScene = std::make_unique<GMainScene>(mEntityManager);
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
	mSystemManager->registerSystem<GAnimationSystem>(mEntityManager);
	mSystemManager->registerSystem<GUserInputSystem>(mEntityManager);
	mSystemManager->registerSystem<GMoveableAnimationSystem>(mEntityManager);
	mSystemManager->registerSystem<GMoveableSystem>(mEntityManager);
	mSystemManager->registerSystem<GScalableSystem>(mEntityManager);
	mSystemManager->registerSystem<GCollisionSystem>(mEntityManager);
	mSystemManager->registerSystem<GRenderSystem>(mEntityManager);
	mSystemManager->registerSystem<GRotableSystem>(mEntityManager);

	CreateGame();

	return true;
}

void GGame::CreateGame()
{
	// create entity for processing the keyboard keys
	Entity controlsEntity = mEntityManager->createEntity();
	GKeyUpEventComponent* keyUpComponent = mEntityManager->addComponentsToEntity<GKeyUpEventComponent>(controlsEntity);
	GKeyDownEventComponent* keyDownComponent = mEntityManager->addComponentsToEntity<GKeyDownEventComponent>(controlsEntity);


	mMainScene->start();
	
	// create plater entity
	/*GSprite* spritePlayer = GResManager::Instance()->GetSprite("player.png");
	Entity playerEntity = mEntityManager->createEntity();
	GLocationComponent* playerLocation = mEntityManager->addComponentsToEntity<GLocationComponent>(playerEntity, 960.0f, 704.0f);
	mEntityManager->addComponentsToEntity<GRenderableComponent>(playerEntity, spritePlayer);
	GMoveableComponent* playerMoveable = mEntityManager->addComponentsToEntity<GMoveableComponent>(playerEntity, 5.0f, 5.0f);
	GCollisionComponent* playerCollision = mEntityManager->addComponentsToEntity<GCollisionComponent>(playerEntity, spritePlayer->getWidth() / 2, spritePlayer->getHeight() / 2);
	
	playerCollision->signal_Collisioned.connect(playerLocation->slot_LocationRestoreToLast);
	keyDownComponent->signal_KeyRight.connect(playerMoveable->slot_MoveDx);
	keyDownComponent->signal_KeyLeft.connect(playerMoveable->slot_MoveDxRevert);
	keyDownComponent->signal_KeyUp.connect(playerMoveable->slot_MoveDyRevert);
	keyDownComponent->signal_KeyDown.connect(playerMoveable->slot_MoveDy);
	keyUpComponent->signal_KeyRight.connect(playerMoveable->slot_Stop);
	keyUpComponent->signal_KeyLeft.connect(playerMoveable->slot_Stop);
	keyUpComponent->signal_KeyUp.connect(playerMoveable->slot_Stop);
	keyUpComponent->signal_KeyDown.connect(playerMoveable->slot_Stop);
	
	playerMoveable->signal_Moved.connect(playerCollision->slot_CheckCollision);
	
	playerMoveable->signal_LocationChanged.connect(playerLocation->slot_LocationChangedWithDxDy);*/
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