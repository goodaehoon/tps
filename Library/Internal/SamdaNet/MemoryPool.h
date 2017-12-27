#pragma once

#include <deque>

template < typename T, size_t N >
class Pool
{
private:
	vector< T* > blocks;		// 할당된 메모리 전체 메모리 블럭을 가지고 있는 벡터
	deque< T* > unused;			// 그 중 사용하지 않은 메모리 블럭을 가지고 있는 벡터
	size_t allocCount;			// 할당할 개수 (처음에 정해주고, 다 사용하면 이 값만큼 계속하여 할당)
	unsigned int allocIndex;	// 지금까지 할당된 메모리 블럭 번호 (0번은 사용하지 않음)

public:
	Pool()
		: allocCount(N)
		, allocIndex(0)
	{}

	~Pool()
	{
		for (auto it = blocks.begin(); it != blocks.end(); ++it)
			delete[] (*it);

		blocks.clear();
		unused.clear();
	}

	T* Acquire(size_t size)
	{
		if (unused.empty())
		{
			T* block = new T[allocCount];
			blocks.push_back(block);

			for (int i = 0; i < allocCount; ++i)
			{
				// 0번은 사용하지 않기 위해 1번부터 할당한다.
				// push_front로 밀어넣어 unused는 [1], [2], [3], [4] ... 이런식으로 가지고 있게 한다
				block[i].index = ++allocIndex;
				unused.push_front(&block[i]);
			}
		}

		// 요청시에는 가장 앞번부터 돌려준다
		T* p = unused.front();
		unused.pop_front();

		return p;
	}

	void Release(void* p)
	{
		if (p == nullptr)
			return;

		T* cast = static_cast<T*>(p);

		// 풀에 돌려줄때 사용한 객체는 가장 뒷쪽에 넣어준다 
		// (재사용 시점을 늦추려고, 혹시나 세션같이 다른 스레드로 작업이 오가다 지연되는 동안, 다른 세션이 이 세션번호를 사용하는 경우가 생기는 걸 방지하고자)
		// 잘 검사하는 것이 맞겠지만 혹시나...
		unused.push_back(cast);
	}
};

// 메모리풀 객체로 만들고자 하는 객체에 상속하여 사용한다
// 상속을 받으면 T타입의 static Pool객체가 만들어지며
// new, delete 키워드를 재정의 했기 때문에, 해당 객체를 new, delete 할때마다 pool에서 Acquire, Release가 불려지게 된다
template < typename T, size_t N >
class MemoryPool
{
private:
	friend class Pool< T, N >;
	static Pool< T, N > pool;
	unsigned int index;

public:
	MemoryPool()
		: index(0)
	{};

	static void* operator new(size_t size)
	{
		return pool.Acquire(size);
	}

	static void operator delete(void* p)
	{
		return pool.Release(p);
	}

	unsigned int GetID()
	{
		return index;
	}
};

template <typename T, size_t N>
Pool<T, N> MemoryPool<T, N>::pool;