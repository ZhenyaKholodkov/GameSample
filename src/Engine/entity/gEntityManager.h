#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

class GEntityManager
{
public:
	static GEntityManager* Instance();
private:
	GEntityManager();
	~GEntityManager();
};

#endif

