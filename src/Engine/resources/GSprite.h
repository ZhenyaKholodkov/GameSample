#ifndef GSPRITE_H
#define GSPRITE_H

#include "gResource.h"

class GSprite : GResource
{
	friend class GResManager;

public :
	GSprite(const char* path);
	virtual ~GSprite();

	int   GetXPos()   { return mXPos;	}
	int   GetYPos()   { return mYPos;   }
	int   GetWidth()  { return mWidth;  }
	int   GetHeight() { return mHeight; }
	float GetPivotX() { return mPivotX; }
	float GetPivotY() { return mPivotY; }

	int GetTextureGLId() { return mTextureHandle.getTextureGLId(); }

	GTextureHandle mTextureHandle; // should be private

	void load();

	bool IsLoaded() { return mIsLoaded; }

private:
	int   mXPos;
	int   mYPos;
	int   mWidth;
	int   mHeight;
	float mPivotX;
	float mPivotY;

	const char* mTextureKey;

	bool mIsLoaded;

};
#endif //GSPRITE_H