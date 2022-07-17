/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
					template <> class TBinaryHeap; 
					struct TMinHeap;
					struct TMaxHeap;
					struct TMinHeapOfPairs;
					struct TMaxHeapOfPairs;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __HEAP_H__
#define __HEAP_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "Allocator.h"
#include "CompareTools.h"

#include "ObjectPool.h"
#include "HeapBase.h"

#include "PairValues.h"

//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif 
//=============================================================================
#ifndef HEAP_REGISTER_INDICES
	#define HEAP_REGISTER_INDICES
#endif // HEAP_REGISTER_INDICES
#ifdef HEAP_REGISTER_INDICES
	//#define REGISTER_INDICES register
	#define REGISTER_INDICES 
#else
	#define REGISTER_INDICES 
#endif // HEAP_REGISTER_INDICES
//=============================================================================

namespace CELL { 

namespace BINARY_HEAP { 

//=============================================================================
//============================= TBinaryHeap ===================================
//=============================================================================
/*! @brief TBinaryHeap 
	The TBinaryHeap class implements a binary heap and is a specialization 
	of the heap base class THeapBase. A binary heap stores the value in form 
	of a binary tree. Where any parent has at most two children. The order 
	of the values is given by the heap order property. 
	This implemenation uses an implicit datastructure to store its values. 
	This implicit datastructure is given by the template parameter TPoolType.
	0. Heap order
	The heap order is given by the template paramenter TCompType. 
	The heap order for minimum heaps insists that any parent value is smaller 
	than its child values (parent < children).
	The heap order for maximum heaps insists that any parent value is greater 
	than its children (parent > children).
	The following 3 tasks are necessary to make sure that a heap will always 
	stay in the given heap order. 
	I. Assure Heap Up:
	There are 2 possible ways to assure that a new inserted element is following 
	the heap order if the element was inserted at the back of the heap (push).
	1. unoptimized version: works with non random access containers like list.
	Parent and children will swap their position.
	2. optimized version: works only with random access container like pool, vector.
	There is no swap of elements instead the parents will be replaced by 
	their children,
	II. Assure Heap Down:
	There are 2 possible ways to assure that the heap is following the heap 
	order if the element was deleted from the front of the heap (pop).
	unoptimized version: works with non random access containers 
	like list.
	1. Parent and children will swap their position.
	2.optimized version: works only with random access container 
	like pool, vector.
	There is no swap of elements instead the parents will be 
	replaced by their children,
	III. Sorting: 
	The following description of the sorting method is using a bottom up 
	heap sorting strategy.
	There are 2 ways to reverse the sorted order: 
	The second method is prefered because of the better complexity. 
	1. First way:
	a. Create a new heap with the reversed heap property. NlogN 
	b. Than sort the heap with the reversed heap property. NlogN 
	Complexity: NlogN + NlogN
	2. Second way:
	a. Sort the heap with the given heap property. NlogN 
	b. Than reverse the order of the elements. N / 2
	Complexity: NlogN + N / 2
	Advantage:
	This class can be seen as a stl priority_queue replacement. 
	In fact it is the way the stl priority_queue should be implemented.
*/
//=============================================================================
	template 
	<  
		typename TValueType, 
		typename TCompType = COMPTOOLS::TCompareLess< TValueType >
	>
	class TBinaryHeap : public HEAP_BASE::THeapBase< TValueType, TCompType >
	{
	public:
		using this_type = TBinaryHeap< TValueType, TCompType >;
		using base_type = HEAP_BASE::THeapBase< TValueType, TCompType >;

		typedef typename base_type::pool_type pool_type;

		typedef typename base_type::allocator_type allocator_type;	
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::value_type value_type;

		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;

		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reverse_iterator reverse_iterator;      
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;

		typedef typename base_type::comparator_type comparator_type;

