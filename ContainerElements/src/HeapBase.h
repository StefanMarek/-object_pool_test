/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
					template <> class THeapBase; 
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __HEAP_BASE_H__
#define __HEAP_BASE_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "MathTools.h"

#include "Allocator.h"
#include "ObjectPool.h"

//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif 
//=============================================================================

namespace CELL { 

namespace HEAP_BASE { 
	
//=============================================================================
//============================= THeapBase =====================================
//=============================================================================
/*! @brief THeapBase 
	THeapBase is the base class of all heap based datastructures. 
	The heap base class uses the binary tree pool to store and grow its values.
	This class provides methods for random access. 
	DESIGN NOTES:
	The cost for this is that the at method has to be virtual. 
	The reason is that a concrete implementation of a heap based datastructure 
	using this base class will define its own memory pool. 
	In case of an implicit datastructure like vector, or object pool 
	nothing has to be overloaded, because the default implementation is exactly 
	for such kind of memory pools. 
	If the pool type is a list the at method has to be overloaded with a 
	searching strategy for a reference at the given index. 
	In case of a list this could be:
	virtual reference at( const size_type & i )
	{
		iterator advanced_iterator = mElements.begin();
		std::advance( advanced_iterator, pos );
		return (*advanced_iterator);
	}
	I will provide indexed heap versions for other containers.
	A heap with indices instead of values should perform best.
*/
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>
	class THeapBase 
	{
	public:
		using this_type = THeapBase
		< 
			TValueType, 
			TCompType 
		>;

		//typedef TPoolType pool_type;
		typedef typename OPOOL::TBTPool< TValueType,
			DEFAULT_OBJECT_ALLOCATOR< TValueType > >::Type pool_type;

		typedef typename pool_type::allocator_type allocator_type;
		typedef typename pool_type::size_type size_type;
		typedef typename pool_type::difference_type difference_type;
		typedef typename pool_type::value_type value_type;

		typedef typename pool_type::reference reference;
		typedef typename pool_type::const_reference const_reference;
		typedef typename pool_type::pointer pointer;
		typedef typename pool_type::const_pointer const_pointer;

		typedef typename pool_type::iterator iterator;
		typedef typename pool_type::const_iterator const_iterator;
		typedef typename pool_type::reverse_iterator reverse_iterator;      
		typedef typename pool_type::const_reverse_iterator const_reverse_iterator;

		typedef TCompType comparator_type;
		//typedef typename pool_type::growing_type growing_type;

	protected:
		enum
		{
			ROOT_INDEX = 0,
			FIRST_INDEX = 1, 
			SECOND_INDEX = 2
		};
		comparator_type mCompare;
		pool_type mElements;
		//value_type mSwapTmp;

