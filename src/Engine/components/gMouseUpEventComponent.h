#ifndef GMOUSEUPCOMPONENT_H
#define GMOUSEUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseUpEventComponent : public GComponent<GMouseUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseUpEventComponent(GSprite* spriteUp) : mSpriteUp(spriteUp) {};
	virtual ~GMouseUpEventComponent() 
	{
		signal_MouseUp.disconnect_all_slots();
		signal_MouseUpOnEntity.disconnect_all_slots();
		signal_MouseUpNewSprite.disconnect_all_slots();
	};

	void SetSpriteUp(GSprite* sprite) { mSpriteUp = sprite; }

public: /*signals*/
	boost::signals2::signal<void()>                 signal_MouseUp;
	boost::signals2::signal<void(Entity)>           signal_MouseUpOnEntity;
	boost::signals2::signal<void(GSprite*)>         signal_MouseUpNewSprite;

private:
	GSprite* mSpriteUp;
};
#endif //GMOUSEUPCOMPONENT_H

