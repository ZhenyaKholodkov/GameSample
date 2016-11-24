#ifndef GPOOL_H
#define GPOOL_H

#include "Types.h"
#include "GDefines.h"
class GBasePool
{
public:
	GBasePool() {}
	virtual ~GBasePool() {}

	virtual size_t Size() = 0;
	virtual size_t Capacity() = 0;
};

template<typename C>
class GPool : public GBasePool
{
private:
	struct Block
	{
		Entity mEntity;
		C mComponent;
	};

	const int BLOCK_SIZE = sizeof(Block);
public:
	GPool(size_t capacity = 5, size_t chunkSize = 5) :
		mCapacity(capacity),
		mChunkSize(chunkSize)
	{
		mIndexes.resize(MAX_ENTITY_COUNT);
		for (int i = 0; i < capacity; ++i)
		{
			mIndexes[i] = -1;
		}
		mData = mCapacity > 0 ? operator new[](mCapacity * BLOCK_SIZE) : nullptr;
	}
	~GPool() 
	{
	}

	class GPoolIterator : public std::iterator<std::forward_iterator_tag, C*>
	{
	private:
		Block* mCurrentBlock;
	public:
		GPoolIterator(Block* block) : mCurrentBlock(block)
		{}
		GPoolIterator& operator=(const GPoolIterator& iter) 
		{
			mCurrentBlock = iter.mCurrentBlock;
			return *this;
		}
		C* operator*() const
		{
			return &mCurrentBlock->mComponent;
		}
		GPoolIterator& operator++()
		{
			++mCurrentBlock;
			return *this;
		}

		GPoolIterator operator++(int)
		{
			GPoolIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator==(const GPoolIterator& iter)
		{
			return mCurrentBlock == iter.mCurrentBlock;
		}
		bool operator!=(const GPoolIterator& iter)
		{
			return mCurrentBlock != iter.mCurrentBlock;
		}

	};

	class GPoolPairIterator : public std::iterator<std::forward_iterator_tag, std::pair<Entity, C*>>
	{
	private:
		Block* mCurrentBlock;
		typedef std::pair<Entity, C*> ValuePair;
		mutable ValuePair pair;

	public:
		GPoolPairIterator(Block* block) : mCurrentBlock(block)
		{}
		GPoolPairIterator& operator=(const GPoolIterator& iter)
		{
			mCurrentBlock = iter.mCurrentBlock;
			return *this;
		}
		ValuePair* operator*() const
		{
			pair.first = mCurrentBlock->mEntity;
			pair.second = &mCurrentBlock->mComponent;
			return &pair;
		}
		GPoolPairIterator& operator++()
		{
			++mCurrentBlock;
			return *this;
		}

		GPoolPairIterator operator++(int)
		{
			GPoolPairIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator==(const GPoolPairIterator& iter)
		{
			return mCurrentBlock == iter.mCurrentBlock;
		}
		bool operator!=(const GPoolPairIterator& iter)
		{
			return mCurrentBlock != iter.mCurrentBlock;
		}

	};

	GPoolIterator begin() 
	{ 
		Block* ptr = static_cast<Block*>(mData);
		return(GPoolIterator(mSize == 0 ? nullptr : ptr));
	}

	GPoolIterator end()
	{
		Block* ptr = static_cast<Block*>(mData);
		return(GPoolIterator(mSize == 0 ? nullptr : &ptr[mSize]));
	}

	GPoolPairIterator beginPair()
	{
		Block* ptr = static_cast<Block*>(mData);
		return(GPoolPairIterator(mSize == 0 ? nullptr : ptr));
	}

	GPoolPairIterator endPair()
	{
		Block* ptr = static_cast<Block*>(mData);
		return(GPoolPairIterator(mSize == 0 ? nullptr : &ptr[mSize]));
	}

	C* get(Entity entity)
	{
		if (entity >= mIndexes.size())
			return nullptr;
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
		if (entity >= mIndexes.size())
			return nullptr;

		if (mIndexes[entity] != -1)
			return nullptr;

		if (mSize == mCapacity)
		{
			if (!reserve(mChunkSize))
				return nullptr;
		}
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

	bool doesContainComponent(Entity entity)
	{
		return mIndexes[entity] != -1;
	}

	size_t Size() { return mSize; }
	size_t Capacity() { return mCapacity; }
private:
	bool reserve(size_t chunk)
	{
		void* newData = operator new[]((mCapacity + chunk) * BLOCK_SIZE);
		if (newData == nullptr)
			return false;

		if (mCapacity != 0)
		{
			memcpy(newData, mData, (mCapacity * BLOCK_SIZE));
			operator delete[] (mData);
		}
		mCapacity += chunk;
		mData = newData;
		return true;
	}
private :
	size_t mCapacity;
	size_t mSize;
	size_t mChunkSize;

	std::vector<int> mIndexes;
	void*               mData;
};
#endif

