#ifndef IGAME_H
#define IGAME_H

#include "Types.h"

class IGame
{
public:
	IGame() {}
	~IGame() {}

	static IGame* Instane();
	virtual void Create() = 0;
	virtual void Update(int dt) = 0;

	virtual void OnMouseDown(Pixel mouse_pos) = 0;
	virtual void OnMouseUp(Pixel mouse_pos) = 0;
};

#endif

