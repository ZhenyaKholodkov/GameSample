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

	virtual void update(int dt);

private:
	Entity createTitle(uint32 row, uint32 col, Entity self);
private:
	G2048MechanicComponent* m2048Component;
	GEntityManager* mEntityManager;
};

#endif //GRENDER_SYSTEM_H

