#ifndef GTEXTUREDICTINARY_H
#define GTEXTUREDICTINARY_H

#include "gTextureObject.h"
#include "Types.h"

class GTextureDictinary
{
	friend class GTextureManager;
public :
	GTextureDictinary* Instance();

	GTextureObject* find(const char* key);
	void            insert(GTextureObject* texture);
	void            remove(GTextureObject* texture);
private:
	GTextureDictinary();
	~GTextureDictinary();

	uint32          getTableID(const char* key);
	void            destroy();

private:
	GTextureObject** mTable;
	GTextureObject*  mChain;
	uint32              mTableSize;

};
#endif //GTEXTUREDICTINARY_H