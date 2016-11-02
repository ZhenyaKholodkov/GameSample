
#ifndef GGAME_H
#define GGAME_H

#include "IGame.h"

class GGame: public IGame
{
public:
	GGame();
	~GGame();

	virtual void Create();
	virtual void Update(int dt);

	int id;
	unsigned int mBackgroundColor;
	void LoadResources();
};

#endif

