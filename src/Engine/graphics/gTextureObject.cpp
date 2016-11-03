#include "gTextureObject.h"


GTextureObject::GTextureObject() :
	mKey(nullptr),
	mRefCounter(0),
	mGLTextureId(1)
{

}

GTextureObject::~GTextureObject()
{

}