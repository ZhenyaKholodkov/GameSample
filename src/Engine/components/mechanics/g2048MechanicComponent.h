#ifndef G2048MECHANICCOMPONENT_H
#define G2048MECHANICCOMPONENT_H

#include "gComponent.h"

class G2048MechanicComponent : public GComponent<G2048MechanicComponent>, public sigslot::has_slots<>
{
	friend class G2048MechanicSystem;
public:
	G2048MechanicComponent(uint32 rows, uint32 cols) : mRows(rows), mCols(cols), mFieldWidth(0),
		mFieldHieght(0), mTitleWidth(0), mTitleHieght(0), titlesInMoving(0)
	{
		mTitleSprites.resize(10); //2048
		mTitles = new Entity*[mRows];
		mLogicalNet = new int*[mRows];
		for (uint32 i = 0; i < mRows; ++i)
		{
			mTitles[i] = new Entity[mCols];
			mLogicalNet[i] = new int[mCols];
		}
	};
	virtual ~G2048MechanicComponent() {	};

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
public:/*slots*/
	void slot_ReportMovingFinished()
	{
		if (titlesInMoving != 0)
			titlesInMoving--;
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
	int**    mLogicalNet;

	uint32 titlesInMoving;
};

#endif //G2048MECHANICCOMPONENT_H

