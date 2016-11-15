#include "GGame.h"
#include "GResManager.h"
#include "IGRender.h"
#include "GSprite.h"


GGame::GGame():
	mSystemManager(nullptr)
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

	GEntityManager* em = GEntityManager::Instance();
	mSystemManager = GSystemManager::Instatnce();
	mSystemManager->RegisterSystem<GRenderSystem>();
	mSystemManager->RegisterSystem<GAnimationSystem>();
	mSystemManager->RegisterSystem<GUserInputSystem>();

	/////////////////////////////////////////////////////////////////////////////////
	//GSprite* sprite1 = GResManager::Instance()->GetSprite("frame1.png");
	//GSprite* sprite2 = GResManager::Instance()->GetSprite("frame2.png");
	//GSprite* sprite3 = GResManager::Instance()->GetSprite("frame3.png");
	//GSprite* sprite4 = GResManager::Instance()->GetSprite("frame4.png");
	//GSprite* sprite5 = GResManager::Instance()->GetSprite("frame5.png");
	//Entity animationEntity = em->CreateEntity();
	//em->AddComponentsToEntity<GLocationComponent>(animationEntity, 500.0f, 100.0f);
	//em->AddComponentsToEntity<GRenderableComponent>(animationEntity, sprite1);
	//
	//GAnimationComponent* animation = em->AddComponentsToEntity<GAnimationComponent>(animationEntity, 1000 / 60, true);
	//animation->AddFrame(sprite1);
	//animation->AddFrame(sprite2);
	//animation->AddFrame(sprite3);
	//animation->AddFrame(sprite4);
	//animation->AddFrame(sprite5);
	//
	////////////////////////////////Animation/////////////////////////////////////////////////
	GSprite* sprite01 = GResManager::Instance()->GetSprite("01.png");
	GSprite* sprite02 = GResManager::Instance()->GetSprite("02.png");
	GSprite* sprite03 = GResManager::Instance()->GetSprite("03.png");
	GSprite* sprite04 = GResManager::Instance()->GetSprite("04.png");
	GSprite* sprite05 = GResManager::Instance()->GetSprite("05.png");
	GSprite* sprite06 = GResManager::Instance()->GetSprite("06.png");
	GSprite* sprite07 = GResManager::Instance()->GetSprite("07.png");
	GSprite* sprite08 = GResManager::Instance()->GetSprite("08.png");
	GSprite* sprite09 = GResManager::Instance()->GetSprite("09.png");
	Entity animation2Entity = em->CreateEntity();
	em->AddComponentsToEntity<GLocationComponent>(animation2Entity, 200.0f, 300.0f);
	em->AddComponentsToEntity<GRenderableComponent>(animation2Entity, sprite01);
	em->AddComponentsToEntity<GActionComponent>(animation2Entity);

	GAnimationComponent* animation2 = em->AddComponentsToEntity<GAnimationComponent>(animation2Entity, 1000 / 30, true);

	animation2->AddFrame(sprite01);
	animation2->AddFrame(sprite02);
	animation2->AddFrame(sprite03);
	animation2->AddFrame(sprite04);
	animation2->AddFrame(sprite05);
	animation2->AddFrame(sprite06);
	animation2->AddFrame(sprite07);
	animation2->AddFrame(sprite08);
	animation2->AddFrame(sprite09);

	///////////////////////////////////Button//////////////////////////////////////////////

	GSprite* spriteButtonDown = GResManager::Instance()->GetSprite("button1.png");
	GSprite* spriteButtonUp = GResManager::Instance()->GetSprite("button1_down.png");

	Entity buttonEntity = em->CreateEntity();
	em->AddComponentsToEntity<GLocationComponent>(buttonEntity, 600.0f, 300.0f);
	em->AddComponentsToEntity<GRenderableComponent>(buttonEntity, spriteButtonUp);

	GMouseDownEventComponent* buttonDownEvent = em->AddComponentsToEntity<GMouseDownEventComponent>(buttonEntity, spriteButtonDown);
	GMouseUpEventComponent* buttonUpEvent = em->AddComponentsToEntity<GMouseUpEventComponent>(buttonEntity, spriteButtonUp);

	buttonDownEvent->SetParamsToNotify(animation2Entity, ACTIONS::ACTION_BEGIN);

}

void GGame::Update(int dt)
{
	mSystemManager->update(dt);
}

void GGame::LoadResources()
{
	char* buf1 = "qwerty";
	uint32 hash1 = (uint32)(((size_t)buf1) >> 2);
	int i = 9;
	char* buf2 = "qwerty";
	uint32 hash2 = (uint32)(((size_t)buf2) >> 2);

	GResManager* resManager = GResManager::Instance();
	resManager->LoadResources("data/resources/scene1/res_config.xml");
	GSprite* sprite = GResManager::Instance()->GetSprite("button1.png");
	sprite->load();
	/*
	Texture* redButtonResFile = new Texture();
	redButtonResFile->mName = "Button.png";
	redButtonResFile->mPath = "data/resources";
	redButtonResFile->mType = ResType::RES_FILE_TEXTURE;
	resManager->Add(redButtonResFile);
	resManager->LoadResource(redButtonResFile->mFileId);
	*/
}

void GGame::OnMouseDown(Pixel mouse_pos)
{
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseDown(mouse_pos);
}

void GGame::OnMouseUp(Pixel mouse_pos)
{
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseUp(mouse_pos);
}