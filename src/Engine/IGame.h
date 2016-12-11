#ifndef IGAME_H
#define IGAME_H

#include "Types.h"

class IGame
{
public:
	IGame() {}
	~IGame() {}

	static IGame* Instane();
	virtual bool Create() = 0;
	virtual void Update(int dt) = 0;

	virtual void OnMouseDown(GCursor point) = 0;
	virtual void OnMouseUp(GCursor point) = 0;
	virtual void OnMouseMove(GCursor point) = 0;

	virtual void keyUp(GKey key) = 0;
	virtual void keyDown(GKey key) = 0;

};

#endif

