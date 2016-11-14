#ifndef GMOUSEUPCOMPONENT_H
#define GMOUSEUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseUpEventComponent : public GComponent<GMouseUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseUpEventComponent() {};
	virtual ~GMouseUpEventComponent() {};

	void SetSpriteDown(GSprite* sprite) { mSpriteDown = sprite; }

	void SetParamsToNotify(Entity entityToNotify, int actionMask) { mEntityToNotify = entityToNotify; mActionMask = actionMask; }

private:
	GSprite* mSpriteDown;

	Entity mEntityToNotify;
	int    mActionMask;
};
#endif //GMOUSEUPCOMPONENT_H

