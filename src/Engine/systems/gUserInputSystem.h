#ifndef GUSERINPUT_SYSTEM_H
#define GUSERINPUT_SYSTEM_H

#include "Utils.h"
#include "Types.h"
#include "gEntityManager.h"
#include "gSystem.h"

class GUserInputSystem : public GSystem<GUserInputSystem>
{
public:
	GUserInputSystem(std::shared_ptr<GEntityManager> manager);
	virtual ~GUserInputSystem();

	virtual void update(int dt) {};

	void OnMouseDown(GCursor point);
	void OnMouseUp(GCursor point);
	void OnMouseMove(GCursor point);
	void OnKeyUp(GKey key);
	void OnKeyDown(GKey key);
};

#endif //GUSERINPUTSYSTEMSYSTEM_SYSTEM_H

