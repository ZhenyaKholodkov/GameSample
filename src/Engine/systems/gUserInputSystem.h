#ifndef GUSERINPUT_SYSTEM_H
#define GUSERINPUT_SYSTEM_H

#include "Utils.h"
#include "Types.h"
#include "gEntityManager.h"
#include "gSystem.h"
#include "gInputSignals.h"

class GUserInputSystem : public GSystem<GUserInputSystem>
{
public:
	GUserInputSystem(std::shared_ptr<GEntityManager> manager, GInputSignals* inputs);
	virtual ~GUserInputSystem();

	virtual void update(int dt) {};

	void OnMouseDown(GCursor point);
	void OnMouseUp(GCursor point);
	void OnMouseMove(GCursor point);
	void OnKeyUp(GKey key);
	void OnKeyDown(GKey key);
private:
	const boost::signals2::signal<void(GCursor)>::slot_type slot_OnMouseDown = boost::bind(&GUserInputSystem::OnMouseDown, this, _1);
	const boost::signals2::signal<void(GCursor)>::slot_type slot_OnMouseUp = boost::bind(&GUserInputSystem::OnMouseUp, this, _1);
	const boost::signals2::signal<void(GCursor)>::slot_type slot_OnMouseMove = boost::bind(&GUserInputSystem::OnMouseMove, this, _1);
	const boost::signals2::signal<void(GKey)>::slot_type    slot_OnKeyUp = boost::bind(&GUserInputSystem::OnKeyUp, this, _1);
	const boost::signals2::signal<void(GKey)>::slot_type    slot_OnKeyDown = boost::bind(&GUserInputSystem::OnKeyDown, this, _1);

};

#endif //GUSERINPUTSYSTEMSYSTEM_SYSTEM_H

