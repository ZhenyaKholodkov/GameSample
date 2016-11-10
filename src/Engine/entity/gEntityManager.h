#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>

#include "Types.h"

#include "components\gLocationComponent.h"
#include "components\gRenderableComponent.h"
#include "systems\gRenderSystem.h"
#include "systems\gRenderSystem.h"

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();

	template<typename Component, typename ...Args> 
	void   AddComponentsToEntity(Entity entity, Args && ... args);


private:
	GEntityManager();
	~GEntityManager();
	
private:
	Entity mFreeEntity;

	uint32 defaulEntityCount;

	vector<vector<GBaseComponent*>> mComponents; // заменить на другой контейнер
	vector<GBaseSystem*> mSystems; // тоже самое
};

template<typename C, typename ... Args>
void   AddComponents(Args && ... args)
{
	//uint32 index = GComponent<C>::getComponentIndex();
	C* c = new C(std::forward<Args>(args) ...);
	/*if (!mComponents[index][entity])
	{
	mComponents[index][entity] = new C(std::forward(srgs) ...);
	}*/
}

#endif