	public:
		CONSTEXPR TBinaryHeap( const comparator_type & comp = comparator_type(),
			const allocator_type & alloc = allocator_type() ) : 
			base_type( comp, alloc )
		{
		}
		CONSTEXPR TBinaryHeap(const size_type _size,
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			base_type( _size, comp, alloc  )
		{
		}
		CONSTEXPR TBinaryHeap(const size_type _size,
			const_reference val,// = value_type(), 
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			base_type( _size, val, comp, alloc  )
		{
		}
		template <typename InputIterator>
		CONSTEXPR TBinaryHeap( InputIterator _First, InputIterator _Last,
			bool assureHeap = true,
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			base_type( _First, _Last, comp, alloc )
		{
			if ( assureHeap )
			{
				assure_heap( );
			}
		}
		CONSTEXPR TBinaryHeap( const this_type & heap ) :
			base_type( heap.get_pool(), heap.get_comparator() )
		{
		}
		CONSTEXPR TBinaryHeap( const pool_type & pool, bool assureHeap = true,
			const comparator_type & comp = comparator_type() ) : 
			base_type( pool, comp )
		{
			if ( assureHeap )
			{
				assure_heap( );
			}
		}
		CONSTEXPRDTOR_VIRTUAL virtual ~TBinaryHeap()
		{
		}

		CONSTEXPR this_type & operator = ( const this_type & rhs )
		{
			return assign( rhs );
		}
		CONSTEXPR this_type & assign( const this_type & rhs )
		{
			if ( this != (&rhs) ) 
			{
				this_type new_pool( rhs );
				swap( new_pool );
			}
			return (*this);
		}
		template <typename InputIterator>
		CONSTEXPR void insert( iterator pos, InputIterator _First, InputIterator _Last, bool assureHeap = true )
		{
			this->mElements.insert( pos, _First, _Last );
			if ( assureHeap )
			{
				assure_heap();
			}
		}
		CONSTEXPR iterator insert( iterator iter, const_reference value, bool assureHeap = true )
		{
			iterator iter_res = this->mElements.insert( iter, value );
			if ( assureHeap )
			{
				assure_heap();
			}
			return iter_res;
		}
		CONSTEXPR void insert( iterator iter, size_type n, const_reference value, bool assureHeap = true )
		{
			this->mElements.insert( iter, n, value );
			if ( assureHeap )
			{
				assure_heap();
			}
		}
		template <class InputIterator>
		CONSTEXPR void assign( InputIterator _First, InputIterator _Last, bool assureHeap = true )
		{
			this->mElements.assign( _First, _Last );
			if ( assureHeap )
			{
				assure_heap();
			}
		}
		CONSTEXPR void assign( const_reference value, bool assureHeap = true )
		{
			this->mElements.assign( base_type::base_type::size(), value );
			if ( assureHeap )
			{
				assure_heap();
			}
		}
		CONSTEXPR void assign( size_type n, const_reference value, bool assureHeap = true )
		{
			this->mElements.assign( n, value );
			if ( assureHeap )
			{
				assure_heap();
			}
		}
		CONSTEXPR void push_back(const_reference element, bool assureHeap = true)
		{
			this->mElements.push_back(element);
			if (assureHeap)
			{
				assure_heap_up(base_type::LAST_ELEMENT_INDEX());
			}
		}
		CONSTEXPR void push_back(value_type&& val, bool assureHeap = true)
		{
			this->mElements.push_back(CELL::TOOLS::MEMORY::TForward<value_type>(val));
			if (assureHeap)
			{
				assure_heap_up(base_type::LAST_ELEMENT_INDEX());
			}
		}
		template <typename... TArgumentTypes>
		CONSTEXPR void emplace_back(TArgumentTypes &&... args, bool assureHeap = true)
		{
			this->mElements.emplace_back(CELL::TOOLS::MEMORY::TForward<TArgumentTypes>(args)...);
			if (assureHeap)
			{
				assure_heap_up(base_type::LAST_ELEMENT_INDEX());
			}
		}
//============================= HEAP BASE INTERFACE ===========================
		CONSTEXPR void push( const_reference element)
		{ 
			this->mElements.push_back( element );
			assure_heap_up(base_type::LAST_ELEMENT_INDEX() );
		}
		CONSTEXPR void push(value_type&& val)
		{
			this->mElements.push_back(CELL::TOOLS::MEMORY::TForward<value_type>(val));
			assure_heap_up(base_type::LAST_ELEMENT_INDEX());
		}
		template <typename... TArgumentTypes>
		CONSTEXPR void push_emplace(TArgumentTypes &&... args)
		{
			this->mElements.emplace_back(CELL::TOOLS::MEMORY::TForward<TArgumentTypes>(args)...);
			assure_heap_up(base_type::LAST_ELEMENT_INDEX());
		}
		CONSTEXPR void pop()
		{ 
			if ( !base_type::empty() )
			{
				const size_type last_element_index = base_type::LAST_ELEMENT_INDEX();
				if ( last_element_index != base_type::ROOT_INDEX )
				{
					// unoptimized version: general
					//base_type::swap_elements( ROOT_INDEX, last_element_index );
					// optimized version: requires random access
					this->mElements[base_type::ROOT_INDEX ] = this->mElements[ last_element_index ];
					this->mElements.pop_back();
					assure_heap_down(base_type::ROOT_INDEX, base_type::LEAFS_END_INDEX() );
				}
				else
				{
					this->mElements.pop_back();
				}
				//this->mElements.pop_back();
			}
		} 
		CONSTEXPR void sort( bool reverseOrder = false )
		{
			//1. First way:
			//if ( reverseOrder )
			//{
			//	const size_type last_element_index = LEAFS_END_INDEX();
			//	size_type element_index = LEAFS_START_INDEX(); 
			//	while ( --element_index )
			//	{
			//		sort_heap_down( element_index, last_element_index );
			//	}
			//	sort_heap_down( element_index, last_element_index );
			//	size_type index = mSize;
			//	while ( --index )
			//	{
			//		std::swap( this->mElements[ ROOT_INDEX ], this->mElements[ index ] );
			//		sort_heap_down( ROOT_INDEX, index - 1 );
			//	}
			//}
			//else
			//{
			//	size_type index = mSize;
			//	while ( --index )
			//	{
			//		std::swap( this->mElements[ ROOT_INDEX ], this->mElements[ index ] );
			//		assure_heap_down( ROOT_INDEX, index - 1 );
			//	}
			//}
			//II.2. Second way:
			size_type element_index = base_type::base_type::size();
			size_type last_element_index = base_type::LAST_ELEMENT_INDEX();
			while ( --element_index  )
			{
				base_type::swap_elements(base_type::ROOT_INDEX, element_index );
				assure_heap_down(base_type::ROOT_INDEX, element_index - 1 );
			}
			if ( reverseOrder )
			{
				base_type::swap_elements( element_index, last_element_index );
				while ( ++element_index < --last_element_index )
				{
					base_type::swap_elements( element_index, last_element_index );
				}
			}
		}
		CONSTEXPR void assure_heap()
		{
			assure_heap(base_type::LEAFS_START_INDEX() );
		}
		CONSTEXPR bool is_heap() const
		{
			const size_type element_size = base_type::size();
			for (size_type i = 1; i < element_size; ++i)
			{
				if ( compare_elements( i, base_type::PARENT_INDEX( i ) ) )
				{
					return false;
				}
			}
			return true;
		}
//=============================================================================
	protected:
		CONSTEXPR bool compare_elements( const size_type & lhs, const size_type & rhs ) const
		{
			// unoptimized version: general requires virtual and overloaded at().
			//return mCompare( at( lhs ), at( rhs ) );
			// optimized version: requires random access
			return base_type::mCompare( this->mElements[ lhs ], this->mElements[ rhs ] );
		}
		CONSTEXPR void assure_heap( size_type index )
		{
			if ( index )
			{
				const size_type last_element_index = base_type::LEAFS_END_INDEX();
				while ( --index )
				{
					assure_heap_down( index, last_element_index );
				}
				assure_heap_down( index, last_element_index );
			}
		}
		CONSTEXPR void assure_heap_up( size_type index )
		{
			//I. 1. unoptimized version:
			REGISTER_INDICES size_type parent_index = base_type::PARENT_INDEX( index );
			while ( index )
			{
				if ( compare_elements( index, parent_index ) )
				{
					base_type::swap_elements( parent_index, index );
				}
				else
				{
					return;
				}
				index = parent_index;
				parent_index = base_type::PARENT_INDEX( index );
			}
			//I. 2. optimized version:
			//const value_type element = this->mElements[ position_index ];
			//size_type child_index = position_index;
			//size_type parent_index = PARENT_INDEX( child_index );
			//while ( child_index )
			//{
			//	if ( compare_elements( child_index, parent_index ) )
			//	{
			//		//base_type::swap_elements( parent_index, child_index );
			//		this->mElements[ child_index ] = this->mElements[ parent_index ];
			//		child_index = parent_index;
			//		parent_index = PARENT_INDEX( child_index ); 
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
			//if (position_index != child_index)
			//{
			//	this->mElements[ child_index ] = element;
			//}
		}
		CONSTEXPR void assure_heap_down( size_type index, const size_type & end )
		{
			//II. 1. unoptimized version:
			REGISTER_INDICES size_type child_index = base_type::LEFT_CHILD_INDEX( index );
			while ( child_index < end ) 
			{
				if ( compare_elements ( child_index + 1, child_index ) )
				{
					++child_index;
				}
				if ( compare_elements( child_index, index ) )
				{
					base_type::swap_elements( index, child_index );
				}
				else
				{
					return;
				}
				index = child_index;
				child_index = base_type::LEFT_CHILD_INDEX( index );
			}
			if ( child_index == end ) 
			{
				if ( compare_elements( child_index, index ) )
				{
					base_type::swap_elements( index, child_index );
				}
			}
			//II. 2. optimized version:
			//const value_type element = this->mElements[ position_index ];
			//size_type parent_index = position_index;
			//size_type child_index = LEFT_CHILD_INDEX( parent_index );
			//while ( child_index < end_index ) 
			//{
			//	if ( compare_elements( child_index + 1, child_index ) )
			//	{
			//		++child_index;
			//	}
			//	if ( compare_elements( child_index, parent_index ) )
			//	{
			//		//base_type::swap_elements( parent_index, child_index );
			//		this->mElements[ parent_index ] = this->mElements[ child_index ];
			//		parent_index = child_index;
			//		child_index = LEFT_CHILD_INDEX( parent_index );
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
			//if ( child_index == end_index ) 
			//{
			//	if ( compare_elements( child_index, parent_index ) )
			//	{
			//		this->mElements[ parent_index ] = this->mElements[ child_index ];
			//		//base_type::swap_elements( parent_index, child_index );
			//	}
			//}
			//if (position_index != parent_index)
			//{
			//	this->mElements[ parent_index ] = element;
			//}
		}
//=============================================================================
	}; // TBinaryHeap
//=============================================================================
//====================== MIN / MAX HEAP ( PRIORITY QUEUE ) ====================
//=============================================================================
// order relation for min heap is parent_nodes <= child_nodes
// Which means all child nodes are greater than the parent nodes.
	template < typename TValueType >
	struct TMinHeap
	{ 
		using Type = TBinaryHeap< TValueType, 
			COMPTOOLS::TCompareLess< TValueType > >; 
	};
//=============================================================================
// order relation for max heap is parent_nodes >= child_nodes
// Which means all child nodes are less than the parent nodes.
	template < typename TValueType >
	struct TMaxHeap
	{ 
		using Type = TBinaryHeap< TValueType,
			COMPTOOLS::TCompareGreater< TValueType > >;
	};
//=============================================================================
	template 
	< 
		typename TFirstValueType,  
		typename TSecondValueType
	>
	struct TMinHeapOfPairs
	{ 
		using value_pair = PAIR::TPair< TFirstValueType, TSecondValueType >;
		using Type = TBinaryHeap< PAIR::TPair< TFirstValueType, TSecondValueType >,
			COMPTOOLS::TCompareLess< value_pair > >; 
	};
//=============================================================================
	template 
	< 
		typename TFirstValueType,  
		typename TSecondValueType
	>
	struct TMaxHeapOfPairs
	{ 
		using value_pair = PAIR::TPair< TFirstValueType, TSecondValueType >;
		using Type = TBinaryHeap< PAIR::TPair< TFirstValueType, TSecondValueType >,
			COMPTOOLS::TCompareGreater< value_pair > >;
	};
//=============================================================================
} // namespace BINARY_HEAP
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_POP
#endif 
//=============================================================================
#ifdef REGISTER_INDICES
	#undef REGISTER_INDICES 
#endif // REGISTER_INDICES
//=============================================================================
#ifdef HEAP_REGISTER_INDICES
	#undef HEAP_REGISTER_INDICES
#endif // HEAP_REGISTER_INDICES
//=============================================================================

#endif //__HEAP_H__
