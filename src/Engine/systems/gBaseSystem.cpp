#include "gBaseSystem.h"

uint32 GBaseSystem::s_system_counter = 0;

GBaseSystem::GBaseSystem(std::shared_ptr<GEntityManager> manager) :
	mEntityManager(manager),
	mIsStoped(false)
{
}

GBaseSystem::~GBaseSystem()
{
}