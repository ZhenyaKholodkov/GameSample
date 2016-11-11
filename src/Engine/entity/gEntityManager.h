#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>

#include "Types.h"

#include "components\gLocationComponent.h"
#include "components\gRenderableComponent.h"
#include "systems\gRenderSystem.h"
#include "systems\gRenderSystem.h"

using namespace std;

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();

	template<typename C, typename... Args>
	void   AddComponentsToEntity(Entity entity, Args&& ... args);



private:
	GEntityManager();
	~GEntityManager();
	
private:
	Entity mFreeEntity;

	uint32 defaulEntityCount;

	vector<vector<GBaseComponent*>> mComponents; // заменить на другой контейнер
	vector<GBaseSystem*> mSystems; // тоже самое
};

template<typename C, typename... Args>
void   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::getComponentIndex();
	C* new_component = new C(std::forward<Args>(args) ...);
	if (!mComponents[index][entity])
	{
		mComponents[index][entity] = static_cast<GBaseComponent*>(new_component);
	}
}


#endif

