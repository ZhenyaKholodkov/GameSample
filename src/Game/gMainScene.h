#ifndef G_MAIN_SCENE

#include <memory>

#include "gEntityManager.h"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"

class GMainScene
{
public:
	GMainScene(std::shared_ptr<GEntityManager> entityManager);
	~GMainScene();
	void start();
	void unload();

	bool isStarted() const { return mIsStarted; }
private:
	void cloudMovingFinished(Entity entity);
	void endScene();

	void createBird();
	void createStartButton();
	void createMill();
	void createCloud();
public: /*lots*/
	const boost::signals2::signal<void(Entity)>::slot_type slot_CloudMovingFinished = boost::bind(&GMainScene::cloudMovingFinished, this, _1);
	boost::signals2::signal<void()>::slot_type             slot_EndScene = boost::bind(&GMainScene::endScene, this);
public:/*signals*/
	boost::signals2::signal<void()> signal_NextScreen;
private:
	const std::shared_ptr<GEntityManager> mEntityManager;

	Entity mBackgound;
	Entity mGrass;
	Entity mMill;
	Entity mMillSacles;
	Entity mStartButton;
	Entity mCloud;
	Entity mBird;

	bool mIsStarted;
};
#define G_MAIN_SCENE
#endif G_MAIN_SCENE//