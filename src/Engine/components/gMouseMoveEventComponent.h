#ifndef GMOUSEMOVECOMPONENT_H
#define GMOUSEMOVECOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseMoveEventComponent : public GComponent<GMouseMoveEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseMoveEventComponent(GSprite* spriteIn, GSprite* spriteOut) : mSpriteMoveIn(spriteIn), mSpriteMoveOut(spriteOut), mMovedIn(false) {};
	virtual ~GMouseMoveEventComponent() {};

	void SetSpriteMoveIn(GSprite* sprite) { mSpriteMoveIn = sprite; }
	void SetSpriteMoveOut(GSprite* sprite) { mSpriteMoveOut = sprite; }

public: /*signals*/
	sigslot::signal2<float, float>     signal_PressedMouseMovedIn;
	sigslot::signal2<float, float>     signal_PressedMouseMovedOut;
	sigslot::signal2<Entity, GSprite*> signal_MouseMovedInEntity;
	sigslot::signal2<Entity, GSprite*> signal_MouseMovedOutEntity;

private:
	GSprite* mSpriteMoveIn;
	GSprite* mSpriteMoveOut;

	bool     mMovedIn;
};
#endif //GMOUSEMOVECOMPONENT_H

