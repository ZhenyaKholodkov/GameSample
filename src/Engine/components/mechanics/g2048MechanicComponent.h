#ifndef G2048MECHANICCOMPONENT_H
#define G2048MECHANICCOMPONENT_H

#include "gComponent.h"

class G2048MechanicComponent : public GComponent<G2048MechanicComponent>, public sigslot::has_slots<>
{
	friend class G2048MechanicSystem;
public:
	G2048MechanicComponent(uint32 rows, uint32 cols) : mRows(rows), mCols(cols), mFieldWidth(0),
		mFieldHieght(0), mTitleWidth(0), mTitleHieght(0), mTitlesInMoving(0), mState(STATE_WAIT)
	{
		mTitleSprites.resize(10); //2048
		mTitles = new Entity[mRows * mCols];
		mLogicalNet = new int[mRows * mCols];
	};
	virtual ~G2048MechanicComponent() {	};

	void setTitleNumberSprite(uint32 exponent, GSprite* sprite)
	{
		mTitleSprites[exponent - 1] = sprite;
	}

	void setTitleBackground(GSprite* sprite)
	{
		mTitleBackground = sprite;
		mFieldWidth = mRows * sprite->getWidth();
		mFieldHieght = mCols * sprite->getHeight();
		mTitleWidth = sprite->getWidth();
		mTitleHieght = sprite->getHeight();
	}
private:
	void setState(uint32 state)
	{
		mState = state;
	}
public:/*slots*/
	void slot_ReportMovingFinished(Entity entity)
	{
		if (mTitlesInMoving != 0)
		{
			mTitlesInMoving--;
			if (mTitlesInMoving == 0)
			{
				setState(STATE_RECALC_MATRIX);
			}
		}
	}

private: 
	enum
	{
		STATE_WAIT,
		STATE_ANIMATING,
		STATE_RECALC_MATRIX
	};
	uint32 mRows;
	uint32 mCols;

	uint32 mFieldWidth;
	uint32 mFieldHieght;

	uint32 mTitleWidth;
	uint32 mTitleHieght;

	GSprite* mTitleBackground;
	std::vector<GSprite*> mTitleSprites; //!< 

	Entity* mTitles;
	int*    mLogicalNet;

	uint32 mTitlesInMoving;
	uint32 mState;
};

#endif //G2048MECHANICCOMPONENT_H

