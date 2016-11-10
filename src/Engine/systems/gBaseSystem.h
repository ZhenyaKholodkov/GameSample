#ifndef GBASE_SYSTEM_H
#define GBASE_SYSTEM_H

//template<class Component>
class GBaseSystem
{
public:
	GBaseSystem();
	virtual ~GBaseSystem();

	virtual void update(int dt) = 0;
};

#endif //GBASE_SYSTEM_H
