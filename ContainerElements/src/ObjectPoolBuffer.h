/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
template <> class TObjectPoolBuffer;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __OBJECT_POOL_BUFFER_ALLOCATOR_H__
#define __OBJECT_POOL_BUFFER_ALLOCATOR_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "ContainerTools.h"

#include <cassert> // assert
#include <cstddef> // ptrdiff_t, size_t
#include <new> // operator new, operator delete
#include <cstring> //memcpy

namespace CELL {

namespace ALLOC {

namespace PBUFFER {

	//=============================================================================
	//================================ TObjectPoolBuffer ==========================
	//=============================================================================
	/*! @brief TObjectPoolBuffer
	IMPLEMENTATION NOTES:
	Stack allocated array wrapper. 
	*/
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS = 32
	>
	class TObjectPoolBuffer
	{
	public:
		typedef TObjectPoolBuffer< TValueType, TELEMENTS > this_type;

		typedef TValueType value_type;
		typedef value_type & reference;
		typedef value_type const & const_reference;
		typedef value_type * pointer;
		typedef value_type const * const_pointer;

		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;

		template < typename UValueType, size_t UELEMENTS >
		struct rebind
		{
			typedef TObjectPoolBuffer< UValueType, UELEMENTS > other;
		};

		enum SIZE : size_type
		{
			TSIZE = TELEMENTS
		};
		enum BYTE : size_type
		{
			TBYTE_SIZE = TELEMENTS * sizeof(TValueType)
		};

		//alignas(::max_align_t) TValueType mBuffer[TELEMENTS];
		TValueType mBuffer[TELEMENTS];

