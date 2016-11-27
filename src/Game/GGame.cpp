#include "GGame.h"
#include "GResManager.h"
#include "gRenderManager.h"
#include "GSprite.h"

#include <time.h>

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

	int i1 = GComponent<GLocationComponent>::GetComponentId();
	int i2 = GComponent<GRenderableComponent>::GetComponentId();
	int i3 = GComponent<GAnimationComponent>::GetComponentId();
	int i5 = GComponent<GMouseDownEventComponent>::GetComponentId();
	int i6 = GComponent<GMouseUpEventComponent>::GetComponentId();


	size_t size1 = sizeof(GLocationComponent);
	size_t size2 = sizeof(GRenderableComponent);
	size_t size3 = sizeof(GAnimationComponent);
	size_t size4 = sizeof(GMouseDownEventComponent);
	size_t size5 = sizeof(GMouseUpEventComponent);

	size_t size6 = sizeof(sigslot::has_slots<>);

	GEntityManager* em = GEntityManager::Instance();
	mSystemManager = GSystemManager::Instatnce();
	mSystemManager->RegisterSystem<GRenderSystem>();
	mSystemManager->RegisterSystem<GAnimationSystem>();
	mSystemManager->RegisterSystem<GUserInputSystem>();
	mSystemManager->RegisterSystem<GMoveableSystem>();
	mSystemManager->RegisterSystem<GScalableSystem>();

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
	GLocationComponent* anim2Location = em->AddComponentsToEntity<GLocationComponent>(animation2Entity, 100.0f, 0.0f);
	anim2Location->setZ(-1.0f);
	em->AddComponentsToEntity<GRenderableComponent>(animation2Entity, sprite01);

	GAnimationComponent* animation2 = em->AddComponentsToEntity<GAnimationComponent>(animation2Entity, 1000 / 60, false);

	animation2->AddFrame(sprite01);
	animation2->AddFrame(sprite02);
	animation2->AddFrame(sprite03);
	animation2->AddFrame(sprite04);
	animation2->AddFrame(sprite05);
	animation2->AddFrame(sprite06);
	animation2->AddFrame(sprite07);
	animation2->AddFrame(sprite08);
	animation2->AddFrame(sprite09);

	///////////////////////////////////Moveable////////////////////////////////////////////
	GSprite* spriteMove = GResManager::Instance()->GetSprite("frame1.png");
	Entity animationEntity = em->CreateEntity();
	em->AddComponentsToEntity<GLocationComponent>(animationEntity, 100.0f, 100.0f);
	em->AddComponentsToEntity<GRenderableComponent>(animationEntity, spriteMove);
	GMoveableComponent* moveable = em->AddComponentsToEntity<GMoveableComponent>(animationEntity, 500.f, 200.0f, 1000);

	///////////////////////////////////Button//////////////////////////////////////////////

	GSprite* spriteButtonDown = GResManager::Instance()->GetSprite("button1.png");
	GSprite* spriteButtonUp = GResManager::Instance()->GetSprite("button1_down.png");

	Entity buttonEntity = em->CreateEntity();
	em->AddComponentsToEntity<GLocationComponent>(buttonEntity, 600.0f, 300.0f);
	GRenderableComponent* renderable = em->AddComponentsToEntity<GRenderableComponent>(buttonEntity, spriteButtonUp);
	GScalableComponent* scalble = em->AddComponentsToEntity<GScalableComponent>(buttonEntity, 1.0f, 1.0f, 0.0f, 0.0f, 1000);
	scalble->signal_ScaleChanged.connect(renderable, &GRenderableComponent::slot_ChangeScale);

	GMouseDownEventComponent* buttonDownEvent = em->AddComponentsToEntity<GMouseDownEventComponent>(buttonEntity, spriteButtonDown);
	GMouseUpEventComponent* buttonUpEvent = em->AddComponentsToEntity<GMouseUpEventComponent>(buttonEntity, spriteButtonUp);
	
	buttonDownEvent->signal_MouseDownNewSprite.connect(renderable, &GRenderableComponent::slot_ChangeSprite);
	buttonUpEvent->signal_MouseUpNewSprite.connect(renderable, &GRenderableComponent::slot_ChangeSprite);
	buttonUpEvent->signal_MouseUp.connect(moveable, &GMoveableComponent::slot_Move);
	///////////////////////////////////Button//////////////////////////////////////////////

	GSprite* sprite2ButtonDown = GResManager::Instance()->GetSprite("btn_2.png");
	GSprite* sprite2ButtonUp = GResManager::Instance()->GetSprite("btn_3.png");
	GSprite* sprite2ButtonMove = GResManager::Instance()->GetSprite("btn_4.png");

	Entity button2Entity = em->CreateEntity();
	em->AddComponentsToEntity<GLocationComponent>(button2Entity, 400.0f, 600.0f);
	GRenderableComponent* renderable2 = em->AddComponentsToEntity<GRenderableComponent>(button2Entity, sprite2ButtonUp);

	GMouseDownEventComponent* button2DownEvent = em->AddComponentsToEntity<GMouseDownEventComponent>(button2Entity, sprite2ButtonDown);
	GMouseUpEventComponent* button2UpEvent = em->AddComponentsToEntity<GMouseUpEventComponent>(button2Entity, sprite2ButtonUp);
	GMouseMoveEventComponent* button2MoveEvent = em->AddComponentsToEntity<GMouseMoveEventComponent>(button2Entity, sprite2ButtonMove, sprite2ButtonUp);

	button2DownEvent->signal_MouseDown.connect(animation2, &GAnimationComponent::slot_RunAnimation);
	button2DownEvent->signal_MouseDownNewSprite.connect(renderable2, &GRenderableComponent::slot_ChangeSprite);
	button2UpEvent->signal_MouseUpNewSprite.connect(renderable2, &GRenderableComponent::slot_ChangeSprite);
	button2MoveEvent->signal_MouseMovedInEntity.connect(renderable2, &GRenderableComponent::slot_ChangeSprite);
	button2MoveEvent->signal_MouseMovedOutEntity.connect(renderable2, &GRenderableComponent::slot_ChangeSprite);

	button2UpEvent->signal_MouseUp.connect(scalble, &GScalableComponent::slot_Scale);
	///////////////////////child - parent Test////////////////
	em->setChildParentRelations(button2Entity, animation2Entity);
	//em->removeParent(animation2Entity);
	////////////////test/////////////////

	//for (int i = 0; i < 100; i++)
	//{
	//	srand(time(0));
	//	float randX = (float)(rand() % 1000 + 100);
	//	float randY = (float)(rand() % 800 + 80);
	//	GSprite* spriteMove = GResManager::Instance()->GetSprite("frame1.png");
	//	Entity animationEntity = em->CreateEntity();
	//	em->AddComponentsToEntity<GLocationComponent>(animationEntity, randX, randY);
	//	em->AddComponentsToEntity<GRenderableComponent>(animationEntity, spriteMove);
	//	GMoveableComponent* moveable = em->AddComponentsToEntity<GMoveableComponent>(animationEntity, 500.f, 200.0f, 1000);
	//
	//	GMouseDownEventComponent* button2DownEvent = em->AddComponentsToEntity<GMouseDownEventComponent>(button2Entity, spriteMove);
	//	GMouseUpEventComponent* button2UpEvent = em->AddComponentsToEntity<GMouseUpEventComponent>(button2Entity, spriteMove);
	//
	//}
	////////////////test end///////////////////////////////
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
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseDown(point);
}

void GGame::OnMouseUp(GCursor point)
{
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseUp(point);
}

void GGame::OnMouseMove(GCursor point)
{
	GUserInputSystem* inputSystem = static_cast<GUserInputSystem*>(mSystemManager->GetSystem<GUserInputSystem>());
	inputSystem->OnMouseMove(point);
}