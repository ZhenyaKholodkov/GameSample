#ifndef GSYSTEM_MANAGER_H
#define GSYSTEM_MANAGER_H

#include "gBaseSystem.h"
#include "gRenderSystem.h"
#include "gMoveableAnimationSystem.h"
#include "gMoveableSystem.h"
#include "gUserInputSystem.h"
#include "gScalableSystem.h"
#include "gCollisionSystem.h"
#include "g2048MechanicSystem.h"

class GSystemManager
{
public:
	static GSystemManager* Instatnce();

	template<typename S>
	void RegisterSystem();

	template<typename S>
	GBaseSystem* GetSystem();

	void update(int dt);

private:
	GSystemManager();
	~GSystemManager();

	uint32 GetSystemCount();
private:
	std::vector<GBaseSystem*>            mSystems; 
};



template<typename S>
void GSystemManager::RegisterSystem()
{
	uint32 index = GSystem<S>::getSystemId();
	if (mSystems[index] == nullptr) 
	{
		mSystems[index] = new S();
	}
}

template<typename S>
GBaseSystem* GSystemManager::GetSystem()
{
	uint32 index = GSystem<S>::getSystemId();
	return mSystems[index];
}
#endif //GSYSTEM_MANAGER_H

