#ifndef GUSERINPUT_SYSTEM_H
#define GUSERINPUT_SYSTEM_H

#include "gEntityManager.h"
#include "Utils.h"
#include "Types.h"

class GUserInputSystem : public GSystem<GUserInputSystem>
{
public:
	GUserInputSystem();
	virtual ~GUserInputSystem();

	virtual void update(int dt) {};

	void OnMouseDown(GCursor point);
	void OnMouseUp(GCursor point);
	void OnMouseMove(GCursor point);
	void OnKeyUp(uint32 msKey);
	void OnKeyDown(uint32 msKey);

private:
	GEntityManager* mEntityManager;
};

#endif //GUSERINPUTSYSTEMSYSTEM_SYSTEM_H

