#ifndef GKEYDOWNCOMPONENT_H
#define GKEYDOWNCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GKeyDownEventComponent : public GComponent<GKeyDownEventComponent>
{
	friend class GUserInputSystem;
public:
	GKeyDownEventComponent() {};
	virtual ~GKeyDownEventComponent() {};


public: /*signals*/
	sigslot::signal0<>  signal_KeyLeft;
	sigslot::signal0<>  signal_KeyRight;
	sigslot::signal0<>  signal_KeyUp;
	sigslot::signal0<>  signal_KeyDown;


private:
};
#endif //GKEYDOWNCOMPONENT_H

