#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include "Types.h"

class GTextureObject
{
	friend class GTextureHandle;
	friend class GTextureDictinary;
	friend class GTextureManager;
private:
	GTextureObject();
	~GTextureObject();

private:
	GTextureObject* mNext;
	GTextureObject* mPrev;
	GTextureObject* mSameHash;

	const char*     mKey;

	int mRefCounter;  // count of refernces on this texture
	int mGLTextureId; // id of the texture

	ulong32 mWidth;
	ulong32 mHeight;
};
#endif //TEXTUREOBJECT_H