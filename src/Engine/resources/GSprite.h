#ifndef GSPRITE_H
#define GSPRITE_H

#include "gResource.h"

class GSprite : GResource
{
	friend class GResManager;

public :
	GSprite(const char* path);
	virtual ~GSprite();

	int    getXPos()   const { return mRect.mXPos;	 }
	int    getYPos()   const { return mRect.mYPos;   }
	int    getWidth()  const { return mRect.mWidth;  }
	int    getHeight() const { return mRect.mHeight; }
	float  getPivotX() const { return mRect.mPivotX; }
	float  getPivotY() const { return mRect.mPivotY; }

	int    getTextureGLId()   const { return mTextureHandle.getTextureGLId(); }
	uint32 getTextureWidth()  const { return mTextureHandle.getTextureWidth(); }
	uint32 getTextureHeight() const { return mTextureHandle.getTextureHeight(); }

	int GetTextureGLId() const { return mTextureHandle.getTextureGLId(); }

	void load();

	bool IsLoaded() const { return mIsLoaded; }

private:
	GObjectRect    mRect;
	GTextureHandle mTextureHandle; // handle of the texture
	const char*    mTextureKey;

	bool mIsLoaded;

};
#endif //GSPRITE_H