#ifndef GSYSTEM_MANAGER_H
#define GSYSTEM_MANAGER_H

#include "gBaseSystem.h"
#include "gRenderableComponent.h"

//template<class Component>
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
	vector<GBaseSystem*>            mSystems; // тоже самое
};



template<typename S>
void GSystemManager::RegisterSystem()
{
	uint32 index = GSystem<S>::getSystemId();
	mSystems[index] = new S();
}

template<typename S>
GBaseSystem* GSystemManager::GetSystem()
{
	uint32 index = GSystem<S>::getSystemId();
	return mSystems[index];
}
#endif //GSYSTEM_MANAGER_H

