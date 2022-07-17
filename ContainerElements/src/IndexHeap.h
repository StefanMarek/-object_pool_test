/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
					template <> class TBinaryIndexHeap;
					struct TMinBinaryIndexHeap;
					struct TMaxBinaryIndexHeap;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __INDEX_HEAP_H__
#define __INDEX_HEAP_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "Allocator.h"
#include "ObjectPool.h"

#include "CompareTools.h"
#include "HeapBase.h"

//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif 
//=============================================================================

namespace CELL { 

namespace BINARY_INDEX_HEAP { 

//=============================================================================
//============================= TBinaryIndexHeap ==============================
//=============================================================================
// The index heap is a binary heap, that works with indices rather than values. 
//=============================================================================
	template 
	<  
		typename TValueType, 
		typename TIndexType, 
		typename TCompType = COMPTOOLS::TCompareLess< TValueType >
	>
	class TBinaryIndexHeap : public HEAP_BASE::THeapBase< TIndexType, TCompType >
	{
	public:
		typedef TBinaryIndexHeap< TValueType, TIndexType, TCompType > this_type;
		typedef HEAP_BASE::THeapBase< TIndexType, TCompType > base_type;

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

		typedef TValueType indexed_values_type;

	private:
		// a pointer to array values of the template type TValueType
		indexed_values_type * mpIndexedValues;

	public:
		CONSTEXPR TBinaryIndexHeap( indexed_values_type * pValues = nullptr,
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
				base_type( comp, alloc ), 
				mpIndexedValues( pValues )
		{
		}
		CONSTEXPR TBinaryIndexHeap( indexed_values_type * pValues,
			const size_type _size, 
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
				base_type( _size, comp, alloc ), 
					mpIndexedValues( pValues )
		{
		}
		template <typename InputIterator>
		CONSTEXPR TBinaryIndexHeap( indexed_values_type * pValues,
			InputIterator _First, 
			InputIterator _Last, 
			bool assureHeap = true, 
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
				base_type( _First, _Last, assureHeap, comp, alloc ), 
				mpIndexedValues( pValues )
		{
		}
		CONSTEXPR TBinaryIndexHeap( const this_type & rhs ) :
			base_type( rhs ), 
			mpIndexedValues( rhs.mpIndexedValues )
		{
		}
		//TBinaryIndexHeap( indexed_values_type * pValues, 
		//	const pool_type & pool, 
		//	bool assureHeap = true,
		//	const comparator_type & comp = comparator_type() ) : 
		//		base_type( pool, assureHeap, comp ), 
		//		mpIndexedValues( pValues )
		//{
		//}
		CONSTEXPRDTOR_VIRTUAL virtual ~TBinaryIndexHeap()
		{
		}

		CONSTEXPR this_type & operator = ( const this_type & rhs )
		{
			return assign( rhs );
		}
		CONSTEXPR this_type & assign( const this_type & rhs )
		{
			if ( this != &rhs ) 
			{
				this_type new_pool( rhs );
				swap( new_pool );
				mpIndexedValues = rhs.mpIndexedValues;
			}
			return (*this);
		}
		CONSTEXPR void reserve( const size_type numValues )
		{
			this->mElements.reserve( numValues );
		}
		CONSTEXPR void set( indexed_values_type * pValues )
		{
			mpIndexedValues = pValues;
		}
		CONSTEXPR void set( indexed_values_type * pValues, const size_type numValues )
		{
			this->mElements.reserve( numValues );
			mpIndexedValues = pValues;
			for (size_type i = 0; i < numValues; ++i)
			{
				push( i );
			}
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
			this->mElements.assign( base_type::size(), value );
			if ( assureHeap )
			{
				assure_heap( );
			}
		}
		CONSTEXPR void assign( size_type n, const_reference value, bool assureHeap = true )
		{
			this->mElements.assign( n, value );
			if ( assureHeap )
			{
				assure_heap( );
			}
		}
//============================= HEAP BASE INTERFACE ===========================
		CONSTEXPR void push( const_reference element )
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
					//swap_elements( ROOT_INDEX, last_element_index );
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
			//II.2. Second way:
			size_type element_index = base_type::size();
			size_type last_element_index = base_type::LAST_ELEMENT_INDEX();
			while ( --element_index  )
			{
				swap_elements(base_type::ROOT_INDEX, element_index );
				assure_heap_down(base_type::ROOT_INDEX, element_index - 1 );
			}
			if ( reverseOrder )
			{
				swap_elements( element_index, last_element_index );
				while ( ++element_index < --last_element_index )
				{
					swap_elements( element_index, last_element_index );
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
			return base_type::mCompare( mpIndexedValues[ this->mElements[ lhs ] ],
				mpIndexedValues[ this->mElements[ rhs ] ] );
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
			size_type parent_index = base_type::PARENT_INDEX( index );
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
		}
		CONSTEXPR void assure_heap_down( size_type index, const size_type & end )
		{
			//II. 1. unoptimized version:
			size_type child_index = base_type::LEFT_CHILD_INDEX( index );
			while ( child_index < end ) 
			{
				if ( compare_elements ( child_index + 1, child_index ) )
				{
					++child_index;
				}
				if ( compare_elements( child_index, index ) )
				{
					swap_elements( index, child_index );
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
					swap_elements( index, child_index );
				}
			}
		}
	};
//=============================================================================
//====================== INDEXED MIN / MAX HEAP ( PRIORITY QUEUE ) ============
//=============================================================================
	template 
	< 
		typename TValueType,  
		typename TIndexType
	>
	struct TMinBinaryIndexHeap
	{ 
		using Type = TBinaryIndexHeap< TValueType, TIndexType,
			COMPTOOLS::TCompareLess< TValueType > >; 
	};
//=============================================================================
	template 
	< 
		typename TValueType,  
		typename TIndexType
	>
	struct TMaxBinaryIndexHeap
	{ 
		using Type = TBinaryIndexHeap< TValueType, TIndexType,
			COMPTOOLS::TCompareGreater< TValueType > >; 
	};
//=============================================================================
//	template 
//	< 
//		typename TFirstValueType,  
//		typename TSecondValueType, 
//		typename TIndexType
//	>
//	struct TMinBinaryIndexHeapOfPairs
//	{ 
//		typedef PAIR::TPair< TFirstValueType, TSecondValueType > value_pair;
//		typedef TBinaryIndexHeap< value_pair, TIndexType,
//			COMPTOOLS::TCompareLess< value_pair > > Type; 
//	};
////=============================================================================
//	template 
//	< 
//		typename TFirstValueType,  
//		typename TSecondValueType, 
//		typename TIndexType
//	>
//	struct TMaxBinaryIndexHeapOfPairs
//	{ 
//		typedef PAIR::TPair< TFirstValueType, TSecondValueType > value_pair;
//		typedef TBinaryIndexHeap< value_pair, TIndexType, 
//			COMPTOOLS::TCompareGreater< value_pair > > Type; 
//	};
//=============================================================================
} // namespace BINARY_INDEX_HEAP
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_POP
#endif 
//=============================================================================

#endif //__INDEX_HEAP_H__
