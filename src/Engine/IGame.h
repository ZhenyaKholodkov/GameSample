#ifndef IGAME_H
#define IGAME_H

class IGame
{
public:
	IGame() {}
	~IGame() {}

	static IGame* Instane();
	virtual void Create() = 0;
	virtual void Update(int dt) = 0;
};

#endif

