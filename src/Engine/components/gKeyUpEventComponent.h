#ifndef GKEYUPCOMPONENT_H
#define GKEYUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GKeyUpEventComponent : public GComponent<GKeyUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GKeyUpEventComponent() {};
	virtual ~GKeyUpEventComponent() {};

public: /*signals*/
	sigslot::signal0<>  signal_KeyLeft;
	sigslot::signal0<>  signal_KeyRight;
	sigslot::signal0<>  signal_KeyUp;
	sigslot::signal0<>  signal_KeyDown;

private:
};
#endif //GKEYUPCOMPONENT_H

