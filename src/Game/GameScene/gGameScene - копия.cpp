#include "gGameScene.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

#include <time.h>


GGameScene::GGameScene(std::shared_ptr<GEntityManager> entityManager) :
	mEntityManager(entityManager),
	mExitWindow(entityManager),
	mGameBackground(entityManager),
	mIsStarted(false),
	mPause(false),
	mComplexityCoef(0.0f),
	mTopPestChance(0),
	mBottomPestChance(0),
	mScore(0),
	mLastAddTime(0),
	mGameTime(0)
{}

GGameScene::~GGameScene()
{
	unload();
}
void GGameScene::unload()
{
	mEntityManager->destroyEntity(mControls);
	mEntityManager->destroyEntity(mBackGround);
	mEntityManager->destroyEntity(mMill);
	mEntityManager->destroyEntity(mMillSacles);
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

	//mGameBackground.start();
	createMill();
	createPauseButton();
	mIsStarted = true;

	mTextScore = mEntityManager->createEntity();
	mEntityManager->addComponentsToEntity<GLocationComponent>(mTextScore, 50.0f, 50.0f);
	mEntityManager->addComponentsToEntity<GRenderableComponent>(mTextScore, nullptr, 1.0f, 1.0f, 0.0f, "Score: 0", GColor(0xffffff), 32);

}

void GGameScene::update(int dt)
{
	if (mPause)
		return;
//	mGameBackground.refreshBackground();
	mLastAddTime += dt;
	mGameTime += dt;

	if (mLastAddTime >= 1000 && mLastAddTime <= 2000) {
		srand(time(0));
		mTopPestChance = rand() % 100;
	}
	if (mLastAddTime >= 2000 && mLastAddTime <= 3000) {
		srand(time(0));
		mBottomPestChance = rand() % 100;
	}

	if (mLastAddTime >= 5000)
	{
		mComplexityCoef = (float)mGameTime / 60000.0f;
		srand(time(0));
		createGroup(rand() % 3);
		mLastAddTime = 0;
	}

	while (mEntitiesToDelete.size() > 0)
	{
		mEntityManager->destroyEntity(mEntitiesToDelete.front());
		mEntitiesToDelete.pop();
	}
}

void GGameScene::createGroup(uint32 line)
{
	srand(time(0));
	const uint32 MAX_ELEMENTS = 6;
	uint32 rndom = (rand() % 5);
	uint32 size = 2 + rndom * mComplexityCoef;

	GSprite* spriteCoin = GResManager::Instance()->GetSprite("coin.png");

	float hDistance = spriteCoin->getHeight() * 2;
	float x = WIDTH / 6 + (line * WIDTH / 3), y = -200.0f;// spriteCoin->getHeight() * MAX_ELEMENTS;
	uint32 index = 0;

	bool topPest = false, bottomPest = false;
	uint32 curTopPestChance = mComplexityCoef < 2.0f ? mComplexityCoef * 20 : 50;
	uint32 curBottomPestChance = mComplexityCoef < 2.0f ? mComplexityCoef * 40 : 80;
	if (mTopPestChance < curTopPestChance)
	{
		createBag(x, y + size * hDistance);
		--size;
	}	
	if (mBottomPestChance < curBottomPestChance)
	{
		createBag(x, y);
		++index;
		y += hDistance;
	}
	while (index != size)
	{
		createCoin(x, y);
		++index;
		y += hDistance;
	}
}

void GGameScene::createBag(float x, float y)
{
	GSprite* spriteBag = GResManager::Instance()->GetSprite("bag.png");
	if (!spriteBag)
		return;

	Entity bag = mEntityManager->createPlainEntity(spriteBag, x, y);

	uint32 movingeTime = mComplexityCoef > 0.1f ? (mComplexityCoef < 1.0f ? 1000 / mComplexityCoef : 1000) : 10000;
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(bag, x, y,
		x, y + HEIGHT * 1.3f, movingeTime);
	moveable->signal_MovingFinishedEntity.connect([this](Entity entity)
	{
		//mEntityManager->destroyEntity(entity);
		mEntitiesToDelete.push(entity);
	});
	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);
}

