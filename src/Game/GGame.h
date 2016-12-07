
#ifndef GGAME_H
#define GGAME_H

#include "IGame.h"

#include "gEntityManager.h"
#include "gSystemManager.h"

class GGame: public IGame, public sigslot::has_slots<>
{
public:
	GGame();
	~GGame();

	virtual void Create();
	virtual void Update(int dt);

	virtual void OnMouseDown(GCursor point);
	virtual void OnMouseUp(GCursor point);
	virtual void OnMouseMove(GCursor point);
	virtual void keyUp(GKey key);
	virtual void keyDown(GKey key);

	void LoadResources();
public:/*slots*/
	void slot_Won();
	void slot_Lost();
private:
	void Create2048Game();
	void CreateGame();
	void CreateField();
	void CreateBomb(float x, float y);
	void CreateCoin(float x, float y);
private:
	GSystemManager* mSystemManager;
	GEntityManager* mEntityManager;

	bool isGameOver;
};

#endif

