
#ifndef GGAME_H
#define GGAME_H

#include "GGameWindow.h"

class GGame: public GGameWindow, public sigslot::has_slots<>
{
public:
	GGame();
	~GGame();

	virtual bool Create();
	virtual void Update(int dt);

	virtual int32  onCreate();
	virtual void   onClose();

	virtual void onTimer(int32 dTime);
	virtual void onMouseMove(GCursor point);
	virtual void onLMouseDown(GCursor point);
	virtual void onLMouseUp(GCursor point);
	virtual void onKeyDown(GKey key);
	virtual void onKeyUp(GKey key);

	bool LoadResources();
public:/*slots*/
	void slot_Won();
	void slot_Lost();
private:
	void CreateGame();
	void CreateField();
	void CreateBomb(float x, float y);
	void CreateCoin(float x, float y);
private:
	bool            isGameOver;
};

#endif

