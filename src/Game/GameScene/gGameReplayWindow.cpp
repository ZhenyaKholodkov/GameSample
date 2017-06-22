#include "gGameReplayWindow.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

GGameReplayWindow::GGameReplayWindow(std::shared_ptr<GEntityManager> manager) :
	mManager(manager)
{

}
GGameReplayWindow::~GGameReplayWindow()
{
	unload();
}
void GGameReplayWindow::unload()
{
	signal_show.disconnect_all_slots();
	signal_hide.disconnect_all_slots();
	signal_exit.disconnect_all_slots();
	signal_replay.disconnect_all_slots();
	mManager->destroyEntity(mWindow);
	mManager->destroyEntity(mMenuButton);
	mManager->destroyEntity(mReplayButton);
	mManager->destroyEntity(mText);
}


void GGameReplayWindow::create()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	mWindow = mManager->createPlainEntity(window, WIDTH / 2, HEIGHT / 3, 2);
	mManager->getComponent<GRenderableComponent>(mWindow)->setXYScale(0.0f, 0.0f);
	auto scale = mManager->addComponentsToEntity<GScalableComponent>(mWindow, 0.0f, 0.0f, 1.0f, 1.0f, 500,
		GEasings::EasingType::EXPO_OUT_EASING);
	signal_show.connect(scale->slot_Scale);
	signal_hide.connect(scale->slot_RevertScale);

	createMenuButton();
	createReplayButton();


	mText = mManager->createEntity(4);
	mManager->addComponentsToEntity<GLocationComponent>(mText, 0.0f, -window->getHeight() / 5.5f);
	mManager->addComponentsToEntity<GRenderableComponent>(mText, nullptr, 1.0f, 1.0f, 0.0f, "You've lost. Replay?", GColor(0x275fa3), 28);
	mManager->setChildParentRelations(mWindow, mText);
}

void GGameReplayWindow::createMenuButton()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	GSprite* menuNormal = GResManager::Instance()->GetSprite("blue_button_menu_normal.png");
	GSprite* menuMove = GResManager::Instance()->GetSprite("blue_button_menu_move.png");
	GSprite* menuDown = GResManager::Instance()->GetSprite("blue_button_menu_down.png");

	float menuButtonScale = 0.5f;
	mMenuButton = mManager->createButtonEntity(menuNormal, menuMove, menuDown, window->getWidth() / 4.0f, window->getHeight() / 5.0f, 4);
	mManager->getComponent<GRenderableComponent>(mMenuButton)->setXYScale(menuButtonScale, menuButtonScale);
	mManager->setChildParentRelations(mWindow, mMenuButton);

	mManager->getComponent<GMouseDownEventComponent>(mMenuButton)->signal_MouseDown.connect([this]() {
		signal_exit();
	});
}

void GGameReplayWindow::createReplayButton()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	GSprite* menuNormal = GResManager::Instance()->GetSprite("blue_button_replay_normal.png");
	GSprite* menuMove = GResManager::Instance()->GetSprite("blue_button_replay_move.png");
	GSprite* menuDown = GResManager::Instance()->GetSprite("blue_button_replay_down.png");

	float backButtonXScale = -0.5f;
	float backButtonYScale = 0.5f;
	mReplayButton = mManager->createButtonEntity(menuNormal, menuMove, menuDown, -window->getWidth() / 4, window->getHeight() / 5, 4);

	mManager->getComponent<GRenderableComponent>(mReplayButton)->setXYScale(backButtonXScale, backButtonYScale);
	mManager->setChildParentRelations(mWindow, mReplayButton);

	mManager->getComponent<GMouseDownEventComponent>(mReplayButton)->signal_MouseDown.connect([this]() {
		signal_replay();
	});
}


void GGameReplayWindow::show()
{
	signal_show();
}

void GGameReplayWindow::hide()
{
	signal_hide();
}