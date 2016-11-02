#ifndef TEXTUREHANDLE_H
#define TEXTUREHANDLE_H

#include "TextureObject.h"

class TextureHandle
{
public:
	TextureHandle();
	TextureHandle(const char* texturePath);
	TextureHandle(const TextureHandle& handle);
	~TextureHandle();

private:
	void lock();
	void unlock();


	inline bool NotNull(void) const { return mTexture != nullptr; }
	inline bool IsNull(void) const { return mTexture == nullptr; }
private:
	TextureObject* mTexture;
};

#endif //TEXTUREHANDLE_H