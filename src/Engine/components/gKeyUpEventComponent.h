#ifndef GKEYUPCOMPONENT_H
#define GKEYUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GKeyUpEventComponent : public GComponent<GKeyUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GKeyUpEventComponent(GSprite* spriteUp) {};
	virtual ~GKeyUpEventComponent() {};

public: /*signals*/

private:
};
#endif //GKEYUPCOMPONENT_H