void GGameScene::createCoin(float x, float y)
{
	GSprite* spriteCoin = GResManager::Instance()->GetSprite("coin.png");
	if (!spriteCoin)
		return;

	Entity bag = mEntityManager->createPlainEntity(spriteCoin, x, y);

	uint32 movingeTime = mComplexityCoef > 0.1f ? (mComplexityCoef < 1.0f ? 1000 / mComplexityCoef : 1000) : 10000;
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(bag, x, y,
		x, y + HEIGHT * 1.3f, movingeTime);
	moveable->signal_MovingFinishedEntity.connect([this](Entity entity)
	{
		mEntitiesToDelete.push(entity);
	//	mEntityManager->destroyEntity(entity);
	});

	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);

	auto collision = mEntityManager->addComponentsToEntity<GCollisionComponent>(bag, spriteCoin->getWidth() / 2, spriteCoin->getHeight() / 2);
	collision->signal_CollisionedWho.connect([this](Entity entity)
	{
		mScore += 1;
		auto renderabe = mEntityManager->getComponent<GRenderableComponent>(mTextScore);
		renderabe->setText("Score: " + std::to_string(mScore));
		mEntitiesToDelete.push(entity);
	});
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
	mMillSacles = mEntityManager->createPlainEntity(spriteMillSales, 0.0f, -57.0f);
	mEntityManager->setChildParentRelations(mMill, mMillSacles);
	auto rotable = mEntityManager->addComponentsToEntity<GRotableComponent>(mMillSacles, 12.0f, 12.0f);
	auto moveable = mEntityManager->addComponentsToEntity<GMoveableAnimationComponent>(mMill, millX, millY,
		                                                               millX, HEIGHT - 0.5f * spriteMillBase->getHeight() / 2,
		                                                               1000, false, GEasings::EasingType::QUAD_IN_EASING);
	rotable->setState(GRotableComponent::STATE_ROTATE);
	moveable->setState(GMoveableAnimationComponent::STATE_MOVE);

	mEntityManager->addComponentsToEntity<GCollisionComponent>(mMill, spriteMillBase->getWidth() / 2, 
		                                                                        spriteMillBase->getHeight() / 2 + spriteMillSales->getHeight() / 6);

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


GGameScene::ExitWindow::ExitWindow(std::shared_ptr<GEntityManager> manager) :
	mManager(manager)
{

}
GGameScene::ExitWindow::~ExitWindow()
{
	mManager->destroyEntity(mWindow);
	mManager->destroyEntity(mMenuButton);
	mManager->destroyEntity(mBackButton);
	mManager->destroyEntity(mText);
}

void GGameScene::ExitWindow::create()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	mWindow = mManager->createPlainEntity(window, WIDTH / 2, HEIGHT / 3);
	mManager->getComponent<GRenderableComponent>(mWindow)->setXYScale(0.0f, 0.0f);
	auto scale = mManager->addComponentsToEntity<GScalableComponent>(mWindow, 0.0f, 0.0f, 1.0f, 1.0f, 1000,
		                                                             GEasings::EasingType::EXPO_OUT_EASING);
	signal_show.connect(scale->slot_Scale);
	signal_hide.connect(scale->slot_RevertScale);

	createMenuButton();
	createBackButton();


	mText = mManager->createEntity();
	mManager->addComponentsToEntity<GLocationComponent>(mText, - window->getWidth() / 2.4f, - window->getHeight() / 5);
	mManager->addComponentsToEntity<GRenderableComponent>(mText, nullptr, 1.0f, 1.0f, 0.0f, "Tap the menu button to exit.", GColor(0x275fa3), 28);
	mManager->setChildParentRelations(mWindow, mText);
}

void GGameScene::ExitWindow::createMenuButton()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	GSprite* menuNormal = GResManager::Instance()->GetSprite("blue_button_menu_normal.png");
	GSprite* menuMove = GResManager::Instance()->GetSprite("blue_button_menu_move.png");
	GSprite* menuDown = GResManager::Instance()->GetSprite("blue_button_menu_down.png");

	float menuButtonScale = 0.5f;
	mMenuButton = mManager->createButtonEntity(menuNormal, menuMove, menuDown, window->getWidth() / 4, window->getHeight() / 5);
	mManager->getComponent<GRenderableComponent>(mMenuButton)->setXYScale(menuButtonScale, menuButtonScale);
	mManager->setChildParentRelations(mWindow, mMenuButton);

	mManager->getComponent<GMouseDownEventComponent>(mMenuButton)->signal_MouseDown.connect([this]() {
		signal_exit();
	});
}

void GGameScene::ExitWindow::createBackButton()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	GSprite* menuNormal = GResManager::Instance()->GetSprite("blue_button_start_normal.png");
	GSprite* menuMove = GResManager::Instance()->GetSprite("blue_button_start_move.png");
	GSprite* menuDown = GResManager::Instance()->GetSprite("blue_button_start_down.png");

	float backButtonXScale = -0.5f;
	float backButtonYScale = 0.5f;
	mBackButton = mManager->createButtonEntity(menuNormal, menuMove, menuDown, - window->getWidth() / 4,
		                                       window->getHeight() / 5);

	mManager->getComponent<GRenderableComponent>(mBackButton)->setXYScale(backButtonXScale, backButtonYScale);
	mManager->setChildParentRelations(mWindow, mBackButton);

	mManager->getComponent<GMouseDownEventComponent>(mBackButton)->signal_MouseDown.connect([this]() {
		hide();
	});
}


void GGameScene::ExitWindow::show()
{
	create();
	signal_show();
}

void GGameScene::ExitWindow::hide()
{
	signal_hide();
}