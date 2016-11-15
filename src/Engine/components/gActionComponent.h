#ifndef GACTIONCOMPONENT_H
#define GACTIONCOMPONENT_H

#include "gComponent.h"

typedef enum ACTIONS
{
	ACTION_BEGIN = BIT(1),
	ACTION_STOP = BIT(2)
}ACTIONS;


class GActionComponent : public GComponent<GActionComponent>
{
public:
	GActionComponent():mActionMask(0) {};
	virtual ~GActionComponent() {};

	void AddActions(uint32 actionMask) { mActionMask |= actionMask; }
	void Reset() { mActionMask = 0; }

	bool DoesContainAction(uint32 action) { return (mActionMask & action); }
	bool DoesContainAnyAction() { return mActionMask != 0; }

private: 
	int mActionMask;
};

#endif //GACTIONCOMPONENT_H

