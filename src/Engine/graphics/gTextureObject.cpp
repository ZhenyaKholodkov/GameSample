#include "gTextureObject.h"

#include "gRenderManager.h"

GTextureObject::GTextureObject(const char* name) :
	mRefCounter(0),
	mGLTextureId(1),
	mWidth(0),
    mHeight(0)
{
	mKey = new char[256];
	strcpy(mKey, name);
}

GTextureObject::~GTextureObject()
{
 	delete[] mKey;
}