#ifndef GMOUSEDOWNCOMPONENT_H
#define GMOUSEDOWNCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GMouseDownEventComponent : public GComponent<GMouseDownEventComponent>
{
	friend class GUserInputSystem;
public:
	GMouseDownEventComponent() {};
	virtual ~GMouseDownEventComponent() {};

	void SetSpriteDown(GSprite* sprite) { mSpriteDown = sprite; }

	template<typename C> 
	void SetStateToChangeComponent(uint32 state);

private:
	GSprite* mSpriteDown;

	uint32 mComponentIndex;
	uint32 mStateToChange;
};


template<typename C>
void GMouseDownEventComponent::SetStateToChangeComponent(uint32 state)
{
	mComponentIndex = GComponent<GMouseDownEventComponent>::getComponentIndex();
	mStateToChange = state;
}
#endif //GMOUSEDOWNCOMPONENT_H

