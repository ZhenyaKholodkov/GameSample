#ifndef GBASE_SYSTEM_H
#define GBASE_SYSTEM_H

#include <memory>

#include "Utils.h"
#include "GEntityManager.h"

class GBaseSystem
{
	friend class GSystemManager;
public:
	GBaseSystem(std::shared_ptr<GEntityManager> manager);
	virtual ~GBaseSystem();

	virtual void update(int dt) = 0;
public:/*slots*/
	boost::signals2::signal<void()>::slot_type slot_Stop = boost::bind(&GBaseSystem::setStop, this, true);
	boost::signals2::signal<void()>::slot_type slot_Continue = boost::bind(&GBaseSystem::setStop, this, false);
protected:
	static uint32 s_system_counter;

	std::shared_ptr<GEntityManager> mEntityManager;
protected:
	bool isStoped() const { return mIsStoped; }
	void setStop(bool stop) { mIsStoped = stop; }
private:
	bool mIsStoped;
};

#endif //GBASE_SYSTEM_H

