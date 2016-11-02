#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

class TextureObject
{
	friend class TextureHandle;
private:
	TextureObject();
	~TextureObject();

private:
	int mRefCounter;  // count of refernces on this texture
	int mGLTextureId; // id of the texture
};
#endif //TEXTUREOBJECT_H