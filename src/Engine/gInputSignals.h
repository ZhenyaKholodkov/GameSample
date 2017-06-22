#ifndef G_INTERFACE_INPUT
#define G_INTERFACE_INPUT

#include "boost/signals2.hpp"
#include "Types.h"

class GInputSignals
{
public:
	GInputSignals() {}
	~GInputSignals() {}

public:/*signals*/
	boost::signals2::signal<void(GCursor)> signal_MouseMove;
	boost::signals2::signal<void(GCursor)> signal_LMouseDown;
	boost::signals2::signal<void(GCursor)> signal_RMouseDown;
	boost::signals2::signal<void(GCursor)> signal_LMouseUp;
	boost::signals2::signal<void(GCursor)> signal_RMouseUp;
	boost::signals2::signal<void(GCursor)> signal_MouseWheelDown;
	boost::signals2::signal<void(GCursor)> signal_MouseWheelUp;
	boost::signals2::signal<void(GKey)>    signal_KeyDown;
	boost::signals2::signal<void(GKey)>    signal_KeyUp;
};

#endif // G_INTERFACE_INPUT