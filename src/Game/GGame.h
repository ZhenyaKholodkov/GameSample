
#ifndef GGAME_H
#define GGAME_H

#include "IGame.h"

#include "gEntityManager.h"
#include "gSystemManager.h"
#include "gRenderSystem.h"

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

private:
	GSystemManager* mSystemManager;
};

#endif

