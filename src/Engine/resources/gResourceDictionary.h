#ifndef GRESOURCEDICTIONARY_H
#define GRESOURCEDICTIONARY_H

#include "gResource.h"

class GResourceDictionary
{
	friend class GTextureManager;
public:
	static GResourceDictionary* Instance();

	GResource*      find(const char* key);
	void            insert(GResource* resource);
	void            remove(GResource* resource);

private:
	GResourceDictionary();
	~GResourceDictionary();

	uint32          getTableID(const char* key);

private:
	GResource** mTable;
	GResource*  mChain;
	uint32              mTableSize;

};
#endif //GRESOURCE_H