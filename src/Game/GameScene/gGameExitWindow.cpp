#include "gGameExitWindow.h"

#include "GResManager.h"
#include "GSprite.h"
#include "GDefines.h"

GGameExitWindow::GGameExitWindow(std::shared_ptr<GEntityManager> manager) :
	mManager(manager)
{

}
GGameExitWindow::~GGameExitWindow()
{
	mManager->destroyEntity(mWindow);
	mManager->destroyEntity(mMenuButton);
	mManager->destroyEntity(mBackButton);
	mManager->destroyEntity(mText);
}

void GGameExitWindow::create()
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
	mManager->addComponentsToEntity<GLocationComponent>(mText, 0.0f/*-window->getWidth() / 2.5f*/, -window->getHeight() / 5);
	mManager->addComponentsToEntity<GRenderableComponent>(mText, nullptr, 1.0f, 1.0f, 0.0f, "Tap the menu button to exit.", GColor(0x275fa3), 26);
	mManager->setChildParentRelations(mWindow, mText);
}

void GGameExitWindow::createMenuButton()
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

void GGameExitWindow::createBackButton()
{
	GSprite* window = GResManager::Instance()->GetSprite("window.png");
	GSprite* menuNormal = GResManager::Instance()->GetSprite("blue_button_start_normal.png");
	GSprite* menuMove = GResManager::Instance()->GetSprite("blue_button_start_move.png");
	GSprite* menuDown = GResManager::Instance()->GetSprite("blue_button_start_down.png");

	float backButtonXScale = -0.5f;
	float backButtonYScale = 0.5f;
	mBackButton = mManager->createButtonEntity(menuNormal, menuMove, menuDown, -window->getWidth() / 4,
		window->getHeight() / 5);

	mManager->getComponent<GRenderableComponent>(mBackButton)->setXYScale(backButtonXScale, backButtonYScale);
	mManager->setChildParentRelations(mWindow, mBackButton);

	mManager->getComponent<GMouseDownEventComponent>(mBackButton)->signal_MouseDown.connect([this]() {
		hide();
	});
}


void GGameExitWindow::show()
{
	create();
	signal_show();
}

void GGameExitWindow::hide()
{
	signal_hide();
}