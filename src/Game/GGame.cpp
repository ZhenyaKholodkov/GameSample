#include "GGame.h"
#include "GResManager.h"
#include "gRenderManager.h"
#include "GSprite.h"

#include <time.h>

GGame::GGame():
	GGameWindow(),
	isGameOver(false)
{
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

	CreateGame();

	return true;
}

void GGame::CreateGame()
{
	CreateField();

	// create entity for processing the keyboard keys
	Entity controlsEntity = mEntityManager->createEntity();
	GKeyUpEventComponent* keyUpComponent = mEntityManager->addComponentsToEntity<GKeyUpEventComponent>(controlsEntity);
	GKeyDownEventComponent* keyDownComponent = mEntityManager->addComponentsToEntity<GKeyDownEventComponent>(controlsEntity);

	// create plater entity
	GSprite* spritePlayer = GResManager::Instance()->GetSprite("player.png");
	Entity playerEntity = mEntityManager->createEntity();
	GLocationComponent* playerLocation = mEntityManager->addComponentsToEntity<GLocationComponent>(playerEntity, 960.0f, 704.0f);
	mEntityManager->addComponentsToEntity<GRenderableComponent>(playerEntity, spritePlayer);
	GMoveableComponent* playerMoveable = mEntityManager->addComponentsToEntity<GMoveableComponent>(playerEntity, 5.0f, 5.0f);
	GCollisionComponent* playerCollision = mEntityManager->addComponentsToEntity<GCollisionComponent>(playerEntity, spritePlayer->getWidth() / 2, spritePlayer->getHeight() / 2);

	playerCollision->signal_Collisioned.connect(playerLocation, &GLocationComponent::slot_LocationRestoreToLast);
	keyDownComponent->signal_KeyRight.connect(playerMoveable, &GMoveableComponent::slot_MoveDx);
	keyDownComponent->signal_KeyLeft.connect(playerMoveable, &GMoveableComponent::slot_MoveDxRevert);
	keyDownComponent->signal_KeyUp.connect(playerMoveable, &GMoveableComponent::slot_MoveDyRevert);
	keyDownComponent->signal_KeyDown.connect(playerMoveable, &GMoveableComponent::slot_MoveDy);
	keyUpComponent->signal_KeyRight.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyLeft.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyUp.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyDown.connect(playerMoveable, &GMoveableComponent::slot_Stop);

	std:shared_ptr<GCollisionSystem> collisionSystem = std::static_pointer_cast<GCollisionSystem>(mSystemManager->getSystem<GCollisionSystem>());

	playerMoveable->signal_Moved.connect(collisionSystem.get(), &GCollisionSystem::slot_CheckCollision);

	playerMoveable->signal_LocationChanged.connect(playerLocation, &GLocationComponent::slot_LocationChangedWithDxDy);

}

void GGame::CreateField()
{
	GSprite* spriteBrick = GResManager::Instance()->GetSprite("stones.png");
	GSprite* spriteGrass = GResManager::Instance()->GetSprite("Grass.png");
	int size = spriteGrass->getWidth();

	const uint32 rows = 6, cols = 8;
	uint32 field[rows][cols] = {1, 1, 1, 1, 1, 1, 1, 1,
		                  0, 0, 0, 0, 0, 0, 0, 1,
		                  0, 0, 0, 0, 0, 1, 0, 1,
		                  0, 0, 0, 0, 3, 1, 2, 1,
		                  0, 1, 1, 1, 1, 1, 0, 1,
		                  0, 0, 0, 0, 0, 0, 0, 0};

	// 0 - grass, 1 - stone wall , 2 - bomb on the grass, 3 - coin on the grass

	float x = (float)size / 2;
	float y = (float)size / 2;
	for (uint32 i = 0; i < rows; ++i)
	{
		x = (float)size / 2;
		for (uint32 j = 0; j < cols; ++j)
		{
			Entity entity = mEntityManager->createEntity();
			mEntityManager->addComponentsToEntity<GLocationComponent>(entity, x, y);
			if (field[i][j] == 1)
			{
				mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteBrick);
				mEntityManager->addComponentsToEntity<GCollisionComponent>(entity, spriteBrick->getWidth() / 2, spriteBrick->getHeight() / 2);

			}
			else
			{
				mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteGrass);
				if (field[i][j] == 2)
				{
					CreateBomb(x, y);
				}
				else if (field[i][j] == 3)
				{
					CreateCoin(x, y);
				}
			}
			x += size;
		}
		y += size;
	}
}

