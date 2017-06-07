#ifndef G_MAIN_SCENE

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GMainScene
{
public:
	GMainScene(std::shared_ptr<GEntityManager> entityManager) : mEntityManager(entityManager) {}
	~GMainScene() {}
	void start();
private:
	void cloudMovingFinished(Entity entity);

	void createBird();
	void createStartButton();
public: /*lots*/
	const boost::signals2::signal<void(Entity)>::slot_type slot_CloudMovingFinished = boost::bind(&GMainScene::cloudMovingFinished, this, _1);
private:
	const std::shared_ptr<GEntityManager> mEntityManager;
};
#define G_MAIN_SCENE
#endif G_MAIN_SCENE//