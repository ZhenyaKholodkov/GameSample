#ifndef G_GAME_ENTOTYGROUP
#define G_GAME_ENTOTYGROUP

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GGameEntityGroup
{
	public:
		GGameEntityGroup(std::shared_ptr<GEntityManager> manager);
		~GGameEntityGroup();
private:

		std::shared_ptr<GEntityManager> mManager;
};
#endif// G_GAME_EXITWINDOW