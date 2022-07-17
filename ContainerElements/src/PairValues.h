/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __PAIR_VALUES_H__
#define __PAIR_VALUES_H__

#include "Definitions.h"
#include "ErrorHandling.h"

//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif 
//=============================================================================

namespace CELL {

namespace PAIR {

	//=============================================================================
	//================================ TPair ======================================
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	class TPair
	{
	public:
		using this_type = TPair< TValueType1, TValueType2 >;
		using first_type = TValueType1;
		using second_type = TValueType2;

	public:
		TValueType1 first;
		TValueType2 second;

	public:
		TPair();
		TPair(const TValueType1 & ref1, const TValueType2 & ref2);
		TPair(const TPair<TValueType1, TValueType2> & rhs);
		~TPair() = default;

		TPair<TValueType1, TValueType2> & operator = (const TPair<TValueType1, TValueType2> & rhs);
		void swap(TPair<TValueType1, TValueType2> & rhs);
	};
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	TPair<TValueType1, TValueType2>::TPair() : first(), second()
	{
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	TPair<TValueType1, TValueType2>::TPair(const first_type & ref1, const second_type & ref2)
		: first(ref1), second(ref2)
	{
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	TPair<TValueType1, TValueType2>::TPair(const TPair<TValueType1, TValueType2> & rhs) : 
		first(rhs.first), second(rhs.second)
	{
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	void TPair<TValueType1, TValueType2>::swap(TPair<TValueType1, TValueType2> & rhs)
	{
		const first_type tmp_left = first;
		first = rhs.first;
		rhs.first = tmp_left;

		const second_type tmp_right = second;
		second = rhs.second;
		rhs.second = tmp_right;
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	TPair<TValueType1, TValueType2> & 
	TPair<TValueType1, TValueType2>::operator = (const TPair<TValueType1, TValueType2> & rhs)
	{
		if (this != &rhs)
		{
			first = rhs.first;
			second = rhs.second;
		}
		return (*this);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	void swap(TPair< TValueType1, TValueType2 > & lhs,
		TPair< TValueType1, TValueType2 > & rhs)
	{
		lhs.swap(rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	TPair< TValueType1, TValueType2 > 
	pair(const TValueType1 & lhs, const TValueType2 & rhs)
	{
		return TPair< TValueType1, TValueType2 >(lhs, rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator == (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first == rhs.first);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator != (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first != rhs.first);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator < (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first < rhs.first);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator > (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first > rhs.first);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator <= (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first <= rhs.first);
	}
	//=============================================================================
	template
	<
		typename TValueType1,
		typename TValueType2
	>
	bool operator >= (const TPair< TValueType1, TValueType2 > & lhs,
		const TPair< TValueType1, TValueType2 > & rhs)
	{
		return (lhs.first >= rhs.first);
	}
//=============================================================================
} // namespace REFERENCE
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
	#pragma PRAGMA_PACK_POP
#endif 
//=============================================================================

#endif //__PAIRED_VALUES_H__
