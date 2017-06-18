#include "gMainScene.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"


GMainScene::GMainScene(std::shared_ptr<GEntityManager> entityManager) : 
	mEntityManager(entityManager),
	mIsStarted(false)
{}

GMainScene::~GMainScene()
{
	unload();
}

void GMainScene::unload()
{
	mEntityManager->destroyEntity(mBackgound);
	mEntityManager->destroyEntity(mGrass);
	mEntityManager->destroyEntity(mMill);
	mEntityManager->destroyEntity(mMillSacles);
	mEntityManager->destroyEntity(mStartButton);
	mEntityManager->destroyEntity(mCloud);
	mEntityManager->destroyEntity(mBird);

	mIsStarted = false;
}

void GMainScene::start()
{
	GSprite* spriteBackgroun = GResManager::Instance()->GetSprite("background.png");
	GSprite* spriteGrass = GResManager::Instance()->GetSprite("grass.png");

	mBackgound = mEntityManager->createPlainEntity(spriteBackgroun, WIDTH / 2, HEIGHT / 2);
	createCloud();
	createMill();
	mGrass = mEntityManager->createPlainEntity(spriteGrass, WIDTH / 2, HEIGHT - spriteGrass->getHeight() / 2);

	createBird();
	createStartButton();

	auto buttonDown = mEntityManager->getComponent<GMouseDownEventComponent>(mStartButton);
	buttonDown->signal_MouseDown.connect([this] {
		auto rotable = mEntityManager->getComponent<GRotableComponent>(mMillSacles);
		rotable->setBegVelocity(rotable->getEndVelocity());
		rotable->setEndVelocity(36.0f);
		rotable->reset();
		auto millMovable = mEntityManager->getComponent<GMoveableAnimationComponent>(mMill);
		rotable->signal_AngleChangingFinished.connect(millMovable->slot_Move);
		millMovable->signal_MovingFinished.connect(slot_EndScene);
	});
	mIsStarted = true;
}

void GMainScene::createMill()
{
	GSprite* spriteMillBase = GResManager::Instance()->GetSprite("mill_color.png");
	GSprite* spriteMillSales = GResManager::Instance()->GetSprite("mill_part2.png");
	GSprite* spriteGrass = GResManager::Instance()->GetSprite("grass.png");

	float millX = WIDTH / 2 - spriteMillBase->getWidth() / 2;
	float millY = HEIGHT - spriteGrass->getHeight() - spriteMillBase->getHeight() / 2 + 70.0f;

	mMill = mEntityManager->createPlainEntity(spriteMillBase, millX, millY);
	mMillSacles = mEntityManager->createPlainEntity(spriteMillSales, 0.0f, -57.0f);
	mEntityManager->setChildParentRelations(mMill, mMillSacles);
	auto rotable = mEntityManager->addComponentsToEntity<GRotableComponent>(mMillSacles, 0, 0.36f, 1500, true, 
		                                                                    GEasings::EasingType::QUART_IN_EASING);
	mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mMill, millX, millY, millX, 0.0f - spriteMillBase->getHeight(),
		                                                               1000, false, GEasings::EasingType::QUAD_IN_EASING);
	rotable->setState(GRotableComponent::STATE_ROTATE);

}

void GMainScene::createCloud()
{
	GSprite* spriteCloud = GResManager::Instance()->GetSprite("cloud.png");

	mCloud = mEntityManager->createPlainEntity(spriteCloud, WIDTH + spriteCloud->getWidth() / 2, HEIGHT * 0.2f);
	auto renderCloud = mEntityManager->getComponent<GRenderableComponent>(mCloud);
	auto moveableCloud = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mCloud,
		WIDTH + spriteCloud->getWidth() / 2, HEIGHT * 0.2f, -spriteCloud->getWidth(), HEIGHT * 0.2f, 20000, true);
	moveableCloud->signal_MovingFinishedEntity.connect(slot_CloudMovingFinished);
	moveableCloud->setState(GMoveableAnimationComponent::STATE_MOVE);
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

	mBird = mEntityManager->createPlainEntity(spriteBird1, -spriteBird1->getWidth() / 2, HEIGHT * 0.4f);
	auto animation = mEntityManager->addComponentsToEntity<GAnimationComponent>(mBird, 60, true);
	animation->AddFrame(spriteBird1);
	animation->AddFrame(spriteBird2);
	animation->AddFrame(spriteBird3);
	animation->AddFrame(spriteBird4);
	animation->AddFrame(spriteBird5);
	animation->AddFrame(spriteBird6);
	animation->AddFrame(spriteBird7);
	animation->setState(GAnimationComponent::STATE_RUN);

	auto moveableBird = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mBird,
		-spriteBird1->getWidth() / 2, HEIGHT * 0.4f,
		WIDTH + spriteBird1->getWidth() / 2, HEIGHT * 0.4f, 10000, true);
	moveableBird->setState(GMoveableAnimationComponent::STATE_MOVE);
}


void GMainScene::createStartButton()
{
	GSprite* spriteButtonNorm = GResManager::Instance()->GetSprite("blue_button_start_normal.png");
	GSprite* spriteButtonMove = GResManager::Instance()->GetSprite("blue_button_start_move.png");
	GSprite* spriteButtonDown = GResManager::Instance()->GetSprite("blue_button_start_down.png");

	const float scaleX = 0.6f;
	const float scaleY = 0.6f;

	float buttonX = WIDTH - (spriteButtonNorm->getWidth() * scaleX * 0.7f);
	float buttonY = spriteButtonNorm->getHeight() * scaleY  * 0.7f;

	mStartButton = mEntityManager->createButtonEntity(spriteButtonNorm, spriteButtonMove, spriteButtonDown, buttonX, buttonY);

	auto scale = mEntityManager->addComponentsToEntity<GScalableComponent>(mStartButton, 0.0f, 0.0f, scaleX, scaleY, 2000,
		                                                                   GEasings::EasingType::ELASTIC_OUT_EASING);
	scale->setState(GScalableComponent::STATE_SCALE);

	auto renderButton = mEntityManager->getComponent<GRenderableComponent>(mStartButton);
	renderButton->setXScale(scaleX);
	renderButton->setYScale(scaleY);

	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mStartButton, buttonX, buttonY, 
		                                                                               WIDTH + spriteButtonNorm->getWidth(), buttonY, 2500, 
		                                                                               false, GEasings::EasingType::EXPO_OUT_EASING);
	auto down = mEntityManager->getComponent<GMouseDownEventComponent>(mStartButton);
	down->signal_MouseDown.connect(moveable->slot_Move);
}

void GMainScene::cloudMovingFinished(Entity entity)
{
	auto moveable = mEntityManager->getComponent<GMoveableAnimationComponent>(entity);
}

void GMainScene::endScene()
{
	signal_NextScreen();
}