#ifndef GSYSTEM_MANAGER_H
#define GSYSTEM_MANAGER_H

#include <memory>

#include "gBaseSystem.h"
#include "gRenderSystem.h"
#include "gMoveableAnimationSystem.h"
#include "gMoveableSystem.h"
#include "gUserInputSystem.h"
#include "gScalableSystem.h"
#include "gCollisionSystem.h"
#include "gAnimationSystem.h"
#include "gRotableSystem.h"

class GSystemManager
{
	friend class GGameWindow;
public:
	//static GSystemManager* instance();

	template<typename S, typename... Args>
	std::shared_ptr<GBaseSystem> registerSystem(Args&& ...args);

	template<typename S>
	std::shared_ptr<GBaseSystem>  getSystem() const;

	void update(int dt);

private:
	GSystemManager();

	uint32 getSystemCount() const;
private:
	std::vector<std::shared_ptr<GBaseSystem>> mSystems; 
};

template<typename S, typename... Args>
std::shared_ptr<GBaseSystem> GSystemManager::registerSystem(Args&& ...args)
{
	uint32 index = GSystem<S>::getSystemId();
	mSystems[index] = std::shared_ptr<GBaseSystem>(new S(std::forward<Args>(args)...));
	return mSystems[index];
}

template<typename S>
std::shared_ptr<GBaseSystem> GSystemManager::getSystem() const
{
	uint32 index = GSystem<S>::getSystemId();
	return mSystems[index];
}
#endif //GSYSTEM_MANAGER_H

