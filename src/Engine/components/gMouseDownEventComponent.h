#ifndef GMOUSEDOWNCOMPONENT_H
#define GMOUSEDOWNCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseDownEventComponent : public GComponent<GMouseDownEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseDownEventComponent(GSprite* spriteDown) : mSpriteDown(spriteDown) {};
	virtual ~GMouseDownEventComponent() {};

	void SetSpriteDown(GSprite* sprite) { mSpriteDown = sprite; }

	void SetParamsToNotify(Entity entityToNotify, int actionMask) { mEntityToNotify = entityToNotify; mActionMask = actionMask; }

public: /*signals*/
	sigslot::signal0<>                 signal_MouseDown;
	sigslot::signal1<Entity>           signal_MouseDownOnEntity;
	sigslot::signal2<Entity, GSprite*> signal_MouseDownNewSprite;

private:
	GSprite* mSpriteDown;

	Entity mEntityToNotify;
	int    mActionMask;
};
#endif //GMOUSEDOWNCOMPONENT_H

