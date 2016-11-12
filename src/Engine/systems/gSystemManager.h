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

	void update(int dt);

private:
	GSystemManager();
	~GSystemManager();

	vector<GBaseSystem*>            mSystems; // тоже самое
};



template<typename S>
void GSystemManager::RegisterSystem()
{
	uint32 index = GSystem<S>::getSystemIndex();
	mSystems[index] = new S();
}
#endif //GSYSTEM_MANAGER_H

