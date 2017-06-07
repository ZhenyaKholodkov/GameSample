
#ifndef GGAME_H
#define GGAME_H

#include "GGameWindow.h"
#include "gMainScene.h"

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
public:/*slots*/
private:
	void CreateGame();
private:
	bool            isGameOver;

	std::unique_ptr<GMainScene> mMainScene;

	std::vector<boost::signals2::connection> mConnections;
};

#endif