	public:
		CONSTEXPR THeapBase( const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			mElements( alloc ), mCompare( comp )
		{
		}
		CONSTEXPR THeapBase( const size_type & _size,
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			mElements( _size, alloc ), mCompare( comp )
		{
		}
		CONSTEXPR THeapBase( const size_type & _size,
			const_reference val,// = value_type(), 
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			mElements( _size, val, alloc ), mCompare( comp )
		{
		}
		template <typename InputIterator>
		CONSTEXPR THeapBase( InputIterator _First, InputIterator _Last,
			const comparator_type & comp = comparator_type(), 
			const allocator_type & alloc = allocator_type() ) : 
			mElements( _First, _Last, alloc ), mCompare( comp )
		{
		}
		CONSTEXPR THeapBase( const pool_type & pool,
			const comparator_type & comp = comparator_type() ) : 
			mElements( pool ), mCompare( comp )
		{
		}
		CONSTEXPRDTOR_VIRTUAL virtual ~THeapBase()
		{
			clear();
		}

		CONSTEXPR void reserve(size_type new_capacity_size)
		{
			mElements.reserve(new_capacity_size);
		}
//============================= HEAP BASE INTERFACE ===========================
		//virtual void push( const_reference element ) = 0;
		//virtual void pop() = 0; 
		//virtual void sort( bool reverseOrder = false ) = 0;
		//virtual void assure_heap() = 0;
		//virtual bool is_heap() const = 0;
//============================= HEAP BASE INTERFACE ===========================
		CONSTEXPR const pool_type & get_pool() const
		{
			return mElements;
		}
		CONSTEXPR pointer get() const
		{
			return ( &at(0) );
		}
		CONSTEXPR void * get_void() const
		{
			return ( &at(0) );
		}
//=========================== STL compliant interface begin ===================
		CONSTEXPR reference at( const size_type & i )
		{
			return mElements[ i ];
			//return mElements.at( i );
		}
		CONSTEXPR const_reference at( const size_type & i ) const
		{
			return mElements[ i ];
			//return mElements.at( i );
		}
		CONSTEXPR reference operator [] ( const size_type & i )
		{
			return mElements[ i ];
			//return at( i );
		}
		CONSTEXPR const_reference operator [] ( const size_type & i ) const
		{
			return mElements[ i ];
			//return at( i );
		}
		CONSTEXPR const allocator_type & get_allocator() const
		{
			return mElements.get_allocator();
		}
		CONSTEXPR reference front()
		{
			return mElements.front();
		}
		CONSTEXPR const_reference front() const
		{
			return mElements.front();
		}
		CONSTEXPR reference back()
		{
			return mElements.back();
		}
		CONSTEXPR const_reference back() const
		{
			return mElements.back();
		}
		CONSTEXPR iterator begin()
		{
			return mElements.begin();
		}
		CONSTEXPR const_iterator begin() const
		{
			return mElements.begin();
		}
		CONSTEXPR iterator end()
		{
			return mElements.end();
		}
		CONSTEXPR const_iterator end() const
		{
			return mElements.end();
		}
		CONSTEXPR reverse_iterator rbegin()
		{
			return mElements.end();
		}
		CONSTEXPR const_reverse_iterator rbegin() const
		{
			return mElements.end();
		}
		CONSTEXPR reverse_iterator rend()
		{
			return mElements.begin();
		}
		CONSTEXPR const_reverse_iterator rend() const
		{
			return mElements.begin();
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
		CONSTEXPR void clear()
		{
			mElements.clear();
		}
		//void reserve( const size_type & news )
		//{
		//	mElements.reserve( news );
		//}
		//void resize( const size_type & news ) 
		//{
		//	mElements.resize( news );
		//}
		CONSTEXPR void swap( this_type & rhs )
		{
			mElements.swap( rhs.mElements );
		}
//=========================== STL compliant interface end ===================
		CONSTEXPR const comparator_type & get_comparator() const
		{
			return mCompare;
		}
		CONSTEXPR reference top()
		{
			return at( FIRST_ELEMENT_INDEX() );
		}
		CONSTEXPR const_reference top() const
		{
			return at( FIRST_ELEMENT_INDEX() );
		}
		CONSTEXPR reference bottom()
		{
			return at( LAST_ELEMENT_INDEX() );
		}
		CONSTEXPR const_reference bottom() const
		{
			return at( LAST_ELEMENT_INDEX() );
		}
		CONSTEXPR reference getRoot()
		{
			return at( FIRST_ELEMENT_INDEX() );
		}
		CONSTEXPR reference getParent( const size_type & i )
		{
			return at( PARENT_INDEX( i ) );
		}
		CONSTEXPR reference getLeftChild( const size_type & i )
		{
			return at( LEFT_CHILD_INDEX( i ) );
		}
		CONSTEXPR reference getRightChild( const size_type & i )
		{
			return at( RIGHT_CHILD_INDEX( i ) );
		}
		CONSTEXPR bool isRoot(const size_type & i) const
		{ 
			return ( i == FIRST_ELEMENT_INDEX() );
		}
		CONSTEXPR bool hasParent( const size_type & index ) const
		{
			return ( index != FIRST_ELEMENT_INDEX()  ); 
		}
		CONSTEXPR bool hasLeftLeaf(const size_type & i) const
		{ 
			return ( LEFT_CHILD_INDEX( i ) < size() );
		}
		CONSTEXPR bool hasRightLeaf(const size_type & i) const
		{ 
			return ( RIGHT_CHILD_INDEX( i ) < size() );
		}
//=============================================================================
	protected:
		//virtual bool compare_elements( 
		//	const size_type & lhs, const size_type & rhs ) const = 0;
	
		//bool compare_elements( const_reference lhs, const_reference rhs ) const
		//{
		//	return mCompare( lhs, rhs );
		//}
		CONSTEXPR void swap_elements( const size_type & lhs, const size_type & rhs )
		{
			//swap_elements( at( lhs ), at( rhs ) );
			swap_elements( mElements[ lhs ], mElements[ rhs ] );
		}
		CONSTEXPR void swap_elements( reference lhs, reference rhs )
		{
			const value_type mSwapTmp( lhs );
			//mSwapTmp = lhs;
			lhs = rhs;
			rhs = mSwapTmp;
		}
		CONSTEXPR void swap_elements( pointer & lhs, pointer & rhs )
		{
			const value_type mSwapTmp( *lhs );
			//mSwapTmp = lhs;
			*lhs = *rhs;
			*rhs = *mSwapTmp;
		}
		//void swap_elements( iterator & lhs, iterator & rhs ) 
		//{
		//	const value_type temp( *lhs );
		//	*lhs = *rhs;
		//	*rhs = *temp;
		//}
		CONSTEXPR size_type FIRST_ELEMENT_INDEX() const
		{
			return ROOT_INDEX;			
		}
		CONSTEXPR size_type LAST_ELEMENT_INDEX() const
		{
			return ( size() - 1 );
		}
		CONSTEXPR size_type MIDDLE_ELEMENT_INDEX() const
		{
			return ( ( size() - 1 ) - FIRST_ELEMENT_INDEX() ) >> 1;			
		}
		CONSTEXPR size_type LEVEL_ELEMENTS_AT_INDEX( const size_type & index ) const
		{
			//return power_of_two_int( log_dualis_int_floor( index + 1 ) );
			return CELL::TOOLS::POW2::next_power_of_two_int( index + 1 );
		}
		CONSTEXPR size_type LEVEL_AT_INDEX( const size_type & index ) const
		{
			return CELL::TOOLS::LOG2::log_dualis_int_floor( index + 1 );
		}
		CONSTEXPR size_type LAST_ELEMENT_LEVEL_AT_INDEX() const
		{
			return CELL::TOOLS::LOG2::log_dualis_int_floor( size() );
		}
		// nodes interval: start index: 0; end index: mSize / 2 - 1
		CONSTEXPR size_type NODES_START_INDEX() const
		{
			return FIRST_ELEMENT_INDEX();
		}
		CONSTEXPR size_type NODES_END_INDEX() const
		{
			return ( ( size() >> 1 ) - 1 );
		}
		// leafs interval: start index: mSize / 2; end index mSize - 1
		CONSTEXPR size_type LEAFS_START_INDEX() const
		{
			return ( size() >> 1 );
		}
		CONSTEXPR size_type LEAFS_END_INDEX() const
		{
			return ( size() - 1 );
		}
		// middle node index: 
		//size_type MIDDLE_NODE_INDEX() const;

		// node relation indices: 
		CONSTEXPR size_type PARENT_INDEX( const size_type & index ) const
		{
			//GLOBAL_ASSERT( isRootIndex( index ) );
			//return ( (index - 1) >> 1 );
			return ( index ? (index - 1) >> 1 : FIRST_ELEMENT_INDEX() );
			//return ( i > 2 ? i >> 1 : ROOT_INDEX );
		}
		CONSTEXPR size_type GRAND_PARENT_INDEX( const size_type & index ) const
		{
			const size_type parent_index = ( index ? (index - 1) >> 1 : 
				FIRST_ELEMENT_INDEX() );
			return ( parent_index ? 
				(parent_index - 1) >> 1 : FIRST_ELEMENT_INDEX() );
		}
		CONSTEXPR size_type LEFT_CHILD_INDEX( const size_type & index ) const
		{
			//GLOBAL_ASSERT( isRootIndex( index ) );
			return ( (index << 1) + 1 );
			//return ( i > 0 ? i << 1 : ROOT_INDEX );
		}
		CONSTEXPR size_type LEFT_GRAND_CHILD_INDEX( const size_type & index ) const
		{
			return ( (index << 2) + 3 );
		}
		CONSTEXPR size_type RIGHT_CHILD_INDEX( const size_type & index ) const
		{
			//GLOBAL_ASSERT( isRootIndex( index ) );
			return ( (index << 1) + 2 );
			//return ( i > 0 ? (i << 1) + 1 : ROOT_INDEX );
		}
		CONSTEXPR size_type RIGHT_GRAND_CHILD_INDEX( const size_type & index ) const
		{
			return ( (index << 2) + 6 );
		}
	};
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>
	CONSTEXPR void swap( THeapBase< TValueType, TCompType > & lhs,
		THeapBase< TValueType, TCompType > & rhs )
	{
		lhs.swap( rhs );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>
	CONSTEXPR bool operator == (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() == rhs.begin() );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>	
	CONSTEXPR bool operator != (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() != rhs.begin() );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>	
	CONSTEXPR bool operator < (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() < rhs.begin() );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>	
	CONSTEXPR bool operator > (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() > rhs.begin() );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>	
	CONSTEXPR bool operator <= (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() <= rhs.begin() );
	}
//=============================================================================
	template 
	< 
		typename TValueType, 
		typename TCompType
	>	
	CONSTEXPR bool operator >= (
		const THeapBase< TValueType, TCompType > & lhs, 
		const THeapBase< TValueType, TCompType > & rhs )
	{
		return ( lhs.begin() >= rhs.begin() );
	}
//=============================================================================
} // namespace HEAP_BASE
  //=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_POP
#endif 
//=============================================================================

#endif //__HEAP_BASE_H__
