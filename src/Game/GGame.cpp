#include "GGame.h"
#include "GResManager.h"
#include "gRenderManager.h"
#include "GSprite.h"

#include <time.h>

GGame::GGame():
	mSystemManager(nullptr),
	mEntityManager(nullptr),
	isGameOver(false)
{
}

GGame::~GGame()
{

}

IGame* IGame::Instane()
{
	static IGame* instance = new GGame();
	return instance;
}

void GGame::Create()
{
	LoadResources();

	mEntityManager = GEntityManager::Instance();
	mSystemManager = GSystemManager::Instatnce();
	mSystemManager->RegisterSystem<GAnimationSystem>();
	mSystemManager->RegisterSystem<GUserInputSystem>();
	mSystemManager->RegisterSystem<GMoveableAnimationSystem>();
	mSystemManager->RegisterSystem<GMoveableSystem>();
	mSystemManager->RegisterSystem<GScalableSystem>();	
	mSystemManager->RegisterSystem<GCollisionSystem>();
	mSystemManager->RegisterSystem<GRenderSystem>();

	CreateGame();


}

void GGame::CreateGame()
{
	CreateField();
	mEntityManager = GEntityManager::Instance();

	Entity controlsEntity = mEntityManager->CreateEntity();
	GKeyUpEventComponent* keyUpComponent = mEntityManager->AddComponentsToEntity<GKeyUpEventComponent>(controlsEntity);
	GKeyDownEventComponent* keyDownComponent = mEntityManager->AddComponentsToEntity<GKeyDownEventComponent>(controlsEntity);


	GSprite* spritePlayer = GResManager::Instance()->GetSprite("player.png");
	Entity playerEntity = mEntityManager->CreateEntity();
	GLocationComponent* playerLocation = mEntityManager->AddComponentsToEntity<GLocationComponent>(playerEntity, 200.0f, 200.0f);
	mEntityManager->AddComponentsToEntity<GRenderableComponent>(playerEntity, spritePlayer);
	GMoveableComponent* playerMoveable = mEntityManager->AddComponentsToEntity<GMoveableComponent>(playerEntity, 5.0f, 5.0f);
	GCollisionComponent* playerCollision = mEntityManager->AddComponentsToEntity<GCollisionComponent>(playerEntity, spritePlayer->GetWidth() / 2, spritePlayer->GetHeight() / 2);

	playerCollision->signal_Collisioned.connect(playerLocation, &GLocationComponent::slot_LocationRestoreToLast);
	keyDownComponent->signal_KeyRight.connect(playerMoveable, &GMoveableComponent::slot_MoveDx);
	keyDownComponent->signal_KeyLeft.connect(playerMoveable, &GMoveableComponent::slot_MoveDxRevert);
	keyDownComponent->signal_KeyUp.connect(playerMoveable, &GMoveableComponent::slot_MoveDyRevert);
	keyDownComponent->signal_KeyDown.connect(playerMoveable, &GMoveableComponent::slot_MoveDy);
	keyUpComponent->signal_KeyRight.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyLeft.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyUp.connect(playerMoveable, &GMoveableComponent::slot_Stop);
	keyUpComponent->signal_KeyDown.connect(playerMoveable, &GMoveableComponent::slot_Stop);

	GCollisionSystem* collisionSystem = (GCollisionSystem*)mSystemManager->GetSystem<GCollisionSystem>();

	playerMoveable->signal_Moved.connect(collisionSystem, &GCollisionSystem::slot_CheckCollision);

	playerMoveable->signal_LocationChanged.connect(playerLocation, &GLocationComponent::slot_LocationChangedWithDxDy);

}