void GGame::CreateBomb(float x, float y)
{
	GSprite* spriteBomb = GResManager::Instance()->GetSprite("Bomb.png");
	Entity entity = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(entity, x, y);
	GRenderableComponent* renderable = mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteBomb);
	GCollisionComponent* collision = mEntityManager->addComponentsToEntity<GCollisionComponent>(entity, spriteBomb->getWidth() / 2, spriteBomb->getHeight() / 2);

	GSprite* sprite0 = GResManager::Instance()->GetSprite("0.png");
	GSprite* sprite1 = GResManager::Instance()->GetSprite("1.png");
	GSprite* sprite2 = GResManager::Instance()->GetSprite("2.png");
	GSprite* sprite3 = GResManager::Instance()->GetSprite("3.png");
	GSprite* sprite4 = GResManager::Instance()->GetSprite("4.png");
	GSprite* sprite5 = GResManager::Instance()->GetSprite("5.png");
	GSprite* sprite6 = GResManager::Instance()->GetSprite("6.png");
	GSprite* sprite7 = GResManager::Instance()->GetSprite("7.png");
	GSprite* sprite8 = GResManager::Instance()->GetSprite("8.png");
	Entity animationEntity = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(animationEntity, x, y);
	GAnimationComponent* animationComponent = mEntityManager->addComponentsToEntity<GAnimationComponent>(animationEntity, 1000 / 60, false);

	GRenderableComponent* animationRender = mEntityManager->addComponentsToEntity<GRenderableComponent>(animationEntity, sprite0);
	animationRender->setVisible(false);
	animationComponent->AddFrame(sprite0);
	animationComponent->AddFrame(sprite1);
	animationComponent->AddFrame(sprite2);
	animationComponent->AddFrame(sprite3);
	animationComponent->AddFrame(sprite4);
	animationComponent->AddFrame(sprite5);
	animationComponent->AddFrame(sprite6);
	animationComponent->AddFrame(sprite7);
	animationComponent->AddFrame(sprite8);

	collision->signal_Collisioned.connect(animationComponent, &GAnimationComponent::slot_RunAnimation); // run animation after the signal about the collision
	collision->signal_Collisioned.connect(animationRender, &GRenderableComponent::slot_SetVisible);   
	animationComponent->signal_AnimationFinished.connect(this, &GGame::slot_Lost);                     // after animation the game will be over
	animationComponent->signal_AnimationFinished.connect(renderable, &GRenderableComponent::slot_SetInvisible);
	animationComponent->signal_AnimationFinished.connect(animationRender, &GRenderableComponent::slot_SetInvisible);
}

void GGame::CreateCoin(float x, float y)
{
	GSprite* spriteCoin = GResManager::Instance()->GetSprite("coin.png");
	Entity entity = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(entity, x, y);
	GRenderableComponent* renderable = mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteCoin);
	GCollisionComponent* collision = mEntityManager->addComponentsToEntity<GCollisionComponent>(entity, spriteCoin->getWidth() / 2, spriteCoin->getHeight() / 2);
	GScalableComponent* scalable = mEntityManager->addComponentsToEntity<GScalableComponent>(entity, 1.0f, 1.0f, 0.0f, 0.0f, 500);

	collision->signal_Collisioned.connect(scalable, &GScalableComponent::slot_Scale);       // scales the coin after the collision signal 
	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->signal_ScaleChangingFinished.connect(this, &GGame::slot_Won);                // and game will be won after scaling
}

void GGame::slot_Won()
{
	GSprite* spriteWon = GResManager::Instance()->GetSprite("won.png");
	Entity entity = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(entity, 512, 384);
	GRenderableComponent* renderable = mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteWon);
	GScalableComponent* scalable = mEntityManager->addComponentsToEntity<GScalableComponent>(entity, 0.0f, 0.0f, 1.0f, 1.0f, 500);

	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->slot_Scale();
	isGameOver = true;
}

void GGame::slot_Lost()
{
	GSprite* spriteWon = GResManager::Instance()->GetSprite("over.png");
	Entity entity = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(entity, 512, 384);
	GRenderableComponent* renderable = mEntityManager->addComponentsToEntity<GRenderableComponent>(entity, spriteWon);
	GScalableComponent* scalable = mEntityManager->addComponentsToEntity<GScalableComponent>(entity, 0.0f, 0.0f, 1.0f, 1.0f, 500);

	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->slot_Scale();
	isGameOver = true;
}

void GGame::Update(int dt)
{
	mSystemManager->update(dt);
}

bool GGame::LoadResources()
{
	GLog::printLog("LoadResources()\n");
	GResManager* resManager = GResManager::Instance();
	return resManager->LoadResources("resources/textures/res_config.xml"); // load resources
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
	destroyGLContext();
}

void GGame::onTimer(int32 dt)
{
	mSystemManager->update(dt);
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