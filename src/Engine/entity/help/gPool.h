#ifndef GPOOL_H
#define GPOOL_H

#include "Types.h"
#include "GDefines.h"

class GPoolPairIterator;

class GBasePool                 // base class for pool. Contains the data and method for reserving new chunks of memory and getting the blocks of memory.
{
public:
	GBasePool()
	{
	}

	virtual ~GBasePool() 
	{
		for (auto chunk : mData)
		{
			delete [] chunk;
		}
	}

	virtual uint32 create() = 0;
	virtual void*  getComponent(uint32 index) const = 0;
	virtual void   destroy(uint32 index) = 0;
	
	size_t size()     const { return mSize; }
	size_t capacity() const { return mCapacity; }
protected:
	void init(size_t chunkSize, size_t dataSize)
	{
		mChunkSize = chunkSize;
		mBlockSize = dataSize;
		mCapacity = 0;
		mSize = 0;
	}
	void reserve(size_t size)
	{
		size_t newSize = mCapacity + size;
		while (mCapacity < newSize)
		{
			char* newChunk = new char[(mChunkSize * mBlockSize)];
			mData.push_back(newChunk);
			mCapacity += mChunkSize;
		}
	}

	void* get(uint32 index) const
	{
		return (mData[index / mChunkSize] + (index % mChunkSize) * mBlockSize);
	}

protected:
	std::size_t mChunkSize;
	std::size_t mBlockSize;
	std::size_t mCapacity;
	std::size_t mSize;

	std::vector<char*> mData;
};

template<typename C, typename... Args>
class GComponentPool : public GBasePool      // class for storing components
{
private:
	struct Block       // block of memory for storing in the pool
	{
		bool   mInUse;         // is it used(active) 
		uint32 mNextFree;      // next free block. If the block is used, mNextFree point to himself
		C      mComponent;     // component
	};

	const int BLOCK_SIZE = sizeof(Block);
public:
	/*class GPoolPairIterator : public std::iterator<std::forward_iterator_tag, std::pair<Entity, C*>>
	{
	private:
		GBasePool* mPool;
		uint32     mIndex;
		typedef std::pair<Entity, C*> ValuePair;
		mutable ValuePair pair;

	public:
		GPoolPairIterator() : mPool(nullptr), mIndex(0)
		{
		}
		GPoolPairIterator(GBasePool* pool, uint32 index)
		{
			mPool = pool;
			mIndex = index;
		}
		GPoolPairIterator& operator=(const GPoolIterator& iter)
		{
			mPool = iter.mPool;
			mIndex = iter.mIndex;
			return *this;
		}
		ValuePair* operator*() const
		{
			if (!mPool)
				return nullptr;
			auto ptrToBlock = static_cast<Block*>(mPool->get(mIndex));
			pair.first = ptrToBlock->mEntity;
			pair.second = &ptrToBlock->mComponent;
			return &pair;
		}
		const GPoolPairIterator& operator++()
		{
			if (mPool)
				++mIndex;
			return *this;
		}

		const GPoolPairIterator operator++(int)
		{
			GPoolPairIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator==(const GPoolPairIterator& iter)
		{
			return mIndex == iter.mIndex;
		}
		bool operator!=(const GPoolPairIterator& iter)
		{
			return mIndex != iter.mIndex;
		}

	};*/

	GComponentPool(size_t chunkSize = 5) :
		GBasePool()
	{
		init(chunkSize, sizeof(Block));
		mLastFree = 0;
	}
	~GComponentPool() 
	{
		for (int i = 0; i < mCapacity; ++i)
		{
			destroy(i);
		}
	}

	/*virtual GPoolPairIterator begin() const
	{
		GPoolPairIterator iter(const_cast<GComponentPool<C>*>(this), 0);
		return iter;
	}

	virtual GPoolPairIterator end() const
	{
		GPoolPairIterator iter(const_cast<GComponentPool<C>*>(this), mSize);
		return iter;
	}*/

	/*C* getComponent(Entity entity) const
	{
		if (entity >= mIndexes.size())
			return nullptr;
		uint32 index = mIndexes[entity];
		if (index == -1)
			return nullptr;

		auto ptrToBlock = static_cast<Block*>(get(index));
		return &(ptrToBlock->mComponent);
	}*/

	virtual void* getComponent(uint32 index) const
	{
		if (index >= mCapacity)
			return nullptr;
		auto ptrToBlock = static_cast<Block*>(get(index));
		return &(ptrToBlock->mComponent);
	}

	/*Entity getEntity(uint32 index) const
	{
		auto ptrToBlock = static_cast<const Block*>(get(index));
		return ptrToBlock->mEntity;
	}*/

	virtual uint32 create()
	{
		if (mSize == mCapacity)
		{
			reserve(mChunkSize);
			initBlocks();
			mLastFree = mSize;
		}
		size_t index = mLastFree;
		auto ptrToBlock = static_cast<Block*>(get(index));
		mLastFree = ptrToBlock->mNextFree != index ? ptrToBlock->mNextFree : -1;
		ptrToBlock->mNextFree = index;
		ptrToBlock->mInUse = true;
		mSize++;

		return index;
	}

	/*template<typename... Args>
	C* create(Entity entity, Args&& ... args)
	{
		size_t s = sizeof(Block);
		if (entity >= mIndexes.size())
			return nullptr;

		if (mIndexes[entity] != -1)
			return nullptr;

		if (mSize == mCapacity)
		{
			reserve(mChunkSize);
			initBlocks();
			mLastFree = mSize;
		}
		size_t index = mLastFree;
		mIndexes[entity] = index;

		auto ptrToBlock = static_cast<Block*>(get(index));

		ptrToBlock->mEntity = entity;
		mLastFree = ptrToBlock->mNextFree != index ? ptrToBlock->mNextFree : -1;
		ptrToBlock->mNextFree = index;
		ptrToBlock->mInUse = true;
		mSize++;

		C* component = getComponent(entity);

		new (component) C(args...);
		return &(ptrToBlock->mComponent);
	}*/
	
	virtual void   destroy(uint32 index)
	{
		assert(index <= mCapacity);
		auto ptrDestroy = static_cast<Block*>(get(index));
		if (ptrDestroy->mInUse)
		{
			ptrDestroy->mNextFree = mLastFree;
			mLastFree = index;
			ptrDestroy->mInUse = false;

			ptrDestroy->mComponent.~C();
			--mSize;
		}
	}
private:
	void initBlocks()
	{
		for (uint32 i = mSize; i < mCapacity; ++i)
		{
			auto ptrToBlock = reinterpret_cast<Block*>(get(i));
			ptrToBlock->mInUse = false;
			ptrToBlock->mNextFree = i < (mCapacity - 1) ? i + 1 : i;
		}
	}

private:
	int              mLastFree;      // last free entity. If it -1 , chunks are over and we need to resorve the new one
	//std::vector<int> mIndexes;       // indexes of components, which are owned by entities. 
	                                 // for example, the entity with identifier 3 has component with 4 index in the pool. 
	                                 // then mIndexes[3] == 4
};
#endif

