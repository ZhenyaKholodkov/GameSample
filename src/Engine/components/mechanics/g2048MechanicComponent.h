#ifndef G2048MECHANICCOMPONENT_H
#define G2048MECHANICCOMPONENT_H

#include "gComponent.h"

class G2048MechanicComponent : public GComponent<G2048MechanicComponent>
{
	friend class G2048MechanicSystem;
public:
	G2048MechanicComponent(uint32 rows, uint32 cols) : mRows(rows), mCols(cols)
	{
		mTitles = new Entity*[mRows];
		for (uint32 i = 0; i < mRows; ++i)
		{
			mTitles[i] = new Entity[mCols];
		}
		mTitleSprites.resize(10); //2048
	};
	virtual ~G2048MechanicComponent() 
	{
		GEntityManager::Instance()->DestroyEntity();
	};

	void setTitleNumberSprite(uint32 exponent, GSprite* sprite)
	{
		mTitleSprites[exponent - 1] = sprite;
	}

	void setTitleBackground(GSprite* sprite)
	{
		mTitleBackground = sprite;
		mFieldWidth = mRows * sprite->GetWidth();
		mFieldHieght = mCols * sprite->GetHeight();
		mTitleWidth = sprite->GetWidth() / 2;
		mTitleHieght = sprite->GetHeight() / 2;
	}

private: 
	uint32 mRows;
	uint32 mCols;

	uint32 mFieldWidth;
	uint32 mFieldHieght;

	uint32 mTitleWidth;
	uint32 mTitleHieght;

	GSprite* mTitleBackground;
	std::vector<GSprite*> mTitleSprites; //!< 

	Entity** mTitles;
};

#endif //G2048MECHANICCOMPONENT_H

