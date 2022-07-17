/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __HASH_POOL_H__
#define __HASH_POOL_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "Allocator.h"
#include "CompareTools.h"
#include "HashTools.h"

#include "ObjectPool.h"
#include "HashPoolNode.h"

#include <cstddef> // size_t

//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif  
//=============================================================================

//=============================================================================
//#if !defined( DEBUG_HASH_POOL ) && defined( _DEBUG )
//#define DEBUG_HASH_POOL
//#endif // DEBUG_HASH_POOL
//=============================================================================
//#define DEBUG_HASH_POOL

namespace CELL {

namespace HPOOL {

	//=============================================================================
	//================================== THashPool ================================
	//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	class THashPool
	{
	public:
		using this_type = THashPool
		<
			TNodeType,
			TAllocType,
			TThresholdType,
			THashFuncType,
			TGrowingType
		>;

		typedef TNodeType node_type;
		typedef typename node_type::value_type value_type;
		typedef typename node_type::key_type key_type;
		typedef typename node_type::hash_type hash_type;

		typedef node_type * pointer;
		typedef node_type const * const_pointer;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t index_type;

		typedef hash_type & hash_reference;
		typedef hash_type const & const_hash_reference;

		typedef key_type & key_reference;
		typedef key_type const & const_key_reference;

		typedef value_type & reference;
		typedef value_type const & const_reference;

		typedef THashFuncType hash_func_type;
		typedef TAllocType allocator_type;
		typedef TGrowingType hash_growing_type;
		typedef TThresholdType threshold_type;

		typedef typename OPOOL::TSPool<pointer, DEFAULT_OBJECT_ALLOCATOR<pointer>>::Type pool_type;

#ifdef DEBUG_HASH_POOL
		static size_type NODE_DEALLOCATIONS_COUNTER;
		static size_type NODE_ALLOCATIONS_COUNTER;
#endif // DEBUG_HASH_POOL

		CONSTEXPR pointer NewNode(const key_type & key, const value_type & value)
		{
			pointer node = mAllocator.allocate(1);
			if (nullptr != node)
			{
				//mAllocator.construct(node, key, value);
				std::allocator_traits<allocator_type>::construct(mAllocator, node, key, value);

#ifdef DEBUG_HASH_POOL
				++NODE_ALLOCATIONS_COUNTER;
#endif // DEBUG_HASH_POOL
				return node;
			}
			else
			{
				GLOBAL_ASSERT(false && "ERROR NODE IS NOT ALLOCATABLE!");
				return node_type::NIL_NODE;
			}
		}
		CONSTEXPR pointer NewNode(const key_type & key)
		{
			pointer node = mAllocator.allocate(1);
			if (nullptr != node)
			{
				//mAllocator.construct(node, key);
				std::allocator_traits<allocator_type>::construct(mAllocator, node, key);

#ifdef DEBUG_HASH_POOL
				++NODE_ALLOCATIONS_COUNTER;
#endif // DEBUG_HASH_POOL
				return node;
			}
			else
			{
				GLOBAL_ASSERT(false && "ERROR NODE IS NOT ALLOCATABLE!");
				return node_type::NIL_NODE;
			}
		}
		CONSTEXPR void DeleteNode(pointer & node, bool attached)
		{
			if ((node_type::NIL_NODE != node) && (nullptr != node))
			{
				if (false == attached)
				{
					//mAllocator.destroy(node);
					std::allocator_traits<allocator_type>::destroy(mAllocator, node);
					mAllocator.deallocate(node, 1);
					node = node_type::NIL_NODE;

#ifdef DEBUG_HASH_POOL
					++NODE_DEALLOCATIONS_COUNTER;
#endif // DEBUG_HASH_POOL
				}
				else
				{
					node->setNextHPN(node_type::NIL_NODE);
				}
			}
			else
			{
				GLOBAL_ASSERT(false && "ERROR NODE IS NOT DEALLOCATABLE!");
			}
		}

	protected:
		typedef typename pool_type::iterator piterator;
		typedef typename pool_type::const_iterator const_piterator;
		typedef typename pool_type::reverse_iterator reverse_piterator;
		typedef typename pool_type::const_reverse_iterator const_reverse_piterator;
																						 
		static const size_type INIT_SIZE = 4;

		CONSTEXPR const_piterator elements_begin() const
		{
			return mElements.begin();
		}
		CONSTEXPR piterator elements_begin()
		{
			return mElements.begin();
		}
		CONSTEXPR const_piterator elements_end() const
		{
			return mElements.end();
		}
		CONSTEXPR piterator elements_end()
		{
			return mElements.end();
		}

	private:
		pool_type mElements;
		size_type mLoad;
		size_type mMaxLoad;
		bool mAttached;
		allocator_type mAllocator;
		hash_func_type mFunc;
		hash_growing_type mGrow;
		threshold_type mThreshold;

	private:
		//=============================================================================
		//============================ THashPoolIterator ==============================
		//=============================================================================
		class THashPoolIterator
		{
		private:
			piterator mpElementsIterEnd;
			piterator mpElementsIter;
			pointer mpNodesIter;

		public:
			CONSTEXPR THashPoolIterator() :
				mpElementsIterEnd(nullptr),
				mpElementsIter(nullptr),
				mpNodesIter(node_type::NIL_NODE)
			{
			}
			CONSTEXPR THashPoolIterator(const piterator & elementsIterEnd, const piterator & elementsIter) :
				mpElementsIterEnd(elementsIterEnd),
				mpElementsIter(elementsIter),
				mpNodesIter((*mpElementsIter))
			{
				init();
			}
			CONSTEXPR THashPoolIterator(const THashPoolIterator & iter) :
				mpElementsIterEnd(iter.mpElementsIterEnd),
				mpElementsIter(iter.mpElementsIter),
				mpNodesIter(iter.mpNodesIter)
			{
			}
			CONSTEXPRDTOR ~THashPoolIterator()
			{
			}
			CONSTEXPR THashPoolIterator & operator = (const THashPoolIterator & iter)
			{
				if (this != &iter)
				{
					mpElementsIterEnd = iter.mpElementsIterEnd;
					mpElementsIter = iter.mpElementsIter;
					mpNodesIter = iter.mpNodesIter;
				}
				return (*this);
			}
			CONSTEXPR const pointer & operator * () const
			{
				return (mpNodesIter);
			}
			CONSTEXPR pointer & operator * ()
			{
				return (mpNodesIter);
			}
			CONSTEXPR void next()
			{
				if (mpElementsIter != mpElementsIterEnd)
				{
					if (mpNodesIter != node_type::NIL_NODE)
					{
						mpNodesIter = mpNodesIter->getNextHPN();
					}
					while (mpNodesIter == node_type::NIL_NODE)
					{
						++mpElementsIter;
						if (mpElementsIter != mpElementsIterEnd)
						{
							mpNodesIter = (*mpElementsIter);
						}
						else
						{
							break;
						}
					}
				}
			}
			CONSTEXPR THashPoolIterator & operator ++ ()
			{
				next();
				return (*this);
			}
			CONSTEXPR const THashPoolIterator & operator ++ () const
			{
				next();
				return (*this);
			}
			CONSTEXPR THashPoolIterator operator ++ (int)
			{
				THashPoolIterator clone((*this));
				next();
				return clone;
			}
			CONSTEXPR THashPoolIterator operator ++ (int) const
			{
				THashPoolIterator clone((*this));
				next();
				return clone;
			}
			CONSTEXPR bool operator == (const THashPoolIterator & iter) const
			{
				return (mpElementsIter == iter.mpElementsIter);
			}
			CONSTEXPR bool operator != (const THashPoolIterator & iter) const
			{
				return (mpElementsIter != iter.mpElementsIter);
			}
		private:
			CONSTEXPR void init()
			{
				if (mpElementsIter != mpElementsIterEnd)
				{
					while (mpNodesIter == node_type::NIL_NODE)
					{
						++mpElementsIter;
						if (mpElementsIter != mpElementsIterEnd)
						{
							mpNodesIter = (*mpElementsIter);
						}
						else
						{
							break;
						}
					}
				}
			}
		};

	public:
		typedef THashPoolIterator iterator;
		typedef const THashPoolIterator const_iterator;

		CONSTEXPR const_iterator begin() const
		{
			return const_iterator(mElements.end(), mElements.begin());
		}
		CONSTEXPR iterator begin()
		{
			return iterator(mElements.end(), mElements.begin());
		}
		CONSTEXPR const_iterator end() const
		{
			return const_iterator(mElements.end(), mElements.end());
		}
		CONSTEXPR iterator end()
		{
			return iterator(mElements.end(), mElements.end());
		}

	public:
		CONSTEXPR THashPool(const this_type & hashTable) :
			mAttached(hashTable.attached()),
			mElements(hashTable.get_pool()),
			mLoad(hashTable.load()),
			mMaxLoad(hashTable.max_load()),
			mFunc(hashTable.get_hash_func()),
			mGrow(hashTable.get_hash_growing()),
			mAllocator(hashTable.get_allocator()),
			mThreshold(hashTable.get_threshold())
		{
		}
		CONSTEXPR THashPool(bool attached = false,
			const hash_func_type & func = hash_func_type(),
			const hash_growing_type & grow = hash_growing_type(),
			const threshold_type & threshold = threshold_type(),
			const allocator_type & alloc = allocator_type()) :
			mAttached(attached),
			mElements(grow.initialize(INIT_SIZE), node_type::NIL_NODE),
			mLoad(0),
			mMaxLoad(threshold(grow.initialize(INIT_SIZE))),
			mFunc(func),
			mGrow(grow),
			mAllocator(alloc),
			mThreshold(threshold)
		{
		}
		CONSTEXPR THashPool(const size_type & _size,
			bool attached = false,
			const hash_func_type & func = hash_func_type(),
			const hash_growing_type & grow = hash_growing_type(),
			const threshold_type & threshold = threshold_type(),
			const allocator_type & alloc = allocator_type()) :
			mAttached(attached),
			mElements(grow.next(_size), node_type::NIL_NODE),
			mLoad(0),
			mMaxLoad(threshold(grow.next(_size))),
			mFunc(func),
			mGrow(grow),
			mAllocator(alloc),
			mThreshold(threshold)
		{
		}
		CONSTEXPRDTOR ~THashPool()
		{
			destroy();		   
		}

		CONSTEXPR this_type & operator = (const this_type & rhs)
		{
			return assign(rhs);
		}
		CONSTEXPR this_type & assign(const this_type & rhs)
		{
			if (this != (&rhs))
			{
				const_piterator iter = rhs.elements_begin();
				const_piterator iter_end = rhs.elements_end();
				while (iter != iter_end)
				{
					pointer ptr = (*iter);
					while (ptr != nullptr)
					{
						insert(ptr->getKey(), ptr->getValue());
						ptr = ptr->getNextHPN();
					}
					++iter;
				}
			}
			return (*this);
		}
		CONSTEXPR const allocator_type & get_allocator() const
		{
			return mAllocator;
		}
		CONSTEXPR const hash_func_type & get_hash_func() const
		{
			return mFunc;
		}
		CONSTEXPR const hash_growing_type & get_hash_growing() const
		{
			return mGrow;
		}
		CONSTEXPR const threshold_type & get_threshold() const
		{
			return mThreshold;
		}
		CONSTEXPR const pool_type & get_pool() const
		{
			return mElements;
		}
		//=============================================================================
		CONSTEXPR size_type load() const
		{
			return mLoad;
		}
		CONSTEXPR size_type max_load() const
		{
			return mMaxLoad;
		}
		CONSTEXPR float load_factor() const
		{
			const float lfactor = static_cast<float>(load()) / static_cast<float>(size());
			return lfactor;
		}
		CONSTEXPR size_type max_size() const
		{
			return mElements.max_size();
		}
		CONSTEXPR size_type capacity() const
		{
			return mElements.capacity();
		}
		CONSTEXPR size_type size() const
		{
			return mElements.size();
		}
		CONSTEXPR bool empty() const
		{
			return mElements.empty();
		}
		CONSTEXPR bool attached() const
		{
			return mAttached;
		}
		CONSTEXPR void setAttached(bool val)
		{
			mAttached = val;
		}
		//=============================================================================
		CONSTEXPR void resize(const size_type size)
		{
			rehash2next(size);
		}
		CONSTEXPR bool contains(const key_type & key) const
		{
			pointer node = findHashNode(key);
			if (node != node_type::NIL_NODE)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		CONSTEXPR bool find(const key_type & key, reference item) const
		{
			pointer node = findHashNode(key);
			if (node != node_type::NIL_NODE)
			{
				item = node->getValue();
				return true;
			}
			else
			{
				return false;
			}
		}
		CONSTEXPR reference at(const key_type & key)
		{
			pointer node = findHashNode(key);
			if (node != node_type::NIL_NODE)
			{
				return node->getValue();
			}
			else
			{
				pointer new_node = insert(key);
				return new_node->getValue();
			}
		}
		CONSTEXPR const_reference at(const key_type & key) const
		{
			pointer node = findHashNode(key);
			return node->getValue(); 
		}
		CONSTEXPR reference operator [] (const key_type & key)
		{
			return at(key);
		}
		CONSTEXPR const_reference operator [] (const key_type & key) const
		{
			return at(key);
		}
		//=============================================================================
		CONSTEXPR pointer insert(const key_type & key, pointer node)
		{
			const size_type key_index = insertHash(node, key);
			insertNode(node, key_index);
			return node;
		}
		CONSTEXPR pointer insert(const key_type & key, const value_type & value)
		{
			pointer node = NewNode(key, value);
			const size_type key_index = insertHash(node, key);
			insertNode(node, key_index);
			return node;
		}
		CONSTEXPR pointer insert(const key_type & key)
		{
			pointer node = NewNode(key);
			const size_type key_index = insertHash(node, key);
			insertNode(node, key_index);
			return node;
		}
		CONSTEXPR bool remove(const key_type & key)
		{
			pointer prevNode = node_type::NIL_NODE;
			const hash_type hash = mFunc(key);
			const size_type size = mElements.size();
			const size_type key_index = getElementIndex(hash, size);
			pointer prev_node = node_type::NIL_NODE;
			pointer foundNode = findRemoveHashNode(key, hash, key_index, prev_node);
			if (foundNode != node_type::NIL_NODE)
			{
				removeNode(foundNode, prev_node, key_index);
				return true;
			}
			else
			{
				return false;
			}
		}
		CONSTEXPR void shrink_to_fit()
		{
			rehash2next(mLoad);
		}				  
		CONSTEXPR void swap(this_type & rhs)
		{
			mElements.swap(rhs.mElements);

			const size_type temp = mLoad;
			mLoad = rhs.mLoad;
			rhs.mLoad = temp;

			const size_type tempm = mMaxLoad;
			mMaxLoad = rhs.mMaxLoad;
			rhs.mMaxLoad = tempm;
									
			const bool tmp_attached = mAttached;
			mAttached = rhs.mAttached;
			rhs.mAttached = tmp_attached;
		}
		CONSTEXPR void clear()
		{
			removeAll();

			pool_type elements(mGrow.initialize(INIT_SIZE), node_type::NIL_NODE);
			mElements.swap(elements);

			mMaxLoad = mThreshold(mElements.size());
			mLoad = 0;
		}
		CONSTEXPR void destroy()
		{
			removeAll();

			mElements.resize(0);

			mLoad = 0;
			mMaxLoad = 0;
		}
		CONSTEXPR size_type activeSlots()
		{
			size_type counter = 0;
			iterator iter = begin();
			iterator iter_end = end();
			pointer node = node_type::NIL_NODE;
			while (iter != iter_end)
			{
				node = (*iter);
				if (node != node_type::NIL_NODE)
				{
					++counter;
				}
				++iter;
			}
			return counter;
		}
		template < typename TObjectPool >
		CONSTEXPR void activeElements(TObjectPool & elements)
		{
			iterator iter = begin();
			iterator iter_end = end();
			pointer node = node_type::NIL_NODE;
			while (iter != iter_end)
			{
				node = (*iter);
				if (node != node_type::NIL_NODE)
				{
					elements.push_back(node);
				}
				++iter;
			}
		}

	protected:
		CONSTEXPR void removeAll()
		{
			piterator iter = mElements.begin();
			piterator iter_end = mElements.end();
			pointer node = node_type::NIL_NODE;
			while (iter != iter_end)
			{
				node = (*iter);
				if (node != node_type::NIL_NODE)
				{
					removeElementNode(node);
				}
				++iter;
			}
		}
		CONSTEXPR size_type insertHash(pointer node, const key_type & key)
		{
			const hash_type hash = mFunc(key);
			node->setHash(hash);
			const size_type size = mElements.size();
			const size_type key_index = getElementIndex(hash, size);
			return key_index;
		}
		CONSTEXPR void insertNode(pointer node, const size_type & key_index)
		{
			pointer node_first = mElements[key_index];
			node->setNextHPN(node_first);
			node_first = node;
			mElements[key_index] = node_first;
			++mLoad;
			if (mLoad > mMaxLoad)
			{
				rehash2next();
			}
		}
		CONSTEXPR void removeElementNode(pointer node)
		{
			pointer node_iter = node;
			pointer node_iter_next = node_type::NIL_NODE;
			while (node_iter != node_type::NIL_NODE)
			{
				node_iter_next = node_iter->getNextHPN();
				DeleteNode(node_iter, mAttached);
				node_iter = node_iter_next;
			}
		}
		CONSTEXPR void removeNode(pointer node, pointer prev_node, const size_type & key_index)
		{
			if (prev_node != node_type::NIL_NODE)
			{
				prev_node->setNextHPN(node->getNextHPN());
			}
			else
			{
				mElements[key_index] = node->getNextHPN();
			}
			DeleteNode(node, mAttached);
			--mLoad;
		}
		CONSTEXPR size_type getElementIndex(const key_type & key) const
		{
			const hash_type hash = mFunc(key);
			const size_type size = mElements.size();
			const size_type key_index = getElementIndex(hash, size);
			return key_index;
		}
		CONSTEXPR size_type getElementIndex(const size_type hash_key, const size_type size) const
		{
			return mFunc(hash_key, size);
		}
		CONSTEXPR bool compare(pointer & obj, const key_type & key, const hash_type & hash) const
		{
			return (obj->compare(key, hash));
			//return mCompare(obj, key, hash);
		}
		CONSTEXPR pointer findHashNode(const key_type & key) const
		{
			const hash_type hash = mFunc(key);
			const size_type size = mElements.size();
			const size_type key_index = getElementIndex(hash, size);
			return findHashNode(key, hash, key_index);
		}
		CONSTEXPR pointer findHashNode(const key_type & key,
			const hash_type & hash,
			const size_type & key_index) const
		{
			pointer node = mElements[key_index];
			while (node != node_type::NIL_NODE)
			{
				if (compare(node, key, hash))
				{
					return (node);
				}
				node = node->getNextHPN();
			}
			return (node_type::NIL_NODE);
		}
		CONSTEXPR pointer findRemoveHashNode(const key_type & key,
			const hash_type & hash,
			const size_type & key_index, 
			pointer & prev_node) const
		{
			prev_node = node_type::NIL_NODE;
			pointer node = mElements[key_index];
			while (node != node_type::NIL_NODE)
			{
				if (compare(node, key, hash))
				{
					return (node);
				}
				prev_node = node;
				node = node->getNextHPN();
			}
			return (node_type::NIL_NODE);
		}
		CONSTEXPR bool rehash2next()
		{
			const size_type size = mElements.size();
			return rehash2next(size);
		}
		CONSTEXPR bool rehash2next(const size_type size)
		{
			const size_type grow_size = mGrow.next(size);
			pool_type elements(grow_size, node_type::NIL_NODE);

			size_type load = 0;
			index_type index = 0;
			hash_type hash_key = 0;
			size_type key_index = 0;
			pointer node_insert = node_type::NIL_NODE;
			pointer node_first = node_type::NIL_NODE;
			pointer node = node_type::NIL_NODE;
			piterator iter = mElements.begin();
			piterator iter_end = mElements.end();
			while (iter != iter_end)
			{
				node = (*iter);
				if (node != node_type::NIL_NODE)
				{
					while (node != node_type::NIL_NODE)
					{
						node_insert = node;
						node = node->getNextHPN();

						hash_key = node_insert->getHash();
						key_index = getElementIndex(hash_key, grow_size);
						
						node_first = elements[key_index];
						node_insert->setNextHPN(node_first);
						node_first = node_insert;
						elements[key_index] = node_first;

						++load;
					}
				}
				++iter;
			}
			if (load == mLoad)
			{
				mElements.swap(elements);
				mMaxLoad = mThreshold(mElements.size());
				mLoad = load;
				return true;
			}
			else
			{
				return false;
			}
		}
//=============================================================================
	}; // THashPool
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR void swap(THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		lhs.swap(rhs);
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator == (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() == rhs.begin());
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator != (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() != rhs.begin());
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator < (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() < rhs.begin());
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator > (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() > rhs.begin());
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator <= (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() <= rhs.begin());
	}
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	CONSTEXPR bool operator >= (
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & lhs,
		const THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType> & rhs)
	{
		return (lhs.begin() >= rhs.begin());
	}
//=============================================================================
	template
	<
		typename TKeyType,
		typename TValueType
	>
	struct THPool
	{
		using value_type = TValueType;
		using key_type = TKeyType;
		using hash_type = size_t;
		using size_type = size_t;
		using node_type = HPOOLNODE::THashPoolNode< TKeyType, TValueType >;

		using alloc_type = DEFAULT_OBJECT_ALLOCATOR< HPOOLNODE::THashPoolNode< TKeyType, TValueType >  >;
		using thresh_ft = TOOLS::HASH::THashPoolThreshFunc;
		using hash_ft = TOOLS::HASH::THashFunc< TKeyType >;
		using grow_ft = OPOOLGROW::TPO2GStrat< size_t >;

		using Type = THashPool< node_type, alloc_type, thresh_ft, hash_ft, grow_ft >;
	};
//=============================================================================
	template
	<
		typename TKeyType,
		typename TValueType
	>
	struct THPoolGrid2D
	{
		using value_type = TValueType;
		using key_type = TKeyType;
		using size_type = size_t;
		using node_type = HPOOLNODE::THashPoolNode< TKeyType, TValueType >;

		using alloc_type = DEFAULT_OBJECT_ALLOCATOR< HPOOLNODE::THashPoolNode< TKeyType, TValueType >  >;
		using thresh_ft = TOOLS::HASH::THashPoolThreshFunc;
		using hash_ft = TOOLS::HASH::THashGrid2DFunc< TKeyType >;
		using grow_ft = OPOOLGROW::TPO2GStrat< size_t >;

		using Type = THashPool< node_type, alloc_type, thresh_ft, hash_ft, grow_ft >;
	};
	//=============================================================================
	template
	<
		typename TKeyType,
		typename TValueType
	>
	struct THPoolGrid3D
	{
		using value_type = TValueType;
		using key_type = TKeyType;
		using size_type = size_t;
		using node_type = HPOOLNODE::THashPoolNode< TKeyType, TValueType >;

		using alloc_type = DEFAULT_OBJECT_ALLOCATOR< HPOOLNODE::THashPoolNode< TKeyType, TValueType >  >;
		using thresh_ft = TOOLS::HASH::THashPoolThreshFunc;
		using hash_ft = TOOLS::HASH::THashGrid3DFunc< TKeyType >;
		using grow_ft = OPOOLGROW::TPO2GStrat< size_t >;

		using Type = THashPool< node_type, alloc_type, thresh_ft, hash_ft, grow_ft >;
	};
	//=============================================================================
#ifdef DEBUG_HASH_POOL
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	size_t THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType>::NODE_DEALLOCATIONS_COUNTER = 0;
//=============================================================================
	template
	<
		typename TNodeType,
		typename TAllocType,
		typename TThresholdType,
		typename THashFuncType,
		typename TGrowingType
	>
	size_t THashPool<TNodeType, TAllocType, TThresholdType, THashFuncType, TGrowingType>::NODE_ALLOCATIONS_COUNTER = 0;
#endif // DEBUG_HASH_POOL
//=============================================================================
} // namespace HPOOL
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef DEBUG_HASH_POOL
#undef DEBUG_HASH_POOL
#endif // DEBUG_HASH_POOL
//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_POP
#endif  
//=============================================================================

#endif // __HASH_POOL_H__
