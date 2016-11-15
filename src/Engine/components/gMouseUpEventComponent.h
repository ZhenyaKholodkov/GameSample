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

	void SetParamsToNotify(Entity entityToNotify, int actionMask) { mEntityToNotify = entityToNotify; mActionMask = actionMask; }

private:
	GSprite* mSpriteUp;

	Entity mEntityToNotify;
	int    mActionMask;
};
#endif //GMOUSEUPCOMPONENT_H

