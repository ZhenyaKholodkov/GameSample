#ifndef GSPRITE_H
#define GSPRITE_H

#include "gResource.h"

class GSprite : GResource
{
	friend class GResManager;

public :
	GSprite(const char* path, std::shared_ptr<GTextureManager> manager);
	virtual ~GSprite();

	float  getXPos()   const { return static_cast<float>(mRect.mXPos);   }
	float  getYPos()   const { return static_cast<float>(mRect.mYPos);   }
	float  getWidth()  const { return static_cast<float>(mRect.mWidth);  }
	float  getHeight() const { return static_cast<float>(mRect.mHeight); }
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