#include "GGame.h"
#include "GResManager.h"
#include "Engine\OpenGL\IGRender.h"


GGame::GGame():
	mBackgroundColor(0xff0000ff)
{
	id = -1;
}

GGame::~GGame()
{

}

IGame* IGame::Instane()
{
	static IGame* instance = new GGame();
	return instance;
}

void GGame::Create()
{
	LoadResources();
}

void GGame::Update(int dt)
{
	IGRender::Instance()->setClearColor(mBackgroundColor);
	IGRender::Instance()->clear();
	IGRender::Instance()->startFrame();
	Texture* texture = reinterpret_cast<Texture*>(GResManager::Instance()->mResources[0]);

	IGRender::Instance()->drawImage(texture->mTextureId, texture->mWeight, texture->mHeight, 0, 0, texture->mWeight, texture->mHeight, 0.0f, 0.0f, 0.0f);
	IGRender::Instance()->setLineColor(100, 100, 100);
	IGRender::Instance()->drawLine(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 100.0f);
	IGRender::Instance()->endFrame();
}

void GGame::LoadResources()
{
	GResManager* resManager = GResManager::Instance();

	Texture* redButtonResFile = new Texture();
	redButtonResFile->mName = "Button.png";
	redButtonResFile->mPath = "data/resources";
	redButtonResFile->mType = ResType::RES_FILE_TEXTURE;
	resManager->Add(redButtonResFile);
	resManager->LoadResource(redButtonResFile->mFileId);
}