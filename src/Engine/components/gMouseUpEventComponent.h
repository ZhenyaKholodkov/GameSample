#ifndef GMOUSEUPCOMPONENT_H
#define GMOUSEUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseUpEventComponent : public GComponent<GMouseUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseUpEventComponent(GSprite* spriteUp) : mSpriteUp(spriteUp) {};
	virtual ~GMouseUpEventComponent() {};

	void SetSpriteUp(GSprite* sprite) { mSpriteUp = sprite; }

public: /*signals*/
	sigslot::signal0<>                 signal_MouseUp;
	sigslot::signal1<Entity>           signal_MouseUpOnEntity;
	sigslot::signal2<Entity, GSprite*> signal_MouseUpNewSprite;

private:
	GSprite* mSpriteUp;
};
#endif //GMOUSEUPCOMPONENT_H

