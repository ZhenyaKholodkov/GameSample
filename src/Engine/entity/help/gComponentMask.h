#ifndef G_COMPONENTMASK
#define G_COMPONENTMASK

#include "Types.h"

class ComponentMask
{
public:
	ComponentMask() : mMask(0) {}
	ComponentMask(const ComponentMask& mask) : mMask(mask.mMask) {}
	ComponentMask(uint32 componentId) : mMask(1 << componentId) {}

	bool empty() const { return mMask == 0; }
	bool contains(const ComponentMask& mask) const { return mMask == mask.mMask; }

	ComponentMask& operator=(const ComponentMask& right) { this->mMask = right.mMask; return *this; }
	bool operator==(ComponentMask& right) const { return this->mMask == right.mMask; }
	bool operator!=(ComponentMask& right) const { return this->mMask != right.mMask; }
	bool operator==(uint32 id)            const { return this->mMask == (1 << id); }
	bool operator!=(uint32 id)            const { return this->mMask != (1 << id); }
	bool operator&(const ComponentMask& right)  const { return this->mMask & right.mMask; }
	bool operator&(uint32 id)             const { return (this->mMask & (1 << id)) != 0; }

	ComponentMask& operator+=(ComponentMask& right) { mMask |= right.mMask; return *this; }
	ComponentMask& operator+=(uint32 id) { mMask |= (1 << id); return *this; }
	ComponentMask& operator-=(ComponentMask& right) { mMask &= ~right.mMask; return *this; }
	ComponentMask& operator-=(uint32 id) { mMask &= ~(1 << id); return *this; }
private:
	uint32 mMask;
};
#endif //G_COMPONENTMASK