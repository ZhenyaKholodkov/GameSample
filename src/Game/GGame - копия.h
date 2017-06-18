
#ifndef GGAME_H
#define GGAME_H

#include "GGameWindow.h"

class GGame: public GGameWindow
{
public:
	GGame();
	~GGame();

	virtual bool Create();

	virtual int32  onCreate();
	virtual void   onClose();

	virtual void onMouseMove(GCursor point);
	virtual void onLMouseDown(GCursor point);
	virtual void onLMouseUp(GCursor point);
	virtual void onKeyDown(GKey key);
	virtual void onKeyUp(GKey key);

	bool LoadResources();
	void won();
	void lost();
public:/*slots*/
	boost::function<void()> slot_Won  = boost::bind(&GGame::won, this);
	boost::function<void()> slot_Lost = boost::bind(&GGame::lost, this);
private:
	void CreateGame();
	void CreateField();
	void CreateBomb(float x, float y);
	void CreateCoin(float x, float y);
private:
	bool            isGameOver;


	std::vector<boost::signals2::connection> mConnections;
};

#endif

