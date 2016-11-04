#include "gTextureObject.h"

#include "IGRender.h"

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
	IGRender::Instance()->UnloadTexture(mGLTextureId);
}