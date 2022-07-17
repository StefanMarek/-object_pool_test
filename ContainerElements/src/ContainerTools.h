/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __CONTAINER_TOOLS_H__
#define __CONTAINER_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

namespace CELL { 

namespace TOOLS { 

//=============================================================================
//============================= TContainer Helper =============================
//=============================================================================
namespace MEMORY { // namespace for protection against ADL

	//FORCEINLINE int compare_t(const void * str1, const void * str2, size_t num, int dirver = 0)
	//{
	//	return ::memcmp((str1), (str2), num);
	//}
	//FORCEINLINE void copy_t(void * strDest, const void * strSrc, size_t num, int dirver = 0)
	//{
	//	::memcpy((strDest), (strSrc), num);
	//}
	//FORCEINLINE void clear_t(void * strDest, size_t num, int dirver = 0)
	//{
	//	::memset((strDest), 0, num);
	//}
	//=============================================================================
	template < typename TValueType >
	constexpr TValueType * TAddressOf(TValueType & value) noexcept
	{
#ifdef _MSC_VER 
		return __builtin_addressof(value);
#else
		return (&value);
		//return (reinterpret_cast<_Ty *>((&const_cast<char&>(reinterpret_cast<const volatile char&>(_Val)))));
#endif
	}
	template < typename TValueType >
	constexpr TValueType const * TAddressOf(TValueType const & value) noexcept
	{
#ifdef _MSC_VER 
		return __builtin_addressof(value);
#else
		return (&value);
			//return (reinterpret_cast<_Ty *>((&const_cast<char&>(reinterpret_cast<const volatile char&>(_Val)))));
#endif
	}
	//=============================================================================
	template < typename TValueType, typename TSizeType >
	FORCEINLINE int TIteratorCompare(TValueType * first, TValueType * second, const TSizeType count)
	{	
		for (TSizeType i = 0; i < count; ++i)
		{
			if ((*first) != (*second))
			{
				if ((*first) < (*second))
				{
					return (-1);
				}
				else
				{
					return (+1);
				}
			}
			++first;
			++second;
		}
		return (0);
	}
	//=============================================================================
	template < typename TValueType, typename TSizeType >
	FORCEINLINE TSizeType TIteratorLength(const TValueType * ptr, const TValueType & defaultValue = TValueType())
	{	
		TSizeType count = 0;
		while ((*ptr) != defaultValue)
		{
			++count;
			++ptr;
		}
		return (count);
	}
	//=============================================================================
	template < typename TValueType, typename TSizeType >
	FORCEINLINE TValueType * TIteratorCopy(TValueType * first, TValueType * second, const TSizeType count)
	{	
		TValueType * ret = first;
		for (TSizeType i = 0; i < count; ++i)
		{
			(*first) = (*second);
			++first;
			++second;
		}
		return (ret);
	}
	//=============================================================================
	template < class TContainerType >
	FORCEINLINE void free_container(TContainerType & v)
	{
		TContainerType().swap(v);
	}
	//=============================================================================
	template < class TContainerType >
	FORCEINLINE void shrink_container(TContainerType & v)
	{
		TContainerType(v).swap(v);
	}
	//=============================================================================
	//http://blogs.msdn.com/b/vcblog/archive/2009/02/03/rvalue-references-c-0x-features-in-vc10-part-2.aspx
	template <typename TTYPE>
	struct TRemoveReference
	{
		typedef TTYPE type;
	};
	template <typename TTYPE>
	struct TRemoveReference<TTYPE &>
	{
		typedef TTYPE type;
	};
	template <typename TTYPE>
	struct TRemoveReference<TTYPE &&>
	{
		typedef TTYPE type;
	};
	template <typename TTYPE>
	constexpr typename TRemoveReference<TTYPE>::type && TMove(TTYPE && arg) noexcept
	{
		typedef typename TRemoveReference<TTYPE>::type argument_type;
		return (static_cast<argument_type &&>(arg));
	}
	//=============================================================================
	template <typename TIterator, typename TTYPE>
	FORCEINLINE TIterator TFind(TIterator first, TIterator last, const TTYPE & value)
	{
		while (first != last)
		{
			if ((*first) == value)
			{
				return first;
			}
			++first;
		}
		return last;
	}
	//=============================================================================
	template< typename TIterator, typename TTYPE >
	FORCEINLINE TIterator TRemove(TIterator first, TIterator last, const TTYPE & value)
	{
		first = TFind(first, last, value);
		if (first != last)
		{
			for (TIterator iter = first; iter != last; ++iter)
			{
				if (!((*iter) == value))
				{
					(*first) = TMove(*iter);
					++first;
				}
			}
		}
		return first;
	}
	//=============================================================================
	template<class TTYPE>
	constexpr TTYPE && TForward(typename TRemoveReference<TTYPE>::type & arg) noexcept
	{
		return (static_cast<TTYPE &&>(arg));
	}
	template <typename TTYPE>
	constexpr TTYPE && TForward(typename TRemoveReference<TTYPE>::type && arg) noexcept
	{
		return (static_cast<TTYPE &&>(arg));
	}
} // MEMORY
//=============================================================================
} // namespace TOOLS
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __CONTAINER_TOOLS_H__ 
