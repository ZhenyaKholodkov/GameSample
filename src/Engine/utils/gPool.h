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
	/*struct Entry
	{
	public:
		Entry(Entity entity): mEntity(entity) {}
		Entity mEntity;
		C      mComponent;
	};*/
private:
	const int DATA_SIZE = sizeof(C);
	struct Block
	{
		Entity mEntity;
		C mComponent;
	};

	const int BLOCK_SIZE = sizeof(Block);
public:
	GPool(size_t capacity) :
		mCapacity(capacity)
	{
		mIndexes.resize(capacity);
		for (int i = 0; i < capacity; ++i)
		{
			mIndexes[i] = -1;
		}
		mData = mCapacity > 0 ? operator new[](mCapacity * BLOCK_SIZE) : nullptr;
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

	C* getByEntity(Entity entity)
	{
		uint32 index = mIndexes[entity];
		if (index == -1)
			return nullptr;

		Block* ptr = static_cast<Block*>(mData);
		Block* ptrToBlock = &ptr[index];
		return &(ptrToBlock->mComponent);
	}

	Entity getEntity(uint32 index)
	{
		Block* ptr = static_cast<Block*>(mData);
		Block* ptrToBlock = &ptr[index];
		return ptrToBlock->mEntity;
	}

	template<typename... Args>
	C* create(Entity entity, Args&& ... args)
	{
		if (mIndexes[entity] != -1)
			return nullptr;

		size_t index = mSize++;
		mIndexes[entity] = index;

		Block* ptr = static_cast<Block*>(mData);
		Block* ptrToBlock = &ptr[index];
		ptrToBlock->mEntity = entity;
		new (&ptrToBlock->mComponent) C(args...);
		return &(ptrToBlock->mComponent);
	}
	
	void destroy(Entity entity)
	{
		uint32 index = mIndexes[entity];

		Block* ptr = static_cast<Block*>(mData);
		Block* ptrDestroy = &ptr[index];
		ptrDestroy->mComponent.~C();
		mIndexes[entity] = -1;

		if (index != mSize - 1)
		{
			memcpy(ptrDestroy, &ptr[mSize - 1], BLOCK_SIZE);
			mIndexes[ptrDestroy->mEntity] = index;
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

