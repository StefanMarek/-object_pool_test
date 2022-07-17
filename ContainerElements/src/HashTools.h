/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __HASH_TOOLS_H__
#define __HASH_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "StringPool.h"
#include "StringPoolTools.h"

#include "MathTools.h"

#include <cstring> // strlen strcmp

namespace CELL {

namespace TOOLS {

namespace HASH { // namespace for protection against ADL

	//=============================================================================
	INLINE size_t THashIndexBytes(const unsigned char* key, const size_t sizeofKey, const size_t tableSize, const size_t seed = 0)
	{
		const size_t PRIME_NUMBER = 37;
		size_t hashVal = seed;
		unsigned char c;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			//hashVal = PRIME_NUMBER * hashVal + static_cast<TSizeType>(key[i]);
			c = key[i];
			hashVal = PRIME_NUMBER * hashVal + c;
		}
		hashVal = (hashVal % tableSize);
		return hashVal;
	}
	INLINE size_t THashBytes(const unsigned char* key, const size_t sizeofKey, const size_t seed = 0)
	{
		const size_t PRIME_NUMBER = 37;
		size_t hashVal = seed;
		unsigned char c;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			//hashVal = PRIME_NUMBER * hashVal + static_cast<TSizeType>(key[i]);
			c = key[i];
			hashVal = PRIME_NUMBER * hashVal + c;
		}
		return hashVal;
	}
	//=============================================================================
	// http://www.seg.rmit.edu.au/code/zwh-ipl/bitwisehash.c
	// #define SEED	1159241
	INLINE size_t THashIndexChars(const char* key, const size_t sizeofKey, const size_t tableSize, const size_t seed = 0)
	{
		size_t hashVal = seed;
		char c;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			c = key[i];
			hashVal ^= ((hashVal << static_cast<size_t>(5)) + c + (hashVal >> static_cast<size_t>(2)));
		}
		hashVal = static_cast<size_t>((hashVal & 0x7fffffff) % tableSize);
		return hashVal;
	}
	INLINE size_t THashChars(const char* key, const size_t sizeofKey, const size_t seed = 0)
	{
		size_t hashVal = seed;
		char c;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			c = key[i];
			hashVal ^= ((hashVal << static_cast<size_t>(5)) + c + (hashVal >> static_cast<size_t>(2)));
		}
		hashVal = static_cast<size_t>(hashVal & 0x7fffffff);
		return hashVal;
	}
	//=============================================================================
	// Jenkins hash function
	// https://en.wikibooks.org/wiki/Data_Structures/Hash_Tables#Choosing_a_good_hash_function
	// https://en.wikipedia.org/wiki/Jenkins_hash_function
	INLINE size_t jenkins_one_at_a_time_hash_index(const char* key, const size_t sizeofKey, const size_t tableSize, const size_t seed = 0)
	{
		size_t hashVal = 0;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			hashVal += key[i];
			hashVal += (hashVal << static_cast<size_t>(10));
			hashVal ^= (hashVal >> static_cast<size_t>(6));
		}
		hashVal += (hashVal << static_cast<size_t>(3));
		hashVal ^= (hashVal >> static_cast<size_t>(11));
		hashVal += (hashVal << static_cast<size_t>(15));

		//hashVal = (hashVal & (tableSize - 1));
		hashVal = (hashVal % tableSize);
		return hashVal;
	}
	INLINE size_t jenkins_one_at_a_time_hash(const char* key, const size_t sizeofKey, const size_t seed = 0)
	{
		size_t hashVal = 0;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			hashVal += key[i];
			hashVal += (hashVal << static_cast<size_t>(10));
			hashVal ^= (hashVal >> static_cast<size_t>(6));
		}
		hashVal += (hashVal << static_cast<size_t>(3));
		hashVal ^= (hashVal >> static_cast<size_t>(11));
		hashVal += (hashVal << static_cast<size_t>(15));

		return hashVal;
	}
	//=============================================================================
	// https://en.wikibooks.org/wiki/Algorithm_Implementation/Hashing
	INLINE size_t jenkins_one_at_a_time_hash_index(const unsigned char* key, const size_t sizeofKey, const size_t tableSize, const size_t seed = 0)
	{
		size_t hashVal = 0;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			hashVal += (key[i] & 0xFF);
			hashVal += (hashVal << static_cast<size_t>(10));
			hashVal ^= (hashVal >> static_cast<size_t>(6));
		}
		hashVal += (hashVal << static_cast<size_t>(3));
		hashVal ^= (hashVal >> static_cast<size_t>(11));
		hashVal += (hashVal << static_cast<size_t>(15));

		hashVal = (hashVal % tableSize);
		//hashVal = (hashVal & (tableSize - 1));
		return hashVal;
	}
	// https://en.wikibooks.org/wiki/Algorithm_Implementation/Hashing
	INLINE size_t jenkins_one_at_a_time_hash(const unsigned char* key, const size_t sizeofKey, const size_t seed = 0)
	{
		size_t hashVal = 0;
		for (size_t i = 0; i < sizeofKey; ++i)
		{
			hashVal += (key[i] & 0xFF);
			hashVal += (hashVal << static_cast<size_t>(10));
			hashVal ^= (hashVal >> static_cast<size_t>(6));
		}
		hashVal += (hashVal << static_cast<size_t>(3));
		hashVal ^= (hashVal >> static_cast<size_t>(11));
		hashVal += (hashVal << static_cast<size_t>(15));

		return hashVal;
	}
	//=============================================================================
	INLINE size_t STL_Hash_Index_seq(const char* _First, size_t _Count, const size_t tableSize, const size_t seed = 0)
	{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
		const size_t _FNV_offset_basis = 14695981039346656037ULL;
		const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < _Count; ++_Next)
		{	// fold in another byte
			_Val ^= static_cast<size_t>(_First[_Next]);
			_Val *= _FNV_prime;
		}
		_Val = (_Val & (tableSize - 1));
		return (_Val);
	}
	//=============================================================================
	INLINE size_t STL_Hash_Index_seq(const unsigned char* _First, size_t _Count, const size_t tableSize, const size_t seed = 0)
	{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
		const size_t _FNV_offset_basis = 14695981039346656037ULL;
		const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < _Count; ++_Next)
		{	// fold in another byte
			_Val ^= static_cast<size_t>(_First[_Next]);
			_Val *= _FNV_prime;
		}
		_Val = (_Val & (tableSize - 1));
		return (_Val);
	}
	//=============================================================================
	INLINE size_t STL_Hash_seq(const char* _First, size_t _Count, const size_t seed = 0)
	{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
		const size_t _FNV_offset_basis = 14695981039346656037ULL;
		const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < _Count; ++_Next)
		{	// fold in another byte
			_Val ^= static_cast<size_t>(_First[_Next]);
			_Val *= _FNV_prime;
		}
		return (_Val);
	}
	//=============================================================================
	INLINE size_t STL_Hash_seq(const unsigned char* _First, size_t _Count, const size_t seed = 0)
	{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
		const size_t _FNV_offset_basis = 14695981039346656037ULL;
		const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
		GLOBAL_STATIC_ASSERT(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < _Count; ++_Next)
		{	// fold in another byte
			_Val ^= static_cast<size_t>(_First[_Next]);
			_Val *= _FNV_prime;
		}
		return (_Val);
	}
	//=============================================================================
	//template <typename TSizeType>
	//INLINE TSizeType STL_Hash(const char *_First, TSizeType _Count, const TSizeType tableSize, const TSizeType seed = 0)
	//{	
	//	std::string str(_First);
	//	std::hash<std::string> hash_fn;
	//	TSizeType _Val = hash_fn(str);
	//	_Val = (_Val & (tableSize - 1));
	//	return (_Val);
	//}
	//=============================================================================
	//=============================================================================
	//=============================================================================
	struct THashPoolThreshFunc
	{
		typedef size_t size_type;
		typedef size_t return_type;
				
		size_t operator()(const size_t _size) const THROW_NEVER
		{
			//const float threshold = 0.7f;
			//const float load = static_cast<float>(_size);
			//const size_t maxLoad = static_cast<size_t>(load * threshold);

			const size_t maxLoad = (_size >> static_cast<size_t>(1)) + (_size >> static_cast<size_t>(2)); // 3/4 of true size
			return maxLoad;
		}
	};
	//template < typename TSizeType >
	//struct THashTableThreshFunc
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;

	//	enum BUCKETS : TSizeType
	//	{
	//		TMAX_BUCKETS = 1
	//	};
	//	enum LOAD : TSizeType
	//	{
	//		TMAX_LOAD = 1
	//	};
	//	TSizeType operator()(const TSizeType _size) const THROW_NEVER
	//	{
	//		const float threshold = 0.7f;
	//		const float load = static_cast<float>(_size);
	//		const size_type maxLoad = static_cast<size_type>(load * threshold);
	//		return maxLoad;
	//	}
	//};
	//template < typename TSizeType >
	//struct THashMapThreshFunc
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;

	//	enum BUCKETS : TSizeType
	//	{
	//		TMAX_BUCKETS = 4
	//	};
	//	enum LOAD : TSizeType
	//	{
	//		TMAX_LOAD = 2
	//	};
	//	TSizeType operator()(const TSizeType _size) const THROW_NEVER
	//	{
	//		const float threshold = 0.7f;
	//		const float load = static_cast<float>(_size * BUCKETS::TMAX_BUCKETS);
	//		const size_type maxLoad = static_cast<size_type>(load * threshold);
	//		return maxLoad;
	//	}
	//};
	//=============================================================================
	//=============================================================================
	//=============================================================================
	//template < typename TSizeType >
	//struct TLinearHashProbeFunc
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;

	//	TSizeType getMaxProbing(const TSizeType tableSize) const THROW_NEVER
	//	{
	//		return (tableSize);
	//	}
	//	//index = (key + i) % size; //linear probing
	//	TSizeType operator()(const TSizeType key, const TSizeType index, const TSizeType iteration, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const TSizeType indexNext = (key + iteration);
	//		const TSizeType indexNext = (key + iteration) % tableSize;
	//		return indexNext;
	//	}
	//};
	//=============================================================================
	//https://en.wikipedia.org/wiki/Quadratic_probing
	//template < typename TSizeType >
	//struct TQuadHashProbeFunc
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;
	//			
	//	TSizeType getMaxProbing(const TSizeType tableSize) const THROW_NEVER
	//	{
	//		TSizeType index = 4;
	//		for (index = 4; ((index * index) < tableSize); ++index)
	//		{
	//		}
	//		return (index);
	//	}
	//	//index = (key + i*i) % size; //quadratic probing
	//	TSizeType operator()(const TSizeType key, const TSizeType index, const TSizeType iteration, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const TSizeType indexNext = (key + iteration * iteration);
	//		const TSizeType indexNext = (key + iteration * iteration) % tableSize;
	//		//const TSizeType indexNext = (index + (2 * iteration - 1)) % tableSize;
	//		return indexNext;
	//	}
	//};
	//=============================================================================
	//template < typename TSizeType >
	//struct TDoubleHashProbeFunc
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;

	//	TSizeType getMaxProbing(const TSizeType tableSize) const THROW_NEVER
	//	{
	//		TSizeType index = 4;
	//		for (index = 4; ((index * index) < tableSize); ++index)
	//		{
	//		}
	//		return (index);
	//	}
	//	//index = (key + i*last_key) % size; //quadratic probing
	//	TSizeType operator()(const TSizeType key, const TSizeType index, const TSizeType iteration, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const TSizeType indexNext = (key + iteration * index);
	//		const TSizeType indexNext = (key + iteration * index) % tableSize;
	//		return indexNext;
	//	}
	//};
	//=============================================================================
	// another would be  index = (int)((index + (float)iteration/2 + (float) (iteration * iteration)) % tableSize);
	//http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_hashtable.aspx
	//template < typename TSizeType >
	//struct TQuadHashProbeFuncJSW
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;

	//	TSizeType getMaxProbing(const TSizeType tableSize) const THROW_NEVER
	//	{
	//		TSizeType index = 4;
	//		for (index = 4; (((index * index - index) / 2) < tableSize); ++index)
	//		{
	//		}
	//		return (index);
	//	}
	//	// h = (h + (step * step - step) / 2) % table->size;
	//	TSizeType operator()(const TSizeType key, const TSizeType index, const TSizeType iteration, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const TSizeType indexNext = (index + (iteration * iteration - iteration) / 2);
	//		const TSizeType indexNext = (index + (iteration * iteration - iteration) / 2) % tableSize;
	//		return indexNext;
	//	}
	//};
	//=============================================================================
	//template <typename TSizeType, typename TArgumentType>
	//struct THashIndexFunc 
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;
	//	typedef TArgumentType argument_type;
	//	TSizeType operator()(TArgumentType const & value, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(&value));
	//		const TSizeType sizeofKey = sizeof(argument_type);
	//		//return THashIndexBytes(bytes, sizeofKey, tableSize);
	//		return STL_Hash_Index_seq(value, sizeofKey, tableSize);
	//	}
	//};
	//template <typename TSizeType, typename TArgumentType>
	//struct THashIndexFunc< TSizeType, TArgumentType * >
	//{
	//	typedef TSizeType size_type;
	//	typedef TSizeType return_type;
	//	typedef TArgumentType argument_type;
	//	TSizeType operator()(TArgumentType * const & value, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value));
	//		const TSizeType sizeofKey = sizeof(argument_type);
	//		//return THashIndexBytes(bytes, sizeofKey, tableSize);
	//		return STL_Hash_Index_seq(value, sizeofKey, tableSize);
	//	}
	//};
	//template <typename TSizeType>
	//struct THashIndexFunc< TSizeType, char * >
	//{
	//	typedef TSizeType size_type;
	//	typedef char * argument_type;
	//	typedef TSizeType return_type;
	//	TSizeType operator()(const char * value, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value));
	//		const TSizeType sizeofKey = strlen(value);
	//		return STL_Hash_Index_seq(value, sizeofKey, tableSize);
	//	}
	//};
	//template <typename TSizeType>
	//struct THashIndexFunc< TSizeType, CELL::SPOOL::string_t >
	//{
	//	typedef TSizeType size_type;
	//	typedef CELL::SPOOL::string_t argument_type;
	//	typedef TSizeType return_type;
	//	TSizeType operator()(const CELL::SPOOL::string_t & value, const TSizeType tableSize) const THROW_NEVER
	//	{
	//		//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value.c_str()));
	//		const TSizeType sizeofKey = value.size();
	//		return STL_Hash_Index_seq(value.c_str(), sizeofKey, tableSize);
	//	}
	//};
	//template <typename TSizeType>
	//struct THashIndexFunc< TSizeType, std::string >
	//{
	//	typedef TSizeType size_type;
	//	typedef CELL::SPOOL::string_t argument_type;
	//	typedef TSizeType return_type;
	//	TSizeType operator()(const std::string & value, const TSizeType tableSize) const
	//	{
	//		//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value.c_str()));
	//		const TSizeType sizeofKey = value.size();
	//		return STL_Hash_Index_seq(value.c_str(), sizeofKey, tableSize);
	//	}
	//};
	//=============================================================================
	template <typename TArgumentType>
	struct THashFunc
	{
		typedef size_t size_type;
		typedef size_t return_type;
		typedef TArgumentType argument_type;
		//std::hash<TArgumentType> myHash;

		size_t operator()(size_t const & hash, size_t const & size) const THROW_NEVER
		{
			return (hash & (size - static_cast<size_t>(1)));
		}
		size_t operator()(TArgumentType const & value) const THROW_NEVER
		{
			const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(&value));
			const size_t sizeofKey = sizeof(argument_type);
			return STL_Hash_seq(bytes, sizeofKey);

			//return myHash(value);
		}
	};
	template <typename TArgumentType>
	struct THashFunc< TArgumentType * >
	{
		typedef size_t size_type;
		typedef size_t return_type;
		typedef TArgumentType argument_type;
		//std::hash<TArgumentType> myHash;

		size_t operator()(size_t const & hash, size_t const & size) const THROW_NEVER
		{
			return (hash & (size - static_cast<size_t>(1)));
		}
		size_t operator()(TArgumentType * const & value) const THROW_NEVER
		{
			const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value));
			const size_t sizeofKey = sizeof(argument_type);
			return STL_Hash_seq(bytes, sizeofKey);

			//return myHash(value);
		}
	};
	template <>
	struct THashFunc< char * >
	{
		typedef size_t size_type;
		typedef char * argument_type;
		typedef size_t return_type;

		size_t operator()(size_t const & hash, size_t const & size) const THROW_NEVER
		{
			return (hash & (size - static_cast<size_t>(1)));
		}
		size_t operator()(const char * value) const THROW_NEVER
		{
			//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value));
			const size_t sizeofKey = strlen(value);
			return STL_Hash_seq(value, sizeofKey);
		}
	};
	template <>
	struct THashFunc< CELL::SPOOL::string_t >
	{
		typedef size_t size_type;
		typedef CELL::SPOOL::string_t argument_type;
		typedef size_t return_type;

		size_t operator()(size_t const & hash, size_t const & size) const THROW_NEVER
		{
			return (hash & (size - static_cast<size_t>(1)));
		}
		size_t operator()(const CELL::SPOOL::string_t & value) const THROW_NEVER
		{
			//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value.c_str()));
			const size_t sizeofKey = value.size();
			return STL_Hash_seq(value.c_str(), sizeofKey);
		}
	};
	//=============================================================================
	template <typename TArgumentType>
	struct THashGrid3DFunc
	{
		typedef uint64_t size_type;
		typedef uint64_t return_type;
		typedef TArgumentType argument_type;

		uint64_t Dilate(uint64_t x) const THROW_NEVER
		{
			x = (x | (x << 32)) & 0x7fff00000000ffff;
			x = (x | (x << 16)) & 0x00ff0000ff0000ff;
			x = (x | (x << 8)) & 0x700f00f00f00f00f;
			x = (x | (x << 4)) & 0x30c30c30c30c30c3;
			x = (x | (x << 2)) & 0x1249249249249249;
			return x;
		}
		uint64_t Contract(uint64_t x) const THROW_NEVER
		{
			x = (x) & 0x1249249249249249;
			x = (x | (x >> 2)) & 0x30c30c30c30c30c3;
			x = (x | (x >> 4)) & 0x700f00f00f00f00f;
			x = (x | (x >> 8)) & 0x00ff0000ff0000ff;
			x = (x | (x >> 16)) & 0x7fff00000000ffff;
			x = (x | (x >> 32)) & 0x00000000ffffffff;
			return x;
		}
		uint64_t Encode(uint64_t x, uint64_t y, uint64_t z) const THROW_NEVER
		{
			return Dilate(x) | (Dilate(y) << 1) | (Dilate(z) << 2);
		}
		void Decode(uint64_t code, uint64_t& x, uint64_t& y, uint64_t& z) const THROW_NEVER
		{
			x = Contract(code);
			y = Contract(code >> 1);
			z = Contract(code >> 2);
		}
		uint64_t operator()(uint64_t const& hash, uint64_t const& size) const THROW_NEVER
		{
			return (hash & (size - 1));
		}
		uint64_t operator()(TArgumentType const& key) const THROW_NEVER
		{
			const uint64_t value = Encode(key.x, key.y, key.z);
			const unsigned char* bytes = static_cast<const unsigned char*>(static_cast<const void*>(&value));
			const uint64_t sizeofKey = sizeof(uint64_t);
			return STL_Hash_seq(bytes, sizeofKey);
		}
		uint64_t operator()(uint64_t code, TArgumentType & key) const THROW_NEVER
		{
			Decode(code, key.x, key.y, key.z);
			return code;
		}
	};
	//=============================================================================
	template <typename TArgumentType>
	struct THashGrid2DFunc
	{
		typedef uint64_t size_type;
		typedef uint64_t return_type;
		typedef TArgumentType argument_type;

		uint64_t Dilate(uint64_t x) const THROW_NEVER
		{
			x = (x | (x << 32)) & 0x7fff00000000ffff;
			x = (x | (x << 16)) & 0x00ff0000ff0000ff;
			x = (x | (x << 8)) & 0x700f00f00f00f00f;
			x = (x | (x << 4)) & 0x30c30c30c30c30c3;
			x = (x | (x << 2)) & 0x1249249249249249;
			return x;
		}
		uint64_t Contract(uint64_t x) const THROW_NEVER
		{
			x = (x) & 0x1249249249249249;
			x = (x | (x >> 2)) & 0x30c30c30c30c30c3;
			x = (x | (x >> 4)) & 0x700f00f00f00f00f;
			x = (x | (x >> 8)) & 0x00ff0000ff0000ff;
			x = (x | (x >> 16)) & 0x7fff00000000ffff;
			x = (x | (x >> 32)) & 0x00000000ffffffff;
			return x;
		}
		uint64_t Encode(uint64_t x, uint64_t y) const THROW_NEVER
		{
			return Dilate(x) | (Dilate(y) << 1);
		}
		void Decode(uint64_t code, uint64_t& x, uint64_t& y) const THROW_NEVER
		{
			x = Contract(code);
			y = Contract(code >> 1);
		}
		uint64_t operator()(uint64_t const& hash, uint64_t const& size) const THROW_NEVER
		{
			return (hash & (size - 1));
		}
		uint64_t operator()(TArgumentType const& key) const THROW_NEVER
		{
			const uint64_t value = Encode(key.x, key.y);
			const unsigned char* bytes = static_cast<const unsigned char*>(static_cast<const void*>(&value));
			const uint64_t sizeofKey = sizeof(uint64_t);
			return STL_Hash_seq(bytes, sizeofKey);
		}
		uint64_t operator()(uint64_t code, TArgumentType& key) const THROW_NEVER
		{
			Decode(code, key.x, key.y);
			return code;
		}
	};
//=============================================================================
} // HASH
//=============================================================================
} // namespace TOOLS
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __HASH_TOOLS_H__ 
