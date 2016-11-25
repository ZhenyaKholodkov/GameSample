#ifndef GKEYDOWNCOMPONENT_H
#define GKEYDOWNCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GKeyDownEventComponent : public GComponent<GKeyDownEventComponent>
{
	friend class GUserInputSystem;
public:
	GKeyDownEventComponent(GSprite* spriteUp) {};
	virtual ~GKeyDownEventComponent() {};


public: /*signals*/

private:
};
#endif //GKEYDOWNCOMPONENT_H

