
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

	virtual void OnMouseDown(GMouse mouse_pos);
	virtual void OnMouseUp(GMouse mouse_pos);

	void LoadResources();
private:
	GSystemManager* mSystemManager;
};

#endif

