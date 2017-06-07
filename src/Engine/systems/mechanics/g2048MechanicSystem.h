#ifndef G2048MECHANIC_SYSTEM_H
#define G2048MECHANIC_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "g2048MechanicComponent.h"
#include "gScalableComponent.h"

class G2048MechanicSystem : public GSystem<G2048MechanicSystem>
{
public:
	G2048MechanicSystem();
	~G2048MechanicSystem();

	void createField();
	void runGame();

	void showRandomTitle();
	void showTitle(int i, int j, int a);
	void moveTitleToTitle(Entity movedTitle, Entity toTitle, int newValue);
	void recalFieldAfterAnimation();

	virtual void update(int dt);

public:/*signals*/

public:/*slots*/
	void slot_MoveLeft();
	void slot_MoveRight();
	void slot_MoveTop();
	void slot_MoveBottom();

private:
	Entity createTitle(uint32 row, uint32 col, Entity self);

	void moveField(uint32 direction);
private:
	enum
	{
		DIRECTION_LEFT = BIT(1),
		DIRECTION_RIGHT = BIT(2),
		DIRECTION_TOP = BIT(3),
		DIRECTION_BOTTOM = BIT(4),
	};
	GEntityManager* mEntityManager;
	G2048MechanicComponent* mCurrentMechanic;
};

#endif //GRENDER_SYSTEM_H

