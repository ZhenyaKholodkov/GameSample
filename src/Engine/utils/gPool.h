#ifndef GPOOL_H
#define GPOOL_H

#include "Types.h"
class GBasePool
{
public:
	GBasePool() {}
	~GBasePool() {}
};

template<typename C>
class GPool //: public GBasePool
{
	struct Entry
	{
	public:
		Entry(Entity entity): mEntity(entity) {}
		Entity mEntity;
		C      mComponent;
	};
public:
	GPool(size_t capacity) :
		mCapacity(capacity)
	{
		mIndexes.resize(capacity);
		mData = mCapacity > 0 ? operator new[](mCapacity * sizeof(Entry)) : nullptr;
	}
	~GPool() {}

	//std::iterator<Entry*> Begin() { return (std::iterator<Entry*>(mSize == 0 ? nullptr : static_cast<Entry*>(mData))); }
	//std::iterator<Entry*> End()
	//{
	//	Entry* ptr = static_cast<Entry*>(mData);
	//	return (std::iterator<Entry*>(mSize == 0 ? nullptr : static_cast<Entry*>(&ptr[mSize])));
	//}

	size_t Size() { return mSize; }
	size_t Capacity() { return mCapacity; }

	void* GetComponent(Entity entity)
	{
		if (entity >= mIndexes)
			return nullptr;
		uint32 index = mIndexes[entity];
		Entry* entry = static_cast<Entry*>(mData[index]);
		return &entry->mComponent;
	}
	
	template<typename... Args>
	void* CreateComponent(Entity entity, Args&& ... args)
	{
		size_t index = mSize++;
		mIndexes[entity] = index;

		Entry* ptr = static_cast<Entry*>(mData);
		Entry* entry = &ptr[index];
		//new (entry) Entry(entity);

		entry->mEntity = entity;
		entry->mComponent = C(args...);

		return &entry->mComponent;
	}
	
	void DestroyComponent(Entity entity)
	{
		uint32 index = mIndexes[entity];

		Entry* ptr = static_cast<Entry*>(mData);
		Entry* entryToDestroy = &ptr[index];
		entryToDestroy->mComponent.~C();

		if (index != mSize - 1)
		{
			memcpy(entryToDestroy, &ptr[mSize - 1], sizeof(Entry));
			mIndexes[entryToDestroy->mEntity] = index;
		}
		--mSize;
	}

private :
	size_t mCapacity;
	size_t mSize;

	std::vector<uint32> mIndexes;
	void*               mData;
};
#endif

