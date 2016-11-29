#ifndef GCOUNTERCOMPONENT_H
#define GCOUNTERCOMPONENT_H

#include "gComponent.h"

class GCounterComponent : public GComponent<GCounterComponent>
{
public:
	GCounterComponent(int value): mValue(value){};
	virtual ~GCounterComponent() {};

	void setValue(int newValue) { mValue = newValue; }
	int getValue() { return mValue; }

private: 
	int mValue;
};

#endif //GCOUNTERCOMPONENT_H

