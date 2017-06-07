#ifndef GMOUSEMOVECOMPONENT_H
#define GMOUSEMOVECOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseMoveEventComponent : public GComponent<GMouseMoveEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseMoveEventComponent(GSprite* spriteIn, GSprite* spriteOut) : mSpriteMoveIn(spriteIn), mSpriteMoveOut(spriteOut), mMovedIn(false) {};
	virtual ~GMouseMoveEventComponent() 
	{
		signal_PressedMouseMovedIn.disconnect_all_slots();
		signal_PressedMouseMovedOut.disconnect_all_slots();
		signal_MouseMovedInEntity.disconnect_all_slots();
		signal_MouseMovedOutEntity.disconnect_all_slots();
	};

	void SetSpriteMoveIn(GSprite* sprite) { mSpriteMoveIn = sprite; }
	void SetSpriteMoveOut(GSprite* sprite) { mSpriteMoveOut = sprite; }

public: /*signals*/
	boost::signals2::signal<void(float, float)>     signal_PressedMouseMovedIn;
	boost::signals2::signal<void(float, float)>     signal_PressedMouseMovedOut;
	boost::signals2::signal<void(GSprite*)>         signal_MouseMovedInEntity;
	boost::signals2::signal<void(GSprite*)>         signal_MouseMovedOutEntity;

private:
	GSprite* mSpriteMoveIn;
	GSprite* mSpriteMoveOut;

	bool     mMovedIn;
};
#endif //GMOUSEMOVECOMPONENT_H

