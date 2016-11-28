#ifndef G2048MECHANICCOMPONENT_H
#define G2048MECHANICCOMPONENT_H

#include "gComponent.h"

class G2048MechanicComponent : public GComponent<G2048MechanicComponent>
{
	friend class G2048MechanicSystem;
public:
	G2048MechanicComponent(uint32 rows, uint32 cols) : mRows(rows), mCols(cols)
	{
		mTitleSprites.resize(10); //2048
		mTitles = new Entity*[mRows];
		for (uint32 i = 0; i < mRows; ++i)
		{
			mTitles[i] = new Entity[mCols];
		}
	};
	virtual ~G2048MechanicComponent() 
	{
		//sfor (uint32 i = 0; i < mRows; ++i)
		//s{
		//s	for (uint32 j = 0; j < mCols; ++j)
		//s	{
		//s		GEntityManager::Instance()->DestroyEntity(mTitles[i][j]);
		//s	}
		//s}
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
		mTitleWidth = sprite->GetWidth();
		mTitleHieght = sprite->GetHeight();
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

	std::vector<Entity> mAvailableEntities;

	Entity** mTitles;
};

#endif //G2048MECHANICCOMPONENT_H

