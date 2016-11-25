#ifndef GMOUSEMOVECOMPONENT_H
#define GMOUSEMOVECOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseMoveEventComponent : public GComponent<GMouseMoveEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseMoveEventComponent(GSprite* spriteUp) : mSpriteMove(spriteUp) {};
	virtual ~GMouseMoveEventComponent() {};

	void SetSpriteUp(GSprite* sprite) { mSpriteMove = sprite; }

public: /*signals*/
	sigslot::signal2<float, float>     signal_MouseMove;
	sigslot::signal2<float, float>     signal_PressedMouseMove;
	sigslot::signal2<Entity, GSprite*> signal_MouseMoveOnEntity;

private:
	GSprite* mSpriteMove;
};
#endif //GMOUSEMOVECOMPONENT_H

