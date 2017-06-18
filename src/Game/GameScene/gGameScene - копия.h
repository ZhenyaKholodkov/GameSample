#ifndef G_GAME_SCENE
#define G_GAME_SCENE

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

#include <queue>
#include "gGameBackground.h"

class GGameScene
{
public:
	GGameScene(std::shared_ptr<GEntityManager> entityManager);
	~GGameScene();

	void start();
	void unload();
	void update(int dt);

	bool isStarted() const { return mIsStarted; }
private:
	class ExitWindow
	{
	public:
		ExitWindow(std::shared_ptr<GEntityManager> manager);
		~ExitWindow();

		void create();
	private:
		void createMenuButton();
		void createBackButton();
		void show();
		void hide();
	public:/*slots*/
		boost::signals2::signal<void()>::slot_type slot_show = boost::bind(&ExitWindow::show, this);
		boost::signals2::signal<void()>::slot_type slot_hide = boost::bind(&ExitWindow::hide, this);
	public:/*signals*/
		boost::signals2::signal<void()> signal_show;
		boost::signals2::signal<void()> signal_hide;
		boost::signals2::signal<void()> signal_exit;
	private:
		Entity mWindow;
		Entity mMenuButton;
		Entity mBackButton;
		Entity mText;
		std::shared_ptr<GEntityManager> mManager;
	};


	void createPauseButton();
	void createMill();
	void createGroup(uint32 line);
	void createBag(float x, float y);
	void createCoin(float x, float y);
public: /*lots*/
public:/*signals*/
	boost::signals2::signal<void()> signal_NextScreen;
	boost::signals2::signal<void()> signal_Pause;
	boost::signals2::signal<void()> signal_Continue;
private:
	const std::shared_ptr<GEntityManager> mEntityManager;

	Entity mMill;
	Entity mMillSacles;
	Entity mPauseButton;
	Entity mBackGround;
	Entity mTextScore;
	Entity mControls;

	ExitWindow mExitWindow;
	GGameBackground mGameBackground;

	bool mIsStarted;
	bool mPause;

	float mComplexityCoef;
	uint32 mTopPestChance;
	uint32 mBottomPestChance;
	uint32 mScore;
	uint32 mLastAddTime;
	uint32 mGameTime;

	std::queue<Entity> mEntitiesToDelete;
};
#endif// G_GAME_SCENE