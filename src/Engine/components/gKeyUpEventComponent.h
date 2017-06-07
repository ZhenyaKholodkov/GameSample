#ifndef GKEYUPCOMPONENT_H
#define GKEYUPCOMPONENT_H

#include "gComponent.h"
#include "GSprite.h"

class GKeyUpEventComponent : public GComponent<GKeyUpEventComponent>
{
	friend class GUserInputSystem;
public:
	GKeyUpEventComponent() {};
	virtual ~GKeyUpEventComponent()
	{
		signal_KeyLeft.disconnect_all_slots();
		signal_KeyRight.disconnect_all_slots();
		signal_KeyUp.disconnect_all_slots();
		signal_KeyDown.disconnect_all_slots();
	};

public: /*signals*/
	boost::signals2::signal<void()>  signal_KeyLeft;
	boost::signals2::signal<void()>  signal_KeyRight;
	boost::signals2::signal<void()>  signal_KeyUp;
	boost::signals2::signal<void()>  signal_KeyDown;

private:
};
#endif //GKEYUPCOMPONENT_H

