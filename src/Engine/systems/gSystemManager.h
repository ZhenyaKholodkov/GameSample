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

	vector<GBaseSystem*>            mSystems; // ���� �����
};



template<typename S>
void GSystemManager::RegisterSystem()
{
	uint32 index = GSystem<S>::getSystemIndex();
	mSystems[index] = new S();
}

template<typename S>
GBaseSystem* GSystemManager::GetSystem()
{
	uint32 index = GSystem<S>::getSystemIndex();
	return mSystems[index];
}
#endif //GSYSTEM_MANAGER_H

