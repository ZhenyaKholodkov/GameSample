#ifndef G2048MECHANIC_SYSTEM_H
#define G2048MECHANIC_SYSTEM_H

#include "gSystem.h"
#include "gEntityManager.h"
#include "g2048MechanicComponent.h"
#include "gScalableComponent.h"

class G2048MechanicSystem : public GSystem<G2048MechanicSystem>, public sigslot::has_slots<>
{
public:
	G2048MechanicSystem();
	~G2048MechanicSystem();

	void createField();
	void runGame();

	void showRandomTitle(G2048MechanicComponent* component);

	virtual void update(int dt);

public:/*signals*/
	sigslot::signal0<> signal_GameLost;
	sigslot::signal0<> signal_GameWon;

public:/*slots*/
	void slot_MoveLeft();

private:
	Entity createTitle(uint32 row, uint32 col, Entity self, G2048MechanicComponent* m2048Component);
private:
	GEntityManager* mEntityManager;
};

#endif //GRENDER_SYSTEM_H