void GGame::CreateField()
{
	GSprite* spriteBrick = GResManager::Instance()->GetSprite("stones.png");
	GSprite* spriteGrass = GResManager::Instance()->GetSprite("Grass.png");
	uint32 size = spriteGrass->GetWidth();

	const uint32 rows = 6, cols = 8;
	uint32 field[rows][cols] = {1, 1, 1, 1, 1, 1, 1, 1,
		                  0, 0, 0, 0, 0, 0, 0, 1,
		                  0, 0, 0, 0, 0, 1, 0, 1,
		                  0, 0, 0, 0, 3, 1, 2, 1,
		                  0, 1, 1, 1, 1, 1, 0, 1,
		                  0, 0, 0, 0, 0, 0, 0, 0};

	float x = size / 2;
	float y = size / 2;
	for (uint32 i = 0; i < rows; ++i)
	{
		x = size / 2;
		for (uint32 j = 0; j < cols; ++j)
		{
			Entity entity = mEntityManager->CreateEntity();
			mEntityManager->AddComponentsToEntity<GLocationComponent>(entity, x, y);
			if (field[i][j] == 1)
			{
				mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteBrick);
				mEntityManager->AddComponentsToEntity<GCollisionComponent>(entity, spriteBrick->GetWidth() / 2, spriteBrick->GetHeight() / 2);

			}
			else
			{
				mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteGrass);
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
	Entity entity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(entity, x, y);
	GRenderableComponent* renderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteBomb);
	GCollisionComponent* collision = mEntityManager->AddComponentsToEntity<GCollisionComponent>(entity, spriteBomb->GetWidth() / 2, spriteBomb->GetHeight() / 2);

	GSprite* sprite0 = GResManager::Instance()->GetSprite("0.png");
	GSprite* sprite1 = GResManager::Instance()->GetSprite("1.png");
	GSprite* sprite2 = GResManager::Instance()->GetSprite("2.png");
	GSprite* sprite3 = GResManager::Instance()->GetSprite("3.png");
	GSprite* sprite4 = GResManager::Instance()->GetSprite("4.png");
	GSprite* sprite5 = GResManager::Instance()->GetSprite("5.png");
	GSprite* sprite6 = GResManager::Instance()->GetSprite("6.png");
	GSprite* sprite7 = GResManager::Instance()->GetSprite("7.png");
	GSprite* sprite8 = GResManager::Instance()->GetSprite("8.png");
	Entity animationEntity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(animationEntity, x, y);
	GRenderableComponent* animationRender = mEntityManager->AddComponentsToEntity<GRenderableComponent>(animationEntity, sprite0);
	animationRender->setVisible(false);
	GAnimationComponent* animationComponent = mEntityManager->AddComponentsToEntity<GAnimationComponent>(animationEntity, 1000 / 60, false);
	
	animationComponent->AddFrame(sprite0);
	animationComponent->AddFrame(sprite1);
	animationComponent->AddFrame(sprite2);
	animationComponent->AddFrame(sprite3);
	animationComponent->AddFrame(sprite4);
	animationComponent->AddFrame(sprite5);
	animationComponent->AddFrame(sprite6);
	animationComponent->AddFrame(sprite7);
	animationComponent->AddFrame(sprite8);

	collision->signal_Collisioned.connect(animationComponent, &GAnimationComponent::slot_RunAnimation);
	collision->signal_Collisioned.connect(animationRender, &GRenderableComponent::slot_SetVisible);
	animationComponent->signal_AnimationFinished.connect(this, &GGame::slot_Lost);
	animationComponent->signal_AnimationFinished.connect(renderable, &GRenderableComponent::slot_SetInvisible);
	animationComponent->signal_AnimationFinished.connect(animationRender, &GRenderableComponent::slot_SetInvisible);
}

void GGame::CreateCoin(float x, float y)
{
	GSprite* spriteCoin = GResManager::Instance()->GetSprite("coin.png");
	Entity entity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(entity, x, y);
	GRenderableComponent* renderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteCoin);
	GCollisionComponent* collision = mEntityManager->AddComponentsToEntity<GCollisionComponent>(entity, spriteCoin->GetWidth() / 2, spriteCoin->GetHeight() / 2);
	GScalableComponent* scalable = mEntityManager->AddComponentsToEntity<GScalableComponent>(entity, 1.0f, 1.0f, 0.0f, 0.0f, 500);

	collision->signal_Collisioned.connect(scalable, &GScalableComponent::slot_Scale);
	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->signal_ScaleChangingFinished.connect(this, &GGame::slot_Won);
}

void GGame::slot_Won()
{
	GSprite* spriteWon = GResManager::Instance()->GetSprite("won.png");
	Entity entity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(entity, 512, 384);
	GRenderableComponent* renderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteWon);
	GScalableComponent* scalable = mEntityManager->AddComponentsToEntity<GScalableComponent>(entity, 0.0f, 0.0f, 1.0f, 1.0f, 500);

	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->slot_Scale();
	isGameOver = true;
}

void GGame::slot_Lost()
{
	GSprite* spriteWon = GResManager::Instance()->GetSprite("over.png");
	Entity entity = mEntityManager->CreateEntity();
	mEntityManager->AddComponentsToEntity<GLocationComponent>(entity, 512, 384);
	GRenderableComponent* renderable = mEntityManager->AddComponentsToEntity<GRenderableComponent>(entity, spriteWon);
	GScalableComponent* scalable = mEntityManager->AddComponentsToEntity<GScalableComponent>(entity, 0.0f, 0.0f, 1.0f, 1.0f, 500);

	scalable->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);
	scalable->slot_Scale();
	isGameOver = true;
}

void GGame::Update(int dt)
{
	mSystemManager->update(dt);
}

void GGame::LoadResources()
{
	GResManager* resManager = GResManager::Instance();
	resManager->LoadResources("data/resources/textures/res_config.xml");
}

void GGame::OnMouseDown(GCursor point)
{
	if (isGameOver)
		return;
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseDown(point);
}

void GGame::OnMouseUp(GCursor point)
{
	if (isGameOver)
		return;
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseUp(point);
}

void GGame::OnMouseMove(GCursor point)
{
	if (isGameOver)
		return;
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseMove(point);
}

void GGame::keyUp(GKey key)
{
	if (isGameOver)
		return;
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnKeyUp(key);
}

void GGame::keyDown(GKey key)
{
	if (isGameOver)
		return;
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnKeyDown(key);
}