	public:
		CONSTEXPR TObjectPoolBuffer() THROW_NEVER : mBuffer()
		{
		}
		CONSTEXPR TObjectPoolBuffer(const this_type & alloc) THROW_NEVER
		{
			assign(alloc);
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR TObjectPoolBuffer(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc) THROW_NEVER
		{
			assign(alloc);
		}
		CONSTEXPRDTOR_VIRTUAL ~TObjectPoolBuffer() THROW_NEVER
		{
		}

		CONSTEXPR this_type & operator = (const this_type & alloc)
		{
			assign(alloc);
			return (*this);
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR this_type & operator = (const TObjectPoolBuffer<UValueType, UELEMENTS> & alloc)
		{
			assign(alloc);
			return (*this);
		}

		CONSTEXPR pointer allocate(size_type count, const void * hint = nullptr)
		{
			GLOBAL_ASSERT(count && "ERROR NOT ALLOCATABLE: parameter count is 0!");
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
			GLOBAL_ASSERT(nullptr != ptr && "ERROR NOT DEALLOCATABLE!");
			GLOBAL_ASSERT(count && "ERROR NOT DEALLOCATABLE: parameter count is 0!");
			//if (is_in_buffer(ptr))
			//{
			//	deallocate_memory(ptr, count * sizeof(value_type));
			//}
		}

		CONSTEXPR void construct(pointer ptr)
		{
			GLOBAL_ASSERT(ptr != nullptr &&
				"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type();
			}
		}
		template <typename... TArgumentTypes>
		CONSTEXPR void construct(pointer ptr, TArgumentTypes &&... args)
		{
			GLOBAL_ASSERT(ptr != nullptr &&
				"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(CELL::TOOLS::MEMORY::TForward<TArgumentTypes>(args)...);
			}
		}
		CONSTEXPR void construct(pointer ptr, const_reference arg)
		{
			GLOBAL_ASSERT(ptr != nullptr &&
				"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(arg);
			}
		}
		CONSTEXPR void construct(pointer ptr, reference arg)
		{
			GLOBAL_ASSERT(ptr != nullptr &&
				"ERROR NOT CONSTRUCTABLE: parameter pointer hasnt been allocated!");
			if (is_from_buffer(ptr))
			{
				::new (static_cast< void * >(ptr)) value_type(arg);
			}
		}
		CONSTEXPR void destroy(pointer ptr)
		{
			GLOBAL_ASSERT(ptr != nullptr &&
				"ERROR NOT DESTROYABLE: parameter pointer is not constructed!");
			if (is_from_buffer(ptr))
			{
				ptr->~value_type();
			}
		}
		//=============================================================================
		CONSTEXPR bool equal(const this_type & alloc) const
		{
			return (mBuffer == alloc.mBuffer);
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR bool equal(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc) const
		{
			return (TELEMENTS == UELEMENTS && mBuffer == alloc.mBuffer);
		}
		CONSTEXPR void assign(const this_type & alloc)
		{
			if (!equal(alloc))
			{
				::memcpy(mBuffer, alloc.mBuffer, BYTE::TBYTE_SIZE);
			}
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR void assign(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc)
		{
			if (!equal(alloc))
			{
				size_type len = BYTE::TBYTE_SIZE;
				if (len > TObjectPoolBuffer< UValueType, UELEMENTS >::BYTE::TBYTE_SIZE)
				{
					len = TObjectPoolBuffer< UValueType, UELEMENTS >::BYTE::TBYTE_SIZE;
				}
				::memcpy(mBuffer, alloc.mBuffer, len);
			}
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
	class TObjectPoolBuffer< TValueType, 0 >
	{
		public:
		typedef TObjectPoolBuffer< TValueType, 0 > this_type;

		typedef TValueType value_type;
		typedef value_type & reference;
		typedef value_type const & const_reference;
		typedef value_type * pointer;
		typedef value_type const * const_pointer;

		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;

		template < typename UValueType, size_t UELEMENTS >
		struct rebind
		{
			typedef TObjectPoolBuffer< UValueType, UELEMENTS > other;
		};

		enum SIZE : size_type
		{
			TSIZE = 0
		};
		enum BYTE : size_type
		{
			TBYTE_SIZE = 0
		};

	public:
		CONSTEXPR TObjectPoolBuffer() THROW_NEVER
		{
		}
		CONSTEXPR TObjectPoolBuffer(const this_type & alloc) THROW_NEVER
		{
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR TObjectPoolBuffer(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc) THROW_NEVER
		{
		}
		CONSTEXPRDTOR_VIRTUAL ~TObjectPoolBuffer() THROW_NEVER
		{
		}

		CONSTEXPR this_type & operator = (const this_type & alloc)
		{
			return (*this);
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR this_type & operator = (const TObjectPoolBuffer<UValueType, UELEMENTS> & alloc)
		{
			return (*this);
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
			return (true);
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR bool equal(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc) const
		{
			return (true);
		}
		CONSTEXPR void assign(const this_type & alloc)
		{
		}
		template < typename UValueType, size_t UELEMENTS >
		CONSTEXPR void assign(const TObjectPoolBuffer< UValueType, UELEMENTS > & alloc)
		{
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
	class TObjectPoolBuffer< void, TELEMENTS >
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
	CONSTEXPR bool operator == (const TObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TObjectPoolBuffer< TValueType, TELEMENTS > & rhs) THROW_NEVER
	{
		return lhs.equal(rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS
	>
	CONSTEXPR bool operator != (const TObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TObjectPoolBuffer< TValueType, TELEMENTS > & rhs) THROW_NEVER
	{
		return !lhs.equal(rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS,
		typename UValueType,
		size_t UELEMENTS
	>
	CONSTEXPR bool operator == (const TObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TObjectPoolBuffer< UValueType, UELEMENTS > & rhs) THROW_NEVER
	{
		return lhs.equal(rhs);
	}
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TELEMENTS,
		typename UValueType,
		size_t UELEMENTS
	>
	CONSTEXPR bool operator != (const TObjectPoolBuffer< TValueType, TELEMENTS > & lhs, 
		const TObjectPoolBuffer< UValueType, UELEMENTS > & rhs) THROW_NEVER
	{
		return !lhs.equal(rhs);
	}
//=============================================================================
} // namespace PBUFFER
//=============================================================================
} // namespace ALLOC
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __OBJECT_POOL_BUFFER_ALLOCATOR_H__
