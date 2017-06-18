#include "gGameScene.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

#include <time.h>

GGameScene::GGameScene(std::shared_ptr<GEntityManager> entityManager) :
	mEntityManager(entityManager),
	mExitWindow(entityManager),
	mReplayWindow(entityManager),
	mGameGun(entityManager),
	mIsStarted(false),
	mPause(false),
	mScore(0)
{}

GGameScene::~GGameScene()
{
	unload();
}
void GGameScene::unload()
{
	mGameGun.unload();
	mEntityManager->destroyEntity(mControls);
	mEntityManager->destroyEntity(mBackGround);
	mEntityManager->destroyEntity(mMill);
	mEntityManager->destroyEntity(mPauseButton);
	mEntityManager->destroyEntity(mTextScore);

	mIsStarted = false;
}


void GGameScene::start()
{
	GSprite* spriteBackgroun = GResManager::Instance()->GetSprite("background.png");
	GSprite* spriteCoin = GResManager::Instance()->GetSprite("coin.png");

	// create entity for processing the keyboard keys
	mControls = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GKeyUpEventComponent>(mControls);
	mEntityManager->addComponentsToEntity<GKeyDownEventComponent>(mControls);


	mBackGround = mEntityManager->createPlainEntity(spriteBackgroun, WIDTH / 2, HEIGHT / 2);

	createMill();
	createPauseButton();
	mIsStarted = true;

	mTextScore = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(mTextScore, 100.0f, 100.0f);
	mEntityManager->addComponentsToEntity<GRenderableComponent>(mTextScore, nullptr, 1.0f, 1.0f, 0.0f, "Score: 0", GColor(0xffffff), 32);

	mReplayWindow.signal_replay.connect(slot_replay);
	mReplayWindow.signal_exit.connect([this]() {
		signal_NextScreen();
	});

	mGameGun.start();
	mGameGun.signal_GotCoin.connect(slot_addCoin);
	mGameGun.signal_Lost.connect([this]()
	{
		mPause = true;
		signal_Pause();
		mReplayWindow.show();
	});
}

void GGameScene::addCoin()
{
	++mScore;
	auto render = mEntityManager->getComponent<GRenderableComponent>(mTextScore);
	render->setText("Score: " + std::to_string(mScore));
}

void GGameScene::replay()
{
	unload();
	start();
	mPause = false;
	signal_Continue();
}

void GGameScene::update(int dt)
{
	if (mPause)
		return;

	mGameGun.update(dt);
}

void GGameScene::createMill()
{
	GSprite* spriteMillBase = GResManager::Instance()->GetSprite("mill_color.png");
	GSprite* spriteMillSales = GResManager::Instance()->GetSprite("mill_part2.png");

	float millX = WIDTH / 2;
	float millY = HEIGHT + spriteMillBase->getHeight();

	mMill = mEntityManager->createPlainEntity(spriteMillBase, millX, millY);
	auto renderMill = mEntityManager->getComponent<GRenderableComponent>(mMill);
	renderMill->setXYScale(0.4f, 0.4f);
	Entity millSacles = mEntityManager->createPlainEntity(spriteMillSales, 0.0f, -57.0f);
	mEntityManager->setChildParentRelations(mMill, millSacles);
	auto rotable = mEntityManager->addComponentsToEntity<GRotableComponent>(millSacles, 12.0f, 12.0f);
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mMill, millX, millY,
		                                                               millX, HEIGHT - 0.5f * spriteMillBase->getHeight() / 2,
		                                                               1000, false, GEasings::EasingType::QUAD_IN_EASING);
	rotable->setState(GRotableComponent::STATE_ROTATE);
	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);

	mEntityManager->addComponentsToEntity<GCollisionComponent>(mMill, spriteMillBase->getWidth() / 4, 
		                                                       spriteMillBase->getHeight() / 2);

	auto move = mEntityManager->addComponentsToEntity<GMoveableComponent>(mMill, WIDTH / 3, 0.0f);
	auto control = mEntityManager->getComponent<GKeyDownEventComponent>(mControls);
	control->signal_KeyLeft.connect(move->slot_MoveDxRevert);
	control->signal_KeyRight.connect(move->slot_MoveDx);
}

void GGameScene::createPauseButton()
{
	GSprite* spriteButtonNorm = GResManager::Instance()->GetSprite("blue_button_pause_normal.png");
	GSprite* spriteButtonMove = GResManager::Instance()->GetSprite("blue_button_pause_move.png");
	GSprite* spriteButtonDown = GResManager::Instance()->GetSprite("blue_button_pause_down.png");

	float buttonX = WIDTH - (spriteButtonNorm->getWidth() * 0.7f);
	float buttonY = spriteButtonNorm->getHeight()  * 0.7f;

	mPauseButton = mEntityManager->createButtonEntity(spriteButtonNorm, spriteButtonMove, spriteButtonDown, buttonX, buttonY);

	auto scale = mEntityManager->addComponentsToEntity<GScalableComponent>(mPauseButton, 0.0f, 0.0f, 1.0f, 1.0f, 2000, 
		                                                                   GEasings::EasingType::ELASTIC_OUT_EASING);
	scale->setState(GScalableComponent::STATE_SCALE);

	auto renderButton = mEntityManager->getComponent<GRenderableComponent>(mPauseButton);
	auto down = mEntityManager->getComponent<GMouseDownEventComponent>(mPauseButton);

	down->signal_MouseDown.connect(mExitWindow.slot_show);
	mExitWindow.signal_exit.connect([this]() {
		signal_NextScreen();
	});
	mExitWindow.signal_show.connect([this]() {
		mPause = true;
		signal_Pause();
	});
	mExitWindow.signal_hide.connect([this]() {
		mPause = false;
		signal_Continue();
	});
}