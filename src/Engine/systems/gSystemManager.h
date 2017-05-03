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
#include "g2048MechanicSystem.h"
#include "gAnimationSystem.h"

class GSystemManager
{
public:
	static GSystemManager* instance();

	template<typename S>
	void registerSystem();

	template<typename S>
	std::shared_ptr<GBaseSystem>  getSystem() const;

	void update(int dt);

private:
	GSystemManager();
	~GSystemManager();

	uint32 getSystemCount() const;
private:
	std::vector<std::shared_ptr<GBaseSystem>> mSystems; 
};

template<typename S>
void GSystemManager::registerSystem()
{
	uint32 index = GSystem<S>::getSystemId();
	mSystems[index] = std::shared_ptr<GBaseSystem>(new S());
}

template<typename S>
std::shared_ptr<GBaseSystem> GSystemManager::getSystem() const
{
	uint32 index = GSystem<S>::getSystemId();
	return mSystems[index];
}
#endif //GSYSTEM_MANAGER_H

