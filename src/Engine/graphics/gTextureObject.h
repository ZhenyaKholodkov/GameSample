#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

class GTextureObject
{
	friend class GTextureHandle;
	friend class GTextureDictinary;
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
};
#endif //TEXTUREOBJECT_H