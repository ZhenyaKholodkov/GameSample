#include "GGame.h"
#include "GResManager.h"
#include "IGRender.h"
#include "GSprite.h"
#include "entity\gEntityManager.h"


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

	GEntityManager* em = GEntityManager::Instance();

	Entity entity1 = em->CreateEntity();
	AddComponents<GLocationComponent>(5.0f, 10.0f);
}

void GGame::Update(int dt)
{
	IGRender::Instance()->setClearColor(mBackgroundColor);
	IGRender::Instance()->clear();
	IGRender::Instance()->startFrame();
	//Texture* texture = reinterpret_cast<Texture*>(GResManager::Instance()->mResources[0]);
	GSprite* sprite = GResManager::Instance()->GetSprite("button1.png");

	IGRender::Instance()->drawImage(sprite->mTextureHandle.getTextureGLId(), sprite->mTextureHandle.getTextureWidth(), sprite->mTextureHandle.getTextureHeight(),
		sprite->GetXPos(), sprite->GetYPos(), sprite->GetWidth(), sprite->GetHeight(), 0.0f, 0.0f, 0.0f);
	IGRender::Instance()->setLineColor(100, 100, 100);
	IGRender::Instance()->drawLine(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 100.0f);
	IGRender::Instance()->endFrame();
}

void GGame::LoadResources()
{
	char* buf1 = "qwerty";
	uint32 hash1 = (uint32)(((size_t)buf1) >> 2);
	int i = 9;
	char* buf2 = "qwerty";
	uint32 hash2 = (uint32)(((size_t)buf2) >> 2);

	GResManager* resManager = GResManager::Instance();
	resManager->LoadResources("data/resources/scene1/res_config.xml");
	GSprite* sprite = GResManager::Instance()->GetSprite("button1.png");
	sprite->load();
	/*
	Texture* redButtonResFile = new Texture();
	redButtonResFile->mName = "Button.png";
	redButtonResFile->mPath = "data/resources";
	redButtonResFile->mType = ResType::RES_FILE_TEXTURE;
	resManager->Add(redButtonResFile);
	resManager->LoadResource(redButtonResFile->mFileId);
	*/
}