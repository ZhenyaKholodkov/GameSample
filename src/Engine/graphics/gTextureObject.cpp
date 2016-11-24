#include "gTextureObject.h"

#include "gRenderManager.h"

GTextureObject::GTextureObject() :
	mKey(nullptr),
	mRefCounter(0),
	mGLTextureId(1),
	mWidth(0),
    mHeight(0)
{

}

GTextureObject::~GTextureObject()
{
	GRenderManager::Instance()->unloadTexture(mGLTextureId);
}