#include "gMainScene.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

void GMainScene::start()
{
	GSprite* spriteBackgroun = GResManager::Instance()->GetSprite("background3.png");
	GSprite* spriteGrass = GResManager::Instance()->GetSprite("grass.png");
	GSprite* spriteMillBase = GResManager::Instance()->GetSprite("mill_color.png");
	GSprite* spriteMillSales = GResManager::Instance()->GetSprite("mill_part2.png");
	GSprite* spriteCloud = GResManager::Instance()->GetSprite("cloud.png");

	mEntityManager->createPlainEntity(spriteBackgroun, WIDTH / 2, HEIGHT / 2);

	Entity millBase = mEntityManager->createPlainEntity(spriteMillBase, WIDTH / 2 - spriteMillBase->getWidth() / 2,
		HEIGHT - spriteGrass->getHeight() - spriteMillBase->getHeight() / 2 + 70.0f);
	Entity millSacles = mEntityManager->createPlainEntity(spriteMillSales, 0.0f, -92.0f);
	mEntityManager->setChildParentRelations(millBase, millSacles);
	mEntityManager->addComponentsToEntity<GRotableComponent>(millSacles, 0, 360.0f, 10000, true)->setState(GRotableComponent::STATE_ROTATE);

	mEntityManager->createPlainEntity(spriteGrass, WIDTH / 2, HEIGHT - spriteGrass->getHeight() / 2);


	Entity cloud = mEntityManager->createPlainEntity(spriteCloud, WIDTH + spriteCloud->getWidth() / 2, HEIGHT / 3);
	auto renderCloud = mEntityManager->getComponent<GRenderableComponent>(cloud);
	auto moveableCloud = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(cloud,
		WIDTH + spriteCloud->getWidth() / 2, HEIGHT / 3, -spriteCloud->getWidth(), HEIGHT / 3, 20000, true);
	moveableCloud->signal_MovingFinished.connect(slot_CloudMovingFinished);
	moveableCloud->setState(GMoveableAnimationComponent::STATE_MOVE);

	createBird();
	createStartButton();
}

void GMainScene::createBird()
{
	GSprite* spriteBird1 = GResManager::Instance()->GetSprite("bird.png");
	GSprite* spriteBird2 = GResManager::Instance()->GetSprite("bird2.png");
	GSprite* spriteBird3 = GResManager::Instance()->GetSprite("bird3.png");
	GSprite* spriteBird4 = GResManager::Instance()->GetSprite("bird4.png");
	GSprite* spriteBird5 = GResManager::Instance()->GetSprite("bird5.png");
	GSprite* spriteBird6 = GResManager::Instance()->GetSprite("bird6.png");
	GSprite* spriteBird7 = GResManager::Instance()->GetSprite("bird7.png");

	Entity bird = mEntityManager->createPlainEntity(spriteBird1, -spriteBird1->getWidth() / 2, HEIGHT / 2);
	auto animation = mEntityManager->addComponentsToEntity<GAnimationComponent>(bird, 60, true);
	animation->AddFrame(spriteBird1);
	animation->AddFrame(spriteBird2);
	animation->AddFrame(spriteBird3);
	animation->AddFrame(spriteBird4);
	animation->AddFrame(spriteBird5);
	animation->AddFrame(spriteBird6);
	animation->AddFrame(spriteBird7);
	animation->setState(GAnimationComponent::STATE_RUN);

	auto moveableBird = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(bird,
		-spriteBird1->getWidth() / 2, HEIGHT / 2,
		WIDTH + spriteBird1->getWidth() / 2, HEIGHT / 2, 10000, true);
	moveableBird->setState(GMoveableAnimationComponent::STATE_MOVE);
}


void GMainScene::createStartButton()
{
	GSprite* spriteButtonNorm = GResManager::Instance()->GetSprite("blue_button_start_normal.png");
	GSprite* spriteButtonMove = GResManager::Instance()->GetSprite("blue_button_start_move.png");
	GSprite* spriteButtonDown = GResManager::Instance()->GetSprite("blue_button_start_down.png");
	GSprite* spriteButtonUp = GResManager::Instance()->GetSprite("blue_button_start_up.png");

	Entity button = mEntityManager->createPlainEntity(spriteButtonNorm, WIDTH - spriteButtonNorm->getWidth() / 1.5f , HEIGHT / 3);

	auto move = mEntityManager->addComponentsToEntity<GMouseMoveEventComponent>(button, spriteButtonMove, spriteButtonNorm);
	auto down = mEntityManager->addComponentsToEntity<GMouseDownEventComponent>(button, spriteButtonDown);
	auto up = mEntityManager->addComponentsToEntity<GMouseUpEventComponent>(button, spriteButtonUp);

	auto renderButton = mEntityManager->getComponent<GRenderableComponent>(button);
	move->signal_MouseMovedInEntity.connect(renderButton->slot_ChangeSprite);
	move->signal_MouseMovedOutEntity.connect(renderButton->slot_ChangeSprite);
	down->signal_MouseDownNewSprite.connect(renderButton->slot_ChangeSprite);
	up->signal_MouseUpNewSprite.connect(renderButton->slot_ChangeSprite);
}

void GMainScene::cloudMovingFinished(Entity entity)
{
	auto moveable = mEntityManager->getComponent<GMoveableAnimationComponent>(entity);
}