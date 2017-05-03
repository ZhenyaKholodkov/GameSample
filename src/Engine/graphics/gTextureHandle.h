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

	int    getTextureGLId()   const { return mTexture->mGLTextureId; }
	uint32 getTextureWidth()  const { return mTexture->mWidth;  }
	uint32 getTextureHeight() const { return mTexture->mHeight; }

	void Set(const char* texturePath);     // set texture
private:
	void lock();      // increases the texture reference countar 
	void unlock();    // decreases the texture reference countar and unload it if needed


	inline bool NotNull(void) const { return mTexture != nullptr; }
	inline bool IsNull(void) const { return mTexture == nullptr; }
private:
	GTextureObject* mTexture;
};

#endif //TEXTUREHANDLE_H