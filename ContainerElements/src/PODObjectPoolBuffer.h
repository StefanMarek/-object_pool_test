/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
template <> class TPODObjectPoolBuffer;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __POD_OBJECT_POOL_BUFFER_ALLOCATOR_H__
#define __POD_OBJECT_POOL_BUFFER_ALLOCATOR_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "ContainerTools.h"

#include <cassert> // assert
#include <cstddef> // ptrdiff_t, size_t
#include <new> // operator new, operator delete

namespace CELL {

namespace ALLOC {

namespace BUFFERPOD {

	//=============================================================================
	//============================== TPODObjectPoolBuffer =========================
	//=============================================================================
	/*! @brief TPODObjectPoolBuffer
	IMPLEMENTATION NOTES:
	Stack allocated array wrapper. POD style!
	*/
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS 
	>
	struct TPODObjectPoolBuffer
	{
		typedef TPODObjectPoolBuffer< TValueType, TELEMENTS > this_type;

		typedef TValueType value_type;
		typedef value_type & reference;
		typedef value_type const & const_reference;
		typedef value_type * pointer;
		typedef value_type const * const_pointer;

		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;

		enum SIZE : size_type
		{
			TSIZE = TELEMENTS
		};

		TValueType mBuffer[TELEMENTS];
		//alignas(TELEMENTS) TValueType mBuffer[TELEMENTS];
		//alignas(::max_align_t) TValueType mBuffer[TELEMENTS];

		//TValueType mBuffer[TELEMENTS];
		//union
		//{	
		//	pointer mpPtr;
		//	char Alias[TELEMENTS];	// to permit aliasing
		//};

		CONSTEXPR size_type get_sizeof() const
		{
			return (sizeof(mBuffer));
		}
		CONSTEXPR pointer get()
		{
			return (mBuffer);
		}
		CONSTEXPR const_pointer get() const
		{
			return (mBuffer);
		}
		CONSTEXPR pointer allocate(size_type count, const void * hint = nullptr)
		{
			//GLOBAL_ASSERT(count && "ERROR NOT ALLOCATABLE: parameter count is 0!");
			if (is_in_buffer(count))
			{
				return (mBuffer);
			}
			else
			{
				return nullptr;
			}
		}
		CONSTEXPR void deallocate(pointer ptr, size_type count)
		{
			//GLOBAL_ASSERT(nullptr != ptr && "ERROR NOT DEALLOCATABLE!");
			//GLOBAL_ASSERT(count && "ERROR NOT DEALLOCATABLE: parameter count is 0!");
			//if (is_in_buffer(ptr))
			//{
			//	deallocate_memory(ptr, count * sizeof(value_type));
			//}
		}

		CONSTEXPR void construct(pointer ptr)
		{
			//GLOBAL_ASSERT(ptr != nullptr &&
			//	"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type();
			}
		}
		template <typename... TArgumentTypes>
		CONSTEXPR void construct(pointer ptr, TArgumentTypes &&... args)
		{
			//GLOBAL_ASSERT(ptr != nullptr &&
			//	"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(CELL::TOOLS::MEMORY::TForward<TArgumentTypes>(args)...);
			}
		}
		CONSTEXPR void construct(pointer ptr, const_reference arg)
		{
			//GLOBAL_ASSERT(ptr != nullptr &&
			//	"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(arg);
			}
		}
		CONSTEXPR void construct(pointer ptr, reference arg)
		{
			//GLOBAL_ASSERT(ptr != nullptr &&
			//	"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(arg);
			}
		}
		CONSTEXPR void destroy(pointer ptr)
		{
			//GLOBAL_ASSERT(ptr != nullptr &&
			//	"ERROR NOT DESTROYABLE: parameter pointer is not constructed!");
			//if (is_from_buffer(ptr))
			//{
			//	ptr->~value_type();
			//}
		}
		//=============================================================================
		CONSTEXPR bool equal(const this_type & alloc) const
		{
			return (mBuffer == alloc.mBuffer);
		}
		CONSTEXPR bool is_in_buffer(const size_type size) const
		{
			return (0 < size && size <= TELEMENTS);
		}
		CONSTEXPR bool is_from_buffer(pointer ptr) const
		{
			return ((mBuffer <= ptr) && (ptr < (mBuffer + TELEMENTS)));
		}
		CONSTEXPR bool is_from_buffer(const_pointer ptr) const
		{
			return ((mBuffer <= ptr) && (ptr < (mBuffer + TELEMENTS)));
		}
	};
	//=============================================================================
	template
	<
		typename TValueType
	>
	struct TPODObjectPoolBuffer< TValueType, 0 >
	{
		typedef TPODObjectPoolBuffer< TValueType, 0 > this_type;

		typedef TValueType value_type;
		typedef value_type & reference;
		typedef value_type const & const_reference;
		typedef value_type * pointer;
		typedef value_type const * const_pointer;
		
		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;

		enum SIZE : size_type
		{
			TSIZE = 0
		};

		CONSTEXPR size_type get_sizeof() const
		{
			return (0);
		}
		CONSTEXPR pointer get()
		{
			return nullptr;
		}
		CONSTEXPR const_pointer get() const
		{
			return nullptr;
		}
		CONSTEXPR pointer allocate(size_type count, const void * hint = nullptr)
		{
			return nullptr;
		}
		CONSTEXPR void deallocate(pointer ptr, size_type count)
		{
		}
		CONSTEXPR void construct(pointer ptr)
		{
		}
		template <typename... TArgumentTypes>
		CONSTEXPR void construct(pointer ptr, TArgumentTypes &&... args)
		{
		}
		CONSTEXPR void construct(pointer ptr, const_reference arg)
		{
		}
		CONSTEXPR void construct(pointer ptr, reference arg)
		{
		}
		CONSTEXPR void destroy(pointer ptr)
		{
		}
		//=============================================================================
		CONSTEXPR bool equal(const this_type & alloc) const
		{
			return true;
		}
		CONSTEXPR bool is_in_buffer(const size_type size) const
		{
			return false;
		}
		CONSTEXPR bool is_from_buffer(pointer ptr) const
		{
			return false;
		}
		CONSTEXPR bool is_from_buffer(const_pointer ptr) const
		{
			return false;
		}
	};
	//=============================================================================
	// Allocator specialization for type void.
	//=============================================================================
	template
	<
		size_t TELEMENTS
	>
	struct TPODObjectPoolBuffer< void, TELEMENTS >
	{
	public:
		typedef void value_type;
		typedef void * pointer;
		typedef void const * const_pointer;
	};
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS
	>
	CONSTEXPR bool operator == (const TPODObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TPODObjectPoolBuffer< TValueType, TELEMENTS > & rhs) THROW_NEVER
	{
		return lhs.equal(rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS
	>
	CONSTEXPR bool operator != (const TPODObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TPODObjectPoolBuffer< TValueType, TELEMENTS > & rhs) THROW_NEVER
	{
		return !lhs.equal(rhs);
	}
//=============================================================================
} // namespace BUFFERPOD
//=============================================================================
} // namespace ALLOC
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __POD_OBJECT_POOL_BUFFER_ALLOCATOR_H__
