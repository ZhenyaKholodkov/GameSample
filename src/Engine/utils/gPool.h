#ifndef GPOOL_H
#define GPOOL_H

template<class C>
class GPool
{
public:
	GPool();
	~GPool();
private :
	C* data;
};

#endif

