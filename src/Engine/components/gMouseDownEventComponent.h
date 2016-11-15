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

private:
	GSprite* mSpriteDown;

	Entity mEntityToNotify;
	int    mActionMask;
};
#endif //GMOUSEDOWNCOMPONENT_H

