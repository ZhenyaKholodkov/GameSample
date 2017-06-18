#ifndef G_GAME_EXITWINDOW
#define G_GAME_EXITWINDOW

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GGameExitWindow
{
	public:
		GGameExitWindow(std::shared_ptr<GEntityManager> manager);
		~GGameExitWindow();

		void create();
	private:
		void createMenuButton();
		void createBackButton();
		void show();
		void hide();
	public:/*slots*/
		boost::signals2::signal<void()>::slot_type slot_show = boost::bind(&GGameExitWindow::show, this);
		boost::signals2::signal<void()>::slot_type slot_hide = boost::bind(&GGameExitWindow::hide, this);
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
#endif// G_GAME_EXITWINDOW