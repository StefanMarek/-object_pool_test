/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
	template<,,> class TStringPool
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __STRING_POOL_H__
#define __STRING_POOL_H__

#include <cassert>

#include "Definitions.h"
#include "ErrorHandling.h"
#include "ContainerTools.h"

#include "ObjectPool.h"
#include "ObjectPoolPOD.h"
#include "Allocator.h"
#include "PoolGrowing.h"

#include <cwchar>
#include <cstring>

#ifdef USE_INITIALIZER_LIST
#include <initializer_list>
#endif

//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif  
//=============================================================================

namespace CELL {

namespace SPOOL {
	
	FORCEINLINE int spcompare(const char * str1, const char * str2, size_t num)
	{
#ifdef _MSC_VER
		return __builtin_memcmp((str1), (str2), num);
#else
		return ::memcmp((str1), (str2), num);
#endif
	}
	FORCEINLINE size_t splength(const char * str)
	{
#ifdef _MSC_VER
		return __builtin_strlen(str);
#else
		return ::strlen(str);
#endif
	}
	FORCEINLINE void spcopy(char * strDest, const char * strSrc, size_t num)
	{
		::memcpy((strDest), (strSrc), num);
	}
	FORCEINLINE int spcompare(const wchar_t * str1, const wchar_t * str2, size_t num)
	{
#ifdef _MSC_VER
		return __builtin_wmemcmp((str1), (str2), num);
#else
		return ::wmemcmp(str1, str2, num);
#endif
	}
	FORCEINLINE size_t splength(const wchar_t * str)
	{
#ifdef _MSC_VER
		return __builtin_wcslen(str);
#else
		return ::wcslen(str);
#endif
	}
	FORCEINLINE void spcopy(wchar_t * strDest, const wchar_t * strSrc, size_t num)
	{
		::wmemcpy(strDest, strSrc, num);
	}
	//=============================================================================
	//============================= StringPool ====================================
	//=============================================================================
	template
	<
		typename TValueType,
		typename TAllocType,
		typename TGrowStratType
	>
	class TStringPool : 
		public OPOOLPOD::TObjectPoolPOD<TValueType, TAllocType, TGrowStratType>
		//public OPOOL::TObjectPool<TValueType, TAllocType, TGrowStratType>
	{
	public:
		using base_type = OPOOLPOD::TObjectPoolPOD<TValueType, TAllocType, TGrowStratType>;
		//typedef CELL::OPOOL::TObjectPool<TValueType, TAllocType, TGrowStratType> base_type;

		using this_type = TStringPool<TValueType, TAllocType, TGrowStratType >;

		typedef TAllocType allocator_type;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;
		typedef typename allocator_type::value_type value_type;

		typedef TGrowStratType growing_type;

		typedef value_type & reference;
		typedef value_type const & const_reference;
		typedef value_type * pointer;
		typedef value_type const * const_pointer;

		// The iterator typedefs are provided to be compliant with the STL. 
		typedef pointer iterator;
		typedef const_pointer const_iterator;
		typedef pointer reverse_iterator;
		typedef const_pointer const_reverse_iterator;

		static CONSTEXPR const size_type npos = (size_type)-1;
		static CONSTEXPR const value_type space = ' ';
		static CONSTEXPR const value_type endl = '\n';
		static CONSTEXPR const value_type delimiter = '\0';
		static CONSTEXPR const value_type path_seperator = '\\';
		
	public:
		// default constructor
		CONSTEXPR TStringPool() THROW_NEVER : base_type()
		{
			addDelimiter();
		}
		CONSTEXPR TStringPool(const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER :
				base_type(alloc, grow)
		{
			addDelimiter();
		}
		// copy initializer for copy constructor
		CONSTEXPR TStringPool(const this_type & pool) THROW_NEVER :
			base_type(pool.get_allocator(), pool.get_growing())
		{
			if (!pool.empty())
			{
				//initializeCopy(pool.begin(), pool.end());
				const size_type len = pool.size();
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool.begin(), len, 0);
			}
			addDelimiter();
		}
		CONSTEXPR TStringPool(const this_type & pool, const size_type pos, const size_type len) THROW_NEVER :
			base_type(pool.get_allocator(), pool.get_growing())
		{
			if (len > 0 && !pool.empty())
			{
				//initializeCopy(pool.begin() + pos, pool.begin() + pos + len);
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool.begin() + pos, len, 0);
			}
			addDelimiter();
		}
		CONSTEXPR TStringPool(this_type && pool) noexcept :
			base_type(pool.get_allocator(), pool.get_growing())
		{
			move(CELL::TOOLS::MEMORY::TForward<this_type>(pool));
		}
#ifdef USE_INITIALIZER_LIST
		CONSTEXPR TStringPool(std::initializer_list<TValueType> pool) THROW_NEVER :
			base_type()
		{
			base_type::initializeCopy(pool.begin(), pool.end());
			addDelimiter();
		}
		CONSTEXPR TStringPool(std::initializer_list<TValueType> pool,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) :
				base_type(alloc, grow)
		{
			base_type::initializeCopy(pool.begin(), pool.end());
			addDelimiter();
		}
#endif
		CONSTEXPR TStringPool(const_pointer pool) THROW_NEVER :
			base_type()
		{
			const size_type len = strlength(pool);
			if (len > 0)
			{
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool, len, 0);
			}
			addDelimiter();
		}
		CONSTEXPR TStringPool(const_pointer pool,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER :
				base_type(alloc, grow)
		{
			const size_type len = strlength(pool);
			if (len > 0)
			{
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool, len, 0);
			}
			addDelimiter();
		}
		CONSTEXPR TStringPool(const_pointer pool, const size_type len) THROW_NEVER :
			base_type()
		{
			size_type sz = strlength(pool);
			if (sz > 0)
			{
				if (len < sz)
				{
					sz = len;
				}
				//initializeCopy(pool, pool + sz);
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool, sz, 0);
			}
			addDelimiter();
		}
		CONSTEXPR TStringPool(const_pointer pool, const size_type len,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER :
				base_type(alloc, grow)
		{
			size_type sz = strlength(pool);
			if (sz > 0)
			{
				if (len < sz)
				{
					sz = len;
				}
				//initializeCopy(pool, pool + sz);
				if (!this->mBuffer.is_in_buffer(len))
				{
					initializeStringPool(len);
				}
				this->mpLastElement = this->mpFirstElement + len;
				strcopy(pool, sz, 0);
			}
			addDelimiter();
		}
		// initializer for default constructor
		CONSTEXPR TStringPool(const size_type & size_count) THROW_NEVER : base_type(size_count)
		{
			addDelimiter();
		}
		CONSTEXPR TStringPool(const size_type & size_count,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(size_count, alloc, grow)
		{
			addDelimiter();
		}
		// fill initializer for copy constructor
		CONSTEXPR TStringPool(const size_type & size_count, const_reference val) THROW_NEVER : base_type(size_count, val)
		{
			addDelimiter();
		}
		CONSTEXPR TStringPool(const size_type & size_count, const_reference val,
			const allocator_type & alloc = allocator_type(),
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(size_count, val, alloc, grow)
		{
			addDelimiter();
		}
		// fill initializer for copy constructor
		CONSTEXPR TStringPool(const size_type & size_count,
			const size_type & capacity_count,
			const_reference val = value_type()) THROW_NEVER : base_type(size_count, capacity_count, val)
		{
			addDelimiter();
		}
		CONSTEXPR TStringPool(const size_type & size_count,
			const size_type & capacity_count,
			const_reference val,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(size_count, capacity_count, val, alloc, grow)
		{
			addDelimiter();
		}
		// copy initializer for copy constructor
		template <typename InputIterator>
		CONSTEXPR TStringPool(InputIterator _Begin, InputIterator _End) THROW_NEVER : base_type(_Begin, _End)
		{
			addDelimiter();
		}
		template <typename InputIterator>
		CONSTEXPR TStringPool(InputIterator _Begin, InputIterator _End,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(_Begin, _End, alloc, grow)
		{
			addDelimiter();
		}
		 // copy and merge initializer for copy constructor
		template <typename InputIterator1, typename InputIterator2>
		CONSTEXPR TStringPool(
			InputIterator1 _Begin1, InputIterator1 _End1,
			InputIterator2 _Begin2, InputIterator2 _End2) THROW_NEVER : base_type(_Begin1, _End1, _Begin2, _End2)
		{
			addDelimiter();
		}
		template <typename InputIterator1, typename InputIterator2>
		CONSTEXPR TStringPool(
			InputIterator1 _Begin1, InputIterator1 _End1,
			InputIterator2 _Begin2, InputIterator2 _End2,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(_Begin1, _End1, _Begin2, _End2, alloc, grow)
		{
			addDelimiter();
		}
		// copy and fill initializer for copy constructor
		CONSTEXPR TStringPool(const size_type & size_count,
			const size_type & capacity_count,
			pointer _Begin, pointer _End,
			const_reference val = value_type()) THROW_NEVER : base_type(size_count, capacity_count, _Begin, _End, val)
		{
			addDelimiter();
		}
		CONSTEXPR TStringPool(const size_type & size_count,
			const size_type & capacity_count,
			pointer _Begin, pointer _End,
			const_reference val,
			const allocator_type & alloc,
			const growing_type & grow = growing_type()) THROW_NEVER : base_type(size_count, capacity_count, _Begin, _End, val, alloc, grow)
		{
			addDelimiter();
		}
		CONSTEXPRDTOR ~TStringPool() THROW_NEVER
		{
			base_type::release(false);
		}

		CONSTEXPR const allocator_type & get_allocator() const
		{
			return this->mAllocator;
		}
		CONSTEXPR allocator_type & get_allocator()
		{
			return this->mAllocator;
		}
		CONSTEXPR const growing_type & get_growing() const
		{
			return this->mGrow;
		}
		CONSTEXPR growing_type & get_growing()
		{
			return this->mGrow;
		}
		CONSTEXPR this_type & operator = (const this_type & pool)
		{
			return assign(pool);
		}
		CONSTEXPR this_type & operator = (this_type && pool) noexcept
		{
			move(CELL::TOOLS::MEMORY::TForward<this_type>(pool));
			return (*this);
		}
#ifdef USE_INITIALIZER_LIST
		CONSTEXPR this_type & operator = (std::initializer_list<TValueType> pool)
		{
			return assign(pool.begin(), pool.end());
		}
#endif
		CONSTEXPR this_type & operator = (const_pointer pool)
		{
			const size_type len = strlength(pool);
			return assign(pool, len);
		}
		CONSTEXPR this_type & operator = (const value_type & pool)
		{
			return assign(1, pool);
		}
		CONSTEXPR this_type & operator += (const this_type & pool)
		{
			if (pool.size() > 0)
			{
				append(pool.begin(), pool.end());
			}
			return (*this);
		}
		CONSTEXPR this_type & operator += (const_pointer pool)
		{
			const size_type len = strlength(pool);
			if (len > 0)
			{
				append(pool, pool + len);
			}
			return (*this);
		}
		CONSTEXPR this_type & operator += (const value_type & pool)
		{
			base_type::push_back(pool);
			addDelimiter();
			return (*this);
		}
#ifdef USE_INITIALIZER_LIST
		CONSTEXPR this_type & assign(std::initializer_list<TValueType> initList)
		{
			//this_type new_pool(initList.begin(), initList.end());
			//swap(new_pool);
			assign(initList.begin(), initList.end());
			return (*this);
		}
#endif
		CONSTEXPR this_type & assign(const this_type & pool)
		{
			if (this != &pool)
			{
				//this_type new_pool(pool);
				//swap(new_pool);
				//assign(pool.begin(), pool.end());
				base_type::assign(pool);
				//this->mpLastElement = this->mpFirstElement + len;
				addDelimiter();
			}
			return (*this);
		}
		template <typename InputIterator>
		CONSTEXPR this_type & assign(InputIterator _First, InputIterator _Last)
		{
			//this_type new_pool(_First, _Last);
			//swap(new_pool);
			const size_type len = base_type::size_distance_pointer(_First, _Last);
			if (len > 0)
			{
				base_type::assign(_First, _Last);
				//this->mpLastElement = this->mpFirstElement + len;
				addDelimiter();
			}
			return (*this);
		}
		CONSTEXPR this_type & assign(size_type len, const_reference value)
		{
			//this_type new_pool(len, value);
			//swap(new_pool);
			if (len > 0)
			{
				base_type::assign(len, value);
				//this->mpLastElement = this->mpFirstElement + len;
				addDelimiter();
			}
			return (*this);
		}
		CONSTEXPR this_type & assign(const_pointer s)
		{
			const size_type len = strlength(s);
			//this_type new_pool(s, s + len);
			//swap(new_pool);
			assign(s, s + len);
			return (*this);
		}
		CONSTEXPR this_type & assign(const_pointer s, size_type n)
		{
			//this_type new_pool(s, s + n);
			//swap(new_pool);
			assign(s, s + n);
			return (*this);
		}
		CONSTEXPR this_type & assign(const this_type & str, size_type subpos, size_type sublen)
		{
			if (this != &str)
			{
				//this_type new_pool(str.begin() + subpos, str.begin() + subpos + sublen);
				//swap(new_pool);
				assign(str.begin() + subpos, str.begin() + subpos + sublen);
			}
			return (*this);
		}
		CONSTEXPR void swap(this_type & other) noexcept
		{
			base_type::swap(other);
			addDelimiter();
			other.addDelimiter();
		}
		CONSTEXPR void move(this_type && other) noexcept
		{
			base_type::move( CELL::TOOLS::MEMORY::TForward<this_type>( other) );
			addDelimiter();
		}
		CONSTEXPR bool equals(const this_type & rhs) const THROW_NEVER
		{
			//if (this->size() != rhs.size())
			//{
			//	return false;
			//}
			//return (spcompare(begin(), rhs.begin(), this->size()) == 0);
			return (compare(rhs) == 0);
		}
		CONSTEXPR bool equals(const_pointer rhs) const THROW_NEVER
		{
			return (compare(rhs) == 0);
		}
		CONSTEXPR bool less(const this_type & rhs) const THROW_NEVER
		{
			return (compare(rhs) < 0);
		}
		CONSTEXPR bool greater(const this_type & rhs) const THROW_NEVER
		{
			return (compare(rhs) > 0);
		}
//===================c=========================================================
// String operations:
//=============================================================================
		CONSTEXPR const_pointer c_str() const
		{
			return base_type::data();
		}
		//=============================================================================
		CONSTEXPR pointer str() const
		{
			return base_type::data();
		}
		//=============================================================================
		CONSTEXPR this_type substr(size_type pos, size_type len = npos) const
		{
			if (len == npos)
			{
				len = base_type::size() - pos;
			}
			this_type pool = base_type::template sub<this_type>(pos, len);
			pool.addDelimiter();
			return pool;
		}
		//=============================================================================
		CONSTEXPR size_type copy(pointer dest, size_type len, size_type pos = 0)
		{
			if (base_type::empty())
			{
				return 0;
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			pointer iter = base_type::begin() + pos;
			pointer iter_end = iter + len;
			GLOBAL_ASSERT_RANGE_ERROR(iter, iter_end);
			if (base_type::is_from(iter) && base_type::is_from(iter_end))
			{
				//base_type::copyOut(iter, iter_end, dest);
				spcopy(dest, iter, len);
				dest[len] = delimiter;
			}
			return len;
		}
		//=============================================================================
		CONSTEXPR size_type find_first_of(const this_type & str, size_type pos = 0) const
		{
			return find_first_of(str.begin(), str.end(), pos, base_type::size() - pos);
		}
		CONSTEXPR size_type find_first_of(const_pointer s, size_type pos = 0) const
		{
			size_type sz = strlength(s);
			return Find_first_of(s, s + sz, pos, base_type::size() - pos);
		}
		CONSTEXPR size_type find_first_of(const_pointer s, size_type pos, size_type n) const
		{
			//size_type sz = strlength(s);
			return Find_first_of(s, s + n, pos, base_type::size() - pos);
		}
		CONSTEXPR size_type find_first_of(value_type c, size_type pos = 0) const
		{
			if (base_type::empty())
			{
				return (npos);
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			pointer iter = const_cast<pointer>(base_type::begin()) + pos;
			pointer iter_end = base_type::end();
			size_type str_pos = pos;
			if (iter >= iter_end)
			{
				return npos;
			}
			//const char * strchr ( const char * str, int character );
			while (iter != iter_end)
			{
				if ((*iter) == c)
				{
					return str_pos;
				}
				++iter;
				++str_pos;
			}
			return (npos);
		}
		template <class InputIterator>
		CONSTEXPR size_type find_first_of(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{	
			if (base_type::empty())
			{
				return (npos);
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			GLOBAL_ASSERT_RANGE_ERROR(_Begin, _End);
			//size_type iter_size = _Begin - _End;
			size_type iter_size = base_type::size_distance_pointer(_Begin, _End);
			pointer iter = const_cast<pointer>(base_type::begin()) + pos;
			if (len == npos)
			{
				len = base_type::size() - pos;
			}
			pointer iter_end = (const_cast<pointer>(base_type::begin()) + pos + len) - iter_size;
			size_type counter = 0;
			size_type str_pos = pos;
			if (iter >= iter_end)
			{
				return npos;
			}
			while (iter != iter_end)
			{
				counter = 0;
				for (InputIterator iter2 = _Begin; iter2 != _End; ++_Begin)
				{
					if ((*(iter + counter)) != (*iter2))
					{
						break;
					}
					else
					{
						++counter;
					}
				}
				if (counter == iter_size)
				{
					return str_pos;
				}
				++iter;
				++str_pos;
			}
			return (npos);
			//return (base_type::size() - (base_type::end() - iter));
		}
		////=============================================================================
		CONSTEXPR size_type find_last_of(const this_type & str, size_type pos = npos) const
		{
			return find_last_of(str.begin(), str.end(), pos, base_type::size());
		}
		CONSTEXPR size_type find_last_of(const_pointer s, size_type pos = npos) const
		{
			size_type sz = strlength(s);
			return find_last_of(s, s + sz, pos, base_type::size());
		}
		CONSTEXPR size_type find_last_of(const_pointer s, size_type pos, size_type n) const
		{
			//size_type sz = strlength(s);
			return find_last_of(s, s + n, pos, base_type::size());
		}
		CONSTEXPR size_type find_last_of(value_type c, size_type pos = npos) const
		{
			if (base_type::empty())
			{
				return (npos);
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			if (pos == npos)
			{
				pos = base_type::size();
			}
			size_type str_pos = pos;
			pointer iter_begin = const_cast<pointer>(base_type::begin());
			pointer iter = const_cast<pointer>(base_type::begin()) + pos;
			if (iter < iter_begin)
			{
				return npos;
			}
			//const char * strrchr ( const char * str, int character );
			while (iter != iter_begin)
			{
				if ((*iter) == c)
				{
					return str_pos;
				}
				--iter;
				--str_pos;
			}
			return (npos);
		}
		template <class InputIterator>
		CONSTEXPR size_type find_last_of(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{
			if (base_type::empty())
			{
				return (npos);
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			GLOBAL_ASSERT_RANGE_ERROR(_Begin, _End);
			//size_type iter_size = _Begin - _End;
			size_type iter_size = base_type::size_distance_pointer(_Begin, _End);
			if (pos == npos)
			{
				pos = base_type::size();
			}
			//if (len == npos)
			//{
			//	len = pos + 1;
			//}
			pointer iter_begin = const_cast<pointer>(base_type::begin()) + pos - len;
			size_type str_pos = pos - iter_size;
			pointer iter = const_cast<pointer>(base_type::begin()) + str_pos;
			size_type counter = 0;
			if (iter < iter_begin)
			{
				return npos;
			}
			while (iter != iter_begin)
			{
				counter = 0;
				for (InputIterator iter2 = _Begin; iter2 != _End; ++_Begin)
				{
					if ((*(iter + counter)) != (*iter2))
					{
						break;
					}
					else
					{
						++counter;
					}
				}
				if (counter == iter_size)
				{
					//return (str_pos + iter_size);
					return (str_pos);
				}
				--iter;
				--str_pos;
			}
			return (npos);
		}
		////=============================================================================
		CONSTEXPR size_type find_first_not_of(const this_type & str, size_type pos = 0) const
		{
			size_type fpos = find_first_of(str, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos < base_type::size() - 1)
			{
				return fpos + 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_first_not_of(const_pointer s, size_type pos = 0) const
		{
			size_type fpos = find_first_of(s, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos < base_type::size() - 1)
			{
				return fpos + 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const
		{
			size_type fpos = find_first_of(s, pos, n);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos < base_type::size() - 1)
			{
				return fpos + 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_first_not_of(value_type c, size_type pos = 0) const
		{
			size_type fpos = find_first_of(c, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos < base_type::size() - 1)
			{
				return fpos + 1;
			}
			return npos;
		}
		template <class InputIterator>
		CONSTEXPR size_type find_first_not_of(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{
			size_type fpos = find_first_of(_Begin, _End, pos, len);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos < base_type::size() - 1)
			{
				return fpos + 1;
			}
			return npos;
		}
		//=============================================================================
		CONSTEXPR size_type find_last_not_of(const this_type & str, size_type pos = npos) const
		{
			size_type fpos = find_last_of(str, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos > 0)
			{
				return fpos - 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_last_not_of(const_pointer s, size_type pos = npos) const
		{
			size_type fpos = find_last_of(s, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos > 0)
			{
				return fpos - 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_last_not_of(const_pointer c, size_type pos, size_type n) const
		{
			size_type fpos = find_last_of(c, pos, n);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos > 0)
			{
				return fpos - 1;
			}
			return npos;
		}
		CONSTEXPR size_type find_last_not_of(value_type c, size_type pos = npos) const
		{
			size_type fpos = find_last_of(c, pos);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos > 0)
			{
				return fpos - 1;
			}
			return npos;
		}
		template <class InputIterator>
		CONSTEXPR size_type find_last_not_of(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{
			size_type fpos = find_last_of(_Begin, _End,  pos, len);
			if (fpos == npos)
			{
				return npos;
			}
			if (fpos > 0)
			{
				return fpos - 1;
			}
			return npos;
		}
		//=============================================================================
		template <class TPoolType>
		CONSTEXPR int compare(const TPoolType & str) const
		{
			const_pointer iter_begin = base_type::begin();
			const_pointer iter_begin2 = str.begin();
			const size_type szr = str.size();
			const size_type szl = base_type::size();
			if (szl < szr)
			{
				return (-1);
			}
			if (szl > szr)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, szr);
		}
		CONSTEXPR int compare(const this_type & str) const
		{
			const_pointer iter_begin = base_type::begin();
			const_pointer iter_begin2 = str.begin();
			const size_type szr = str.size();
			const size_type szl = base_type::size();
			if (szl < szr)
			{
				return (-1);
			}
			if (szl > szr)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, szr);
		}
		template <class TPoolType>
		CONSTEXPR int compare(size_type pos, size_type len, const TPoolType & str) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const_pointer iter_begin2 = str.begin();
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = str.size();
			if (len < szr)
			{
				return (-1);
			}
			if (len > szr)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, len);
		}
		CONSTEXPR int compare(size_type pos, size_type len, const this_type & str) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const_pointer iter_begin2 = str.begin();
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = str.size();
			if (len < szr)
			{
				return (-1);
			}
			if (len > szr)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, len);
		}
		template <class TPoolType>
		CONSTEXPR int compare(size_type pos, size_type len, const TPoolType & str, size_type subpos, size_type sublen) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const_pointer iter_begin2 = str.begin() + subpos;
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = str.size();
			if (sublen > szr - subpos)
			{
				sublen = szr - subpos;
			}
			if (len < sublen)
			{
				return (-1);
			}
			if (len > sublen)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, len);
		}
		CONSTEXPR int compare(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const_pointer iter_begin2 = str.begin() + subpos;
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = str.size();
			if (sublen > szr - subpos)
			{
				sublen = szr - subpos;
			}
			if (len < sublen)
			{
				return (-1);
			}
			if (len > sublen)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, len);
		}
		CONSTEXPR int compare(const_pointer s) const
		{
			const_pointer iter_begin = base_type::begin();
			const size_type szl = base_type::size();
			const size_type szr = strlength(s);
			if (szl < szr)
			{
				return (-1);
			}
			if (szl > szr)
			{
				return (1);
			}
			return compare(iter_begin, s, szr);
		}
		CONSTEXPR int compare(size_type pos, size_type len, const_pointer s) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = strlength(s);
			if (len < szr)
			{
				return (-1);
			}
			if (len > szr)
			{
				return (1);
			}
			return compare(iter_begin, s, len);
		}
		CONSTEXPR int compare(size_type pos, size_type len, const_pointer s, size_type n) const
		{
			const_pointer iter_begin = base_type::begin() + pos;
			const size_type szl = base_type::size();
			if (len > szl - pos)
			{
				len = szl - pos;
			}
			const size_type szr = n;
			if (len < szr)
			{
				return (-1);
			}
			if (len > szr)
			{
				return (1);
			}
			return compare(iter_begin, s, szr);
		}
		template <class InputIterator>
		CONSTEXPR size_type compare(InputIterator _Begin, InputIterator _End) const
		{
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			GLOBAL_ASSERT_RANGE_ERROR(_Begin, _End);
			const_pointer iter_begin = base_type::begin();
			const_pointer iter_begin2 = static_cast<const_pointer>( _Begin );
			const size_type szl = base_type::size();
			const size_type szr = _End - _Begin;
			if (szl < szr)
			{
				return (-1);
			}
			if (szl > szr)
			{
				return (1);
			}
			return compare(iter_begin, iter_begin2, szr);
		}
		CONSTEXPR int compare(const_pointer str1, const_pointer str2, size_type num) const
		{
			return spcompare(str1, str2, num);
		}
		CONSTEXPR size_type length() const
		{
			return base_type::size();
		}
		CONSTEXPR size_type strlength(const_pointer str) const
		{
			//return static_cast<size_type>( ::strlen(static_cast<const char *>(str)) );
			//return static_cast<size_type>(splength(str));
			return ( splength(str) );
		}
		CONSTEXPR void strcopy(const_pointer src, size_type len, size_type pos = 0)
		{
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			if (len > 0)
			{
				if (base_type::empty() || ((base_type::size() - pos) < (len)))
				{
					base_type::resize(pos + len + 1);
				}
				
				pointer iter = base_type::begin() + pos;
				pointer iter_end = base_type::begin() + pos + len;
				spcopy(iter, src, len);

				if (base_type::ValidateIfFull())
				{
					iter_end = base_type::begin() + pos + len;
				}
				*iter_end = delimiter;
			}
		}
		//=============================================================================
		CONSTEXPR size_type find(const this_type & str, size_type pos = 0) const
		{
			return find(str.begin(), pos, str.length());
		}
		CONSTEXPR size_type find(const_pointer str, size_type pos = 0) const
		{
			return find(str, pos, strlength(str));
		}
		CONSTEXPR size_type find(const_pointer str, size_type pos, size_type n) const
		{
			if (base_type::empty())
			{
				return (npos);	// no match
			}
			pointer iter = const_cast<pointer>( base_type::begin() ) + pos;
			if (n == npos)
			{
				n = splength(str);
			}
			const_pointer iter_end = base_type::end();
			GLOBAL_ASSERT_RANGE_ERROR(iter, iter_end);
			size_type str_pos = pos;
			if (iter >= iter_end)	
			{
				return npos;
			}
			while (iter != iter_end)
			{
				if (compare(iter, str, n) == 0)
				{
					return str_pos;
				}
				++iter;
				++str_pos;
			}
			return (npos);	// no match
		}
		CONSTEXPR size_type find(value_type c, size_type pos = 0) const
		{
			return find(static_cast<const_pointer>(&c), pos, 1);
		}
		template <class InputIterator>
		CONSTEXPR size_type find(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{
			if (base_type::empty())
			{
				return (npos);	// no match
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			GLOBAL_ASSERT_RANGE_ERROR(_Begin, _End);
			//size_type iter_size = _Begin - _End;
			size_type iter_size = base_type::size_distance_pointer(_Begin, _End);
			pointer iter = const_cast<pointer>(base_type::begin()) + pos;
			if (len == npos)
			{
				len = base_type::size() - pos;
			}
			pointer iter_end = (const_cast<pointer>(base_type::begin()) + pos + len) - iter_size;
			size_type counter = 0;
			size_type str_pos = pos;
			if (iter >= iter_end)
				return npos;
			while (iter != iter_end)
			{
				if (compare(iter, _Begin, iter_size) == 0)
				{
					return str_pos;
				}
				++iter;
				++str_pos;
			}
			return (npos);
		}
		//=============================================================================
		CONSTEXPR size_type rfind(const this_type & str, size_type pos = npos) const
		{
			return rfind(str.begin(), pos, str.length());
		}
		CONSTEXPR size_type rfind(const_pointer s, size_type pos = npos) const
		{
			return rfind(s, pos, strlength(s));
		}
		CONSTEXPR size_type rfind(const_pointer s, size_type pos, size_type n) const
		{
			if (base_type::empty())
			{
				return (npos);	// no match
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			if (pos == npos)
			{
				pos = base_type::size();
			}
			size_type str_pos = pos;
			pointer iter_begin = const_cast<pointer>(base_type::begin());
			pointer iter = const_cast<pointer>(base_type::begin()) + pos;
			if (iter < iter_begin)
				return npos;
			//const char * strrchr ( const char * str, int character );
			while (iter != iter_begin)
			{
				if (compare(iter, s, n) == 0)
				{
					return str_pos;
				}
				--iter;
				--str_pos;
			}
			return (npos);
		}
		CONSTEXPR size_type rfind(value_type c, size_type pos = npos) const
		{
			return rfind(static_cast<const_pointer>(&c), pos, 1);
		}
		template <class InputIterator>
		CONSTEXPR size_type rfind(InputIterator _Begin, InputIterator _End, size_type pos, size_type len) const
		{
			if (base_type::empty())
			{
				return (npos);	// no match
			}
			GLOBAL_ASSERT_RANGE_ERROR(this->mpFirstElement, this->mpLastElement);
			GLOBAL_ASSERT_RANGE_ERROR(_Begin, _End);
			//size_type iter_size = _Begin - _End;
			size_type iter_size = base_type::size_distance_pointer(_Begin, _End);
			if (len == npos)
			{
				len = base_type::size() - pos;
			}
			pointer iter_begin = const_cast<pointer>(base_type::begin()) + pos - len;
			size_type str_pos = pos - iter_size;
			pointer iter = const_cast<pointer>(base_type::begin()) + str_pos;
			size_type counter = 0;
			if (iter < iter_begin)
			{
				return npos;
			}
			while (iter != iter_begin)
			{
				if (compare(iter, _Begin, iter_size) == 0)
				{
					return (str_pos + iter_size);
				}
				--iter;
				--str_pos;
			}
			return (npos);
			//return (base_type::size() - (base_type::end() - iter));
		}
		//=============================================================================
		CONSTEXPR this_type & append(const this_type & str)
		{
			append(str.begin(), str.end());
			return (*this);
		}
		CONSTEXPR this_type & append(const this_type & str, size_type subpos, size_type sublen)
		{
			pointer iter_begin = str.begin() + subpos;
			pointer iter_end = iter_begin + sublen;
			append(iter_begin, iter_end);
			return (*this);
		}
		CONSTEXPR this_type & append(const_pointer s)
		{
			pointer iter_begin = s;
			pointer iter_end = s + strlength(s);
			append(iter_begin, iter_end);
			return (*this);
		}
		CONSTEXPR this_type & append(const_pointer s, size_type n)
		{
			pointer iter_begin = s;
			pointer iter_end = s + n;
			append(iter_begin, iter_end);
			return (*this);
		}
		CONSTEXPR this_type & append(size_type n, value_type c)
		{
			insert(base_type::end(), n, c);
			return (*this);
		}
		template <class InputIterator>
		CONSTEXPR this_type & append(InputIterator first, InputIterator last)
		{
			insert(base_type::end(), first, last);
			return (*this);
		}
		//=============================================================================
		CONSTEXPR this_type & insert(size_type pos, const this_type & str)
		{
			if (!str.empty())
			{
				pointer iter_pos = base_type::begin() + pos;
				pointer iter_begin = str.begin();
				pointer iter_end = str.end();
				insert(iter_pos, iter_begin, iter_end);
			}
			return (*this);
		}
		CONSTEXPR this_type & insert(size_type pos, const this_type & str, size_t subpos, size_t sublen)
		{
			if (!str.empty())
			{
				pointer iter_pos = base_type::begin() + pos;
				pointer iter_begin = str.begin() + subpos;
				pointer iter_end = iter_begin + sublen;
				insert(iter_pos, iter_begin, iter_end);
			}
			return (*this);
		}
		CONSTEXPR this_type & insert(size_type pos, const_pointer s)
		{
			const size_type len = splength(s);
			if (len > 0)
			{
				pointer iter_pos = base_type::begin() + pos;
				pointer iter_begin = s;
				pointer iter_end = s + len;
				insert(iter_pos, iter_begin, iter_end);
			}
			return (*this);
		}
		CONSTEXPR this_type & insert(size_type pos, const_pointer s, size_type n)
		{
			if (n > 0)
			{
				pointer iter_pos = base_type::begin() + pos;
				pointer iter_begin = s;
				pointer iter_end = s + n;
				insert(iter_pos, iter_begin, iter_end);
			}
			return (*this);
		}
		CONSTEXPR this_type & insert(size_type pos, size_type n, value_type c)
		{
			if (n > 0)
			{
				pointer iter_pos = base_type::begin() + pos;
				insert(iter_pos, n, c);
			}
			return (*this);
		}
		//iterator insert(const_iterator p, size_type n, value_type c)
		CONSTEXPR iterator insert(iterator p, size_type n, value_type c)
		{
			if (n > 0)
			{
				pointer iter_pos = p;
				size_type len = base_type::size_distance_pointer(base_type::begin(), iter_pos);
				base_type::insert(iter_pos, n, c);
				addDelimiter();
				return (base_type::begin() + len);
			}
			else
			{
				return base_type::begin();
			}
		}
		//iterator insert(const_iterator p, value_type c)
		CONSTEXPR iterator insert(iterator p, value_type c)
		{
			pointer iter_pos = p;
			size_type len = base_type::size_distance_pointer(base_type::begin(), iter_pos);
			//size_type n = base_type::size_distance_pointer(iter_pos, base_type::mpLastElement);
			base_type::insert(iter_pos, c);
			addDelimiter();
			return (base_type::begin() + len);
		}
		template <class InputIterator>
		CONSTEXPR iterator insert(iterator p, InputIterator first, InputIterator last)
		{
			size_type n = base_type::size_distance_pointer(first, last);
			if (n > 0)
			{
				pointer iter_pos = p;
				size_type len = base_type::size_distance_pointer(base_type::begin(), iter_pos);
				base_type::insert(iter_pos, first, last);
				addDelimiter();
				return (base_type::begin() + len);
			}
			else
			{
				return base_type::begin();
			}
		}
		//=============================================================================
		CONSTEXPR this_type & replace(size_type pos, size_type len, const this_type & str)
		{
			if (!str.empty())
			{
				pointer iter_pos = base_type::begin() + pos;
				pointer iter_pos_end = iter_pos + len;
				replace(iter_pos, iter_pos_end, str.begin(), str.end());
			}
			return (*this);
		}
		CONSTEXPR this_type & replace(const_iterator i1, const_iterator i2, const this_type & str)
		{
			pointer iter_pos = i1;
			pointer iter_pos_end = i2;
			replace(iter_pos, iter_pos_end, str.begin(), str.end());
			return (*this);
		}
		CONSTEXPR this_type & replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen)
		{
			pointer iter_pos = base_type::begin() + pos;
			pointer iter_pos_end = iter_pos + len;
			pointer str_begin = str.begin() + subpos;
			pointer str_last = str + sublen;
			replace(iter_pos, iter_pos_end, str_begin, str_last);
			return (*this);
		}
		CONSTEXPR this_type & replace(size_type pos, size_type len, const_pointer s)
		{
			pointer iter_pos = base_type::begin() + pos;
			pointer iter_pos_end = iter_pos + len;
			const size_type dlen = base_type::size_distance_pointer(iter_pos, iter_pos_end);
			replace(iter_pos, iter_pos_end, (*s), dlen);
			return (*this);
		}
		CONSTEXPR this_type & replace(const_iterator i1, const_iterator i2, const_pointer s)
		{
			pointer iter_pos = i1;
			pointer iter_pos_end = i2;
			const size_type dlen = base_type::size_distance_pointer(i1, i2);
			replace(iter_pos, iter_pos_end, s, dlen);
			return (*this);
		}
		CONSTEXPR this_type & replace(size_type pos, size_type len, const_pointer s, size_type n)
		{
			pointer iter_pos = base_type::begin() + pos;
			pointer iter_pos_end = iter_pos + len;
			replace(iter_pos, iter_pos_end, s, n);
			return (*this);
		}
		CONSTEXPR this_type & replace(const_iterator i1, const_iterator i2, const_pointer s, size_type n)
		{
			const size_type len = base_type::size_distance_pointer(i1, i2);
			if (len > 0)
			{
				pointer iter_pos = i1;
				pointer iter_pos_end = i2;
				pointer str = s;
				size_type cnt = 0;
				while (iter_pos != iter_pos_end)
				{
					if (cnt > n)
					{
						break;
					}
					(*iter_pos) = (*str);
					++str;
					++iter_pos;
					++cnt;
				}
				addDelimiter();
			}
			return (*this);
		}
		CONSTEXPR this_type & replace(size_type pos, size_type len, size_type n, value_type c)
		{
			pointer iter_pos = base_type::begin() + pos;
			pointer iter_pos_end = iter_pos + len;
			replace(iter_pos, iter_pos_end, n, c);
			return (*this);
		}
		CONSTEXPR this_type & replace(const_iterator i1, const_iterator i2, size_t n, value_type c)
		{
			const size_type len = base_type::size_distance_pointer(i1, i2);
			if (len > 0)
			{
				pointer iter_pos = i1;
				pointer iter_pos_end = i2;
				size_type cnt = 0;
				while (iter_pos != iter_pos_end)
				{
					if (cnt > n)
					{
						break;
					}
					(*iter_pos) = c;
					++iter_pos;
					++cnt;
				}
				addDelimiter();
			}
			return (*this);
		}
		template <class InputIterator>
		CONSTEXPR this_type & replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)
		{
			const size_type len1 = base_type::size_distance_pointer(i1, i2);
			const size_type len2 = base_type::size_distance_pointer(first, last);
			if (len1 > 0 && len2 > 0)
			{
				pointer iter_pos = i1;
				pointer iter_pos_end = i2;
				pointer str = first;
				pointer str_last = last;
				while (iter_pos != iter_pos_end)
				{
					if (str >= str_last)
					{
						break;
					}
					(*iter_pos) = (*str);
					++str;
					++iter_pos;
				}
				addDelimiter();
			}
			return (*this);
		}
		//=============================================================================
		CONSTEXPR void addDelimiter()
		{
			if ((*this->mpLastElement) != delimiter)
			{
				base_type::ValidateIfFull();
				base_type::constructFillLast(delimiter);
				this->mpLastElement = this->mpLastElement - 1;
			}
		}
	protected:
		CONSTEXPR void initializeStringPool(const size_type & size_count)
		{
			initializeStringPool(size_count, base_type::initialize_capacity_size(size_count));
		}
		CONSTEXPR void initializeStringPool(const size_type & size_count, const size_type & capacity_count)
		{
			if (base_type::initialize(capacity_count, false))
			{
				this->mpLastElement = this->mpFirstElement + size_count;
			}
		}
	//=============================================================================
	}; // StringPool
	//=============================================================================
	//template < typename TValueType, typename TAllocType, typename TGrowStratType >
	//TValueType TStringPool<TValueType, TAllocType, TGrowStratType>::endl = '\n';
	////=============================================================================
	//template < typename TValueType, typename TAllocType, typename TGrowStratType >
	//TValueType TStringPool<TValueType, TAllocType, TGrowStratType>::delimiter = '\0';
	////=============================================================================
	//template < typename TValueType, typename TAllocType, typename TGrowStratType >
	//TValueType TStringPool<TValueType, TAllocType, TGrowStratType>::path_seperator = '\\';
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR void swap(TStringPool<TValueType, TAllocType, TGrowStratType> & lhs,
		TStringPool<TValueType, TAllocType, TGrowStratType> & rhs)
	{
		lhs.swap(rhs);
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType> &
	operator << (TStringPool<TValueType, TAllocType, TGrowStratType> & os,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & pool)
	{
		os.append(pool.begin(), pool.end());
		return os;
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType> &
	operator << (TStringPool<TValueType, TAllocType, TGrowStratType> & os,
		const TValueType * pool)
	{
		const size_t sz = splength(pool);
		os.append(pool, pool + sz);
		return os;
	}
	//=============================================================================
	//	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	//	TStringPool<TValueType, TAllocType, TGrowStratType> &
	//	operator << (TValueType * dest,
	//		const TStringPool<TValueType, TAllocType, TGrowStratType> & src)
	//{
	//	spcopy(dest, src.get_void(), src.length());
	//	return dest;
	//}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType> &
	operator << (TStringPool<TValueType, TAllocType, TGrowStratType> & os,
		const TValueType & pool)
	{
		os += pool;
		return os;
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType> &
	operator >> (TStringPool<TValueType, TAllocType, TGrowStratType> & is,
		TStringPool<TValueType, TAllocType, TGrowStratType> & pool)
	{
		pool.append(is.begin(), is.end());
		return is;
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType>
	operator + (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		TStringPool<TValueType, TAllocType, TGrowStratType> new_pool(left.begin(), left.end(), right.begin(), right.end());
		return CELL::TOOLS::MEMORY::TForward<TStringPool<TValueType, TAllocType, TGrowStratType>>(new_pool);
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType>
	operator + (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TValueType * right)
	{
		const size_t sz = splength(right);
		TStringPool<TValueType, TAllocType, TGrowStratType> new_pool(left.begin(), left.end(), right, right + sz);
		return CELL::TOOLS::MEMORY::TForward<TStringPool<TValueType, TAllocType, TGrowStratType>>(new_pool);
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType>
	operator + (const TStringPool<TValueType, TAllocType, TGrowStratType> & os,
		const TValueType & pool)
	{
		TStringPool<TValueType, TAllocType, TGrowStratType> ret(os);
		ret += pool;
		return ret;
	}
	//=============================================================================
	//	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	//TStringPool<TValueType, TAllocType, TGrowStratType>
	//operator + (const TValueType * left, const TValueType * right)
	//{
	//	size_t szleft = splength(left);
	//	size_t szright = splength(right);
	//	TStringPool<TValueType, TAllocType, TGrowStratType> new_pool(left, left + szleft, right, right + szright);
	//	return (new_pool);
	//}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR TStringPool<TValueType, TAllocType, TGrowStratType>
	operator + (const TValueType * left, const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		const size_t szleft = splength(left);
		TStringPool<TValueType, TAllocType, TGrowStratType> new_pool(left, left + szleft, right.begin(), right.end());
		return CELL::TOOLS::MEMORY::TForward<TStringPool<TValueType, TAllocType, TGrowStratType>>(new_pool);
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator == (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (left.equals(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator != (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (!left.equals(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator == (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TValueType * right)
	{
		return (left.equals(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator != (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TValueType * right)	
	{
		return (!left.equals(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator < (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (left.less(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator <= (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (!left.greater(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator > (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (left.greater(right));
	}
	//=============================================================================
	template < typename TValueType, typename TAllocType, typename TGrowStratType >
	CONSTEXPR bool operator >= (const TStringPool<TValueType, TAllocType, TGrowStratType> & left,
		const TStringPool<TValueType, TAllocType, TGrowStratType> & right)
	{
		return (!left.less(right));
	}
	//=============================================================================
	template
	<
		typename TValueType,
		size_t TBUFELEMS = 16,
		typename TAllocType = DEFAULT_BUFFER_ALLOCATOR< TValueType >
	>
	struct TSPool
	{
		using Type = TStringPool
		<
			TValueType,
			TAllocType,
			OPOOLGROW::TStringGStratPOD< typename TAllocType::size_type, TBUFELEMS >
		>;
	};
	//=============================================================================
	//using StringPool = SPOOL::TSPool< char, 16, DEFAULT_BUFFER_ALLOCATOR< char > >::Type;
	//using WStringPool = SPOOL::TSPool< wchar_t, 8, DEFAULT_BUFFER_ALLOCATOR< wchar_t > >::Type;
	using string_t = SPOOL::TSPool< char, 16, DEFAULT_BUFFER_ALLOCATOR< char > >::Type;
	using wstring_t = SPOOL::TSPool< wchar_t, 8, DEFAULT_BUFFER_ALLOCATOR< wchar_t > >::Type;
	using string_array = OPOOL::TPool< string_t >::Type;
	using wstring_array = OPOOL::TPool< wstring_t >::Type;
//=============================================================================
} // namespace SPOOL
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_POP
#endif  
//=============================================================================

#endif // __STRING_POOL_H__
