#ifndef GMOUSEDOWNCOMPONENT_H
#define GMOUSEDOWNCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseDownEventComponent : public GComponent<GMouseDownEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseDownEventComponent(GSprite* spriteDown) : mSpriteDown(spriteDown) {};
	virtual ~GMouseDownEventComponent()
	{
		signal_MouseDown.disconnect_all_slots();
		signal_MouseDownOnEntity.disconnect_all_slots();
		signal_MouseDownNewSprite.disconnect_all_slots();
	};

	void SetSpriteDown(GSprite* sprite) { mSpriteDown = sprite; }

	void SetParamsToNotify(Entity entityToNotify, int actionMask) { mEntityToNotify = entityToNotify; mActionMask = actionMask; }

public: /*signals*/
	boost::signals2::signal<void()>                 signal_MouseDown;
	boost::signals2::signal<void(Entity)>           signal_MouseDownOnEntity;
	boost::signals2::signal<void(GSprite*)>         signal_MouseDownNewSprite;

private:
	GSprite* mSpriteDown;

	Entity mEntityToNotify;
	int    mActionMask;
};
#endif //GMOUSEDOWNCOMPONENT_H

