#ifndef G_GAME_REPLAYWINDOW
#define G_GAME_REPLAYWINDOW

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GGameReplayWindow
{
	public:
		GGameReplayWindow(std::shared_ptr<GEntityManager> manager);
		~GGameReplayWindow();

		void create();
		void show();
		void hide();
	private:
		void createMenuButton();
		void createReplayButton();
	public:/*slots*/
		boost::signals2::signal<void()>::slot_type slot_show = boost::bind(&GGameReplayWindow::show, this);
		boost::signals2::signal<void()>::slot_type slot_hide = boost::bind(&GGameReplayWindow::hide, this);
	public:/*signals*/
		boost::signals2::signal<void()> signal_show;
		boost::signals2::signal<void()> signal_hide;
		boost::signals2::signal<void()> signal_exit;
		boost::signals2::signal<void()> signal_replay;
	private:
		Entity mWindow;
		Entity mMenuButton;
		Entity mReplayButton;
		Entity mText;
		std::shared_ptr<GEntityManager> mManager;
};
#endif// G_GAME_EXITWINDOW