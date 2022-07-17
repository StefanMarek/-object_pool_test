/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	implements: 
					namespace CONTAINER_FROM_BOOST
					struct checked_deleter;
					struct checked_array_deleter;
					namespace ALLOCATION_TOOLS
					struct TSafeDeleteType;
					struct TSafeNewType;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __ALLOCATION_TOOLS_H__
#define __ALLOCATION_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include <cstddef> // ptrdiff_t, size_t, ptrdiff_t, NULL, offsetof

//=============================================================================
//========================== CONTAINER_FROM_BOOST  ============================
//=============================================================================
namespace CONTAINER_FROM_BOOST {
//=============================================================================
//========================== CHECKED DELETER  =================================
//=============================================================================
// http://www.boost.org/doc/libs/1_36_0/libs/utility/checked_delete.html
// INFORMATION: The checked deleter methods forbid the deletion of incomplete 
// types (like void *). Actually it doesnt allow to generate the template code 
// at compile time. (This will lead to an compile error! ).
//=============================================================================
	template<class T> 
	void checked_delete(T * & p)
	{
		// intentionally complex - simplification causes regressions
		typedef char type_must_be_complete[ sizeof(T) ? 1 : -1 ];
		(void) sizeof(type_must_be_complete);
		if (p)
		{
			::delete p;
			p = NULL;
		}
	}
	template<class T> 
	struct checked_deleter
	{
		typedef void result_type;
		typedef T * argument_type;

		void operator () (T * & x) const
		{
			checked_delete(x);
		}
	};
//=============================================================================
	template<class T> 
	void checked_array_delete(T * & p)
	{
		typedef char type_must_be_complete[ sizeof(T) ? 1 : -1 ];
		(void) sizeof(type_must_be_complete);
		if (p)
		{
			::delete [] p;
			p = NULL;
		}
	}
	template<class T> 
	struct checked_array_deleter
	{
		typedef void result_type;
		typedef T * argument_type;

		void operator () (T * & x) const
		{
			checked_array_delete(x);
		}
	};
//=============================================================================
} // CONTAINER_FROM_BOOST
//=============================================================================
//=========================== ALLOCATION_TOOLS ================================
//=============================================================================
namespace ALLOCATION_TOOLS { 
//=============================================================================
//=========================== CHECKED NEW / DELETE ============================
//=============================================================================
// Using this methods means that there has to be a fully specifyable type. 
// This means no uncomplete specified type like void * allowed.
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_delete(TYPE * & data, bool isArray)	
	{
		if (data)
		{
			if (isArray)
			{
				CONTAINER_FROM_BOOST::checked_array_delete<TYPE>( data );
				//delete[] data;
			}
			else
			{
				CONTAINER_FROM_BOOST::checked_delete<TYPE>( data );
				//delete data;
			}
			data = NULL;
		}
	}
//=============================================================================
	template <typename TYPE, bool TIsArray> 
	struct TSafeDeleteType
	{
		typedef void result_type;
		typedef TYPE * argument_type;

		void operator () (TYPE * & data) const
		{
			safe_delete<TYPE>( data, TIsArray );
		}
	};
//=============================================================================
	// For functor based template classes like TScoped and TShared.
	template <typename TYPE> 
	struct TNoDeletion
	{
		typedef void result_type;
		typedef TYPE * argument_type;

		void operator () (TYPE * & data) const
		{
		}
	};
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_new(TYPE * & data, const size_t & size, bool isArray)	
	{
		safe_delete<TYPE>( data, isArray );
		if (isArray)
		{
			data = ::new (std::nothrow) TYPE[size];
			//memset( static_cast<void *>(data), 0, size * sizeof( TYPE ) );
		}
		else
		{
			data = ::new (std::nothrow) TYPE();
			//data = new TYPE();
		}
	}
//=============================================================================
	template <typename TYPE, size_t TSIZE, bool TIsArray> 
	struct TSafeNewType
	{
		typedef void result_type;
		typedef TYPE * argument_type;

		void operator () (TYPE * & data) const
		{
			safe_new<TYPE>( data, TSIZE, TIsArray );
		}
	};
//=============================================================================
//=========================== UNCHECKED NEW / DELETE ==========================
//=============================================================================
	//template <typename TYPE>
	//INLINE void 
	//safe_delete(TYPE & data)	
	//{
	//}
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_delete(TYPE * & data)	
	{
		if (data)
		{
			::delete data;
			data = NULL;
		}
	}
//=============================================================================
	//template <typename TYPE>
	//INLINE void 
	//safe_delete(void * & data)	
	//{
	//}
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_delete_array(TYPE * & data)
	{
		if (data)
		{
			::delete[] data;
			data = NULL;
		}
	}
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_new(TYPE * & data)	
	{
		safe_delete<TYPE>(data);
		data = ::new (std::nothrow) TYPE();
	}
//=============================================================================
	template <typename TYPE>
	INLINE void 
	safe_new_array(TYPE * & data, const size_t & size)	
	{
		if (size)
		{
			safe_delete_array<TYPE>(data);
			data = ::new (std::nothrow) TYPE[size];
		}
	}
//=============================================================================
} // namespace ALLOCATION_TOOLS
//=============================================================================

#endif // __ALLOCATION_TOOLS_H__ 
