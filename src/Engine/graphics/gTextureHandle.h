#ifndef TEXTUREHANDLE_H
#define TEXTUREHANDLE_H

#include "gTextureObject.h"
#include "gTextureManager.h"

class GTextureHandle
{
public:
	GTextureHandle();
	GTextureHandle(const char* texturePath);
	GTextureHandle(const GTextureHandle& handle);
	~GTextureHandle();

	int    getTextureGLId()   { return mTexture->mGLTextureId; }
	uint32 getTextureWidth()  { return mTexture->mWidth;  }
	uint32 getTextureHeight() { return mTexture->mHeight; }

	void Set(const char* texturePath);
private:
	void lock();
	void unlock();


	inline bool NotNull(void) const { return mTexture != nullptr; }
	inline bool IsNull(void) const { return mTexture == nullptr; }
private:
	GTextureObject* mTexture;
};

#endif //TEXTUREHANDLE_H