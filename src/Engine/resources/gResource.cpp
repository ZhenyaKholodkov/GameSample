#include "gResource.h"

GResource::GResource(const char* path, ResourceType type):
	mType(type)
{
	mKey = new char[256];
	strcpy(mKey, path);
}

GResource::~GResource()
{
	delete [] mKey;
}