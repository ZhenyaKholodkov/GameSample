
#ifndef GGAME_H
#define GGAME_H

#include "IGame.h"

#include "gEntityManager.h"
#include "gSystemManager.h"

class GGame: public IGame
{
public:
	GGame();
	~GGame();

	virtual void Create();
	virtual void Update(int dt);

	virtual void OnMouseDown(GCursor point);
	virtual void OnMouseUp(GCursor point);
	virtual void OnMouseMove(GCursor point);

	void LoadResources();
private:
	void Create2048Game();
private:
	GSystemManager* mSystemManager;
};

#endif

