/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __COMPARE_TOOLS_H__
#define __COMPARE_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include <cstring> // strcmp

//=============================================================================
namespace COMPTOOLS { 

//=============================================================================
//============================ COMPARE SPECIFIC ===============================
//=============================================================================
	template <typename TArgumentType> 
	struct TCompare 
	{
		typedef TArgumentType argument_type;
		typedef int return_type;
		int operator () ( TArgumentType const & t1, TArgumentType const & t2 ) THROW_NEVER
		{
			return ( (t1 < t2) ? -1 : (t1 == t2) ? 0 : 1 );
		}
	};
	template <typename TArgumentType> 
	struct TCompare<TArgumentType *>
	{
		typedef TArgumentType argument_type;
		typedef int return_type;
		int operator () ( TArgumentType * const & t1, TArgumentType * const & t2 ) THROW_NEVER
		{
			return ( ( (*t1) < (*t2) ) ? -1 : ( (*t1) == (*t2) ) ? 0 : 1 );
		}
	};
//=============================================================================
//============================ COMPARE FUNCTIONAL =============================
//=============================================================================
	template <typename TArgumentType>
	struct TCompareLess 
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()(TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a < b);
		}
	};
	template <typename TArgumentType>
	struct TCompareLess< TArgumentType * > 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType * const & a, TArgumentType * const & b) const THROW_NEVER
		{
			return ( (*a) < (*b) );
		}
	};
	template<>
	struct TCompareLess<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(const char * a, const char * b) const THROW_NEVER
		{
			return ( strcmp(a, b) < 0 );
		}
	};
	//template<>
	//struct TCompareLess<unsigned char *> 
	//{
	//	typedef unsigned char * argument_type;
	//	typedef bool return_type;
	//	bool operator()(const unsigned char * a, const unsigned char * b) const
	//	{
	//		return ( ::memcmp(a, b) < 0 );
	//	}
	//};
	template <typename TArgumentType>
	struct TCompareLessKD
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()(TArgumentType const & a, TArgumentType const & b, unsigned int const & i) const THROW_NEVER
		{
			return (a[i] <= b[i]);
		}
	};
//=============================================================================
	template <typename TArgumentType>
	struct TCompareLessEqual 
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a <= b);
		}
	};
	template <typename TArgumentType>
	struct TCompareLessEqual<TArgumentType *> 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType * const & a, TArgumentType * const & b) const THROW_NEVER
		{
			return ( (*a) <= (*b) );
		}
	};
	template<>
	struct TCompareLessEqual<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(const char * a, const char * b) const THROW_NEVER
		{
			return ( strcmp(a, b) <= 0 );
		}
	};
	template <typename TArgumentType>
	struct TCompareLessEqualKD
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()(TArgumentType const & a, TArgumentType const & b, unsigned int const & i) const THROW_NEVER
		{
			return (a[i] <= b[i]);
		}
	};
//=============================================================================
	template <typename TArgumentType>
	struct TCompareEqual 
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a == b);
		}
	};
	template <typename TArgumentType>
	struct TCompareEqual<TArgumentType *> 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType * const & a, TArgumentType * const & b) const THROW_NEVER
		{
			return ( (*a) == (*b) );
		}
	};
	template<>
	struct TCompareEqual<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(const char * a, const char * b) const THROW_NEVER
		{
			return ( strcmp(a, b) == 0 );
		}
	};
//=============================================================================
	template <typename TArgumentType>
	struct TCompareNotEqual 
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a != b);
		}
	};
	template <typename TArgumentType>
	struct TCompareNotEqual<TArgumentType *> 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType * const & a, TArgumentType * const & b) const THROW_NEVER
		{
			return ( (*a) != (*b) );
		}
	};
	template<>
	struct TCompareNotEqual<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(const char * a, const char * b) const THROW_NEVER
		{
			return ( strcmp(a, b) != 0 );
		}
	};
//=============================================================================
	template <typename TArgumentType>
	struct TCompareGreater
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()(TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a > b);
		}
	};
	template <typename TArgumentType>
	struct TCompareGreater<TArgumentType *> 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()(TArgumentType * const & a, TArgumentType * const & b) const THROW_NEVER
		{
			return ( (*a) > (*b) );
		}
	};
	template<>
	struct TCompareGreater<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(char const * a, char const * b) const THROW_NEVER
		{
			return ( strcmp(a, b) > 0 );
		}
	};
//=============================================================================
	template <typename TArgumentType>
	struct TCompareGreaterEqual 
	{
		typedef TArgumentType argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType const & a, TArgumentType const & b) const THROW_NEVER
		{
			return (a >= b);
		}
	};
	template <typename TArgumentType>
	struct TCompareGreaterEqual<TArgumentType *> 
	{
		typedef TArgumentType * argument_type;
		typedef bool return_type;
		bool operator()( TArgumentType * const & a, TArgumentType * & b) const THROW_NEVER
		{
			return ( (*a) >= (*b) );
		}
	};
	template<>
	struct TCompareGreaterEqual<char *> 
	{
		typedef char * argument_type;
		typedef bool return_type;
		bool operator()(const char * a, const char * b) const THROW_NEVER
		{
			return ( strcmp(a, b) >= 0 );
		}
	};
//=============================================================================
//========================== ARRAY - INDEX COMPARE ============================
//=============================================================================
	template <typename TArrayType, typename TIndexType>
	struct TCompareEqualIndex 
	{
		typedef TArrayType array_type;
		typedef TIndexType index_type;
		typedef bool return_type;
		TArrayType * mpElements;

		TCompareEqualIndex( TArrayType * pElements ) THROW_NEVER : mpElements( pElements )
		{
		}
		TArrayType * get() const THROW_NEVER
		{
			return mpElements;
		}
		void set( TArrayType * pElements ) THROW_NEVER
		{
			mpElements = pElements;
		}
		bool operator()(const TIndexType & a, const TIndexType & b) const THROW_NEVER
		{
			return( ( nullptr != mpElements ) ? 
				( mpElements[a] == mpElements[b] ) : false );
		}
	};
//=============================================================================
	template <typename TArrayType, typename TIndexType>
	struct TCompareLessIndex 
	{
		typedef TArrayType array_type;
		typedef TIndexType index_type;
		typedef bool return_type;
		TArrayType * mpElements;

		TCompareLessIndex( TArrayType * pElements ) THROW_NEVER : mpElements( pElements )
		{
		}
		TArrayType * get() const THROW_NEVER
		{
			return mpElements;
		}
		void set( TArrayType * pElements ) THROW_NEVER
		{
			mpElements = pElements;
		}
		bool operator()(const TIndexType & a, const TIndexType & b) const THROW_NEVER
		{
			return( ( nullptr != mpElements ) ? 
				( mpElements[a] < mpElements[b] ) : false );
		}
	};
//=============================================================================
	template <typename TArrayType, typename TIndexType>
	struct TCompareLessEqualIndex 
	{
		typedef TArrayType array_type;
		typedef TIndexType index_type;
		typedef bool return_type;
		TArrayType * mpElements;

		TCompareLessEqualIndex( TArrayType * pElements ) THROW_NEVER : mpElements( pElements )
		{
		}
		TArrayType * get() const THROW_NEVER
		{
			return mpElements;
		}
		void set( TArrayType * pElements ) THROW_NEVER
		{
			mpElements = pElements;
		}
		bool operator()(const TIndexType & a, const TIndexType & b) const THROW_NEVER
		{
			return( ( nullptr != mpElements ) ? 
				( mpElements[a] <= mpElements[b] ) : false );
		}
	};
//=============================================================================
	template <typename TArrayType, typename TIndexType>
	struct TCompareGreaterIndex 
	{
		typedef TArrayType array_type;
		typedef TIndexType index_type;
		typedef bool return_type;
		TArrayType * mpElements;

		TCompareGreaterIndex( TArrayType * pElements ) THROW_NEVER : mpElements( pElements )
		{
		}
		TArrayType * get() const THROW_NEVER
		{
			return mpElements;
		}
		void set( TArrayType * pElements ) THROW_NEVER
		{
			mpElements = pElements;
		}
		bool operator()(const TIndexType & a, const TIndexType & b) const THROW_NEVER
		{
			return( ( nullptr != mpElements ) ? 
				( mpElements[a] > mpElements[b] ) : false );
		}
	};
//=============================================================================
	template <typename TArrayType, typename TIndexType>
	struct TCompareGreaterEqualIndex 
	{
		typedef TArrayType array_type;
		typedef TIndexType index_type;
		typedef bool return_type;
		TArrayType * mpElements;

		TCompareGreaterEqualIndex( TArrayType * pElements ) THROW_NEVER : mpElements( pElements )
		{
		}
		TArrayType * get() const THROW_NEVER
		{
			return mpElements;
		}
		void set( TArrayType * pElements ) THROW_NEVER
		{
			mpElements = pElements;
		}
		bool operator()(const TIndexType & a, const TIndexType & b) const THROW_NEVER
		{
			return( ( nullptr != mpElements ) ? 
				( mpElements[a] >= mpElements[b] ) : false );
		}
	};
	//=============================================================================
	//============================= HASH COMPARE ==================================
	//=============================================================================
	template <typename TNodeType, typename TKeyType, typename THashType>
	struct THPoolKeyComp
	{
		//const TKeyType & getKey(TNodeType const & obj) const
		//{
		//	return (obj.mKey);
		//}
		//const THashType & getHash(TNodeType const & obj) const
		//{
		//	return (obj.mHash);
		//}
		bool operator()(TNodeType const & obj, TKeyType const & key) const THROW_NEVER
		{
			return (obj.mKey == key);
		}
		bool operator()(TNodeType const & obj, TKeyType const & key, THashType const & hash) const THROW_NEVER
		{
			return ((obj.mHash == hash) && (obj.mKey == key));
		}
	};
	template <typename TNodeType, typename TKeyType, typename THashType>
	struct THPoolKeyComp<TNodeType *, TKeyType, THashType>
	{
		//const TKeyType & getKey(TNodeType * const & obj) const
		//{
		//	return ((*obj).mKey);
		//}
		//const THashType & getHash(TNodeType * const & obj) const
		//{
		//	return ((*obj).mHash);
		//}
		bool operator()(TNodeType * const & obj, TKeyType const & key) const THROW_NEVER
		{
			return ((*obj).mKey == key);
		}
		bool operator()(TNodeType * const & obj, TKeyType const & key, THashType const & hash) const THROW_NEVER
		{
			return (((*obj).mHash == hash) && ((*obj).mKey == key));
		}
	};
	//=============================================================================
	//============================= LIST COMPARE ==================================
	//=============================================================================
	template <typename TNodeType, typename TValueType>
	struct TListValueComp
	{
		bool operator()(TNodeType const & a, TValueType const & b) const THROW_NEVER
		{
			return (a.mValue == b);
		}
	};
	template <typename TNodeType, typename TValueType>
	struct TListValueComp<TNodeType *, TValueType>
	{
		bool operator()(TNodeType * const & a, TValueType const & b) const THROW_NEVER
		{
			return ((*a).mValue == b);
		}
	};
	//=============================================================================
	//template <typename TNodeType, typename TValueType>
	//struct TListValueKeyComp
	//{
	//	bool operator()(TNodeType const & a, TValueType const & b) const
	//	{
	//		return (a.mValue.mKey == b.mKey);
	//	}
	//};
	//template <typename TNodeType, typename TValueType>
	//struct TListValueKeyComp<TNodeType *, TValueType>
	//{
	//	bool operator()(TNodeType * const & a, TValueType const & b) const
	//	{
	//		return ((*a).mValue.mKey == b.mKey);
	//	}
	//};
	//template <typename TNodeType, typename TValueType>
	//struct TListValueKeyComp<TNodeType *, TValueType *>
	//{
	//	bool operator()(TNodeType * const & a, TValueType * const & b) const
	//	{
	//		return ((*a).mValue->mKey == b->mKey);
	//	}
	//};
	//template <typename TNodeType, typename TValueType>
	//struct TListValueKeyComp<TNodeType, TValueType *>
	//{
	//	bool operator()(TNodeType const & a, TValueType * const & b) const
	//	{
	//		return (a.mValue->mKey == b->mKey);
	//	}
	//};
	//=============================================================================
	template <typename TNodeType, typename TValueType>
	struct TListKeyComp
	{
		bool operator()(TNodeType const & a, TValueType const & b) const THROW_NEVER
		{
			return (a.mKey == b);
		}
	};
	template <typename TNodeType, typename TValueType>
	struct TListKeyComp<TNodeType *, TValueType>
	{
		bool operator()(TNodeType * const & a, TValueType const & b) const THROW_NEVER
		{
			return ((*a).mKey == b);
		}
	};
	//=============================================================================
	template <typename TNodeType, typename TValueType, typename THashType>
	struct TListHashKeyComp
	{
		bool operator()(TNodeType const & a, TValueType const & b, THashType const & bhash) const THROW_NEVER
		{
			//return ((a.getHash() == bhash) && (a.getKey() == b));
			return (a.compare(b, bhash));
		}
	};
	template <typename TNodeType, typename TValueType, typename THashType>
	struct TListHashKeyComp<TNodeType *, TValueType, THashType>
	{
		bool operator()(TNodeType * const & a, TValueType const & b, THashType const & bhash) const THROW_NEVER
		{
			//return (((*a).getHash() == bhash) && ((*a).getKey() == b));
			return (a->compare(b, bhash));
		}
	};
	//=============================================================================
	//template <typename TNodeType, typename TValueType>
	//struct TTreeKeyComp
	//{
	//	bool compareKeys(pointer & n1, pointer & n2) const
	//	{
	//		return mCompare(n1->getKey(), n2->getKey());
	//	}
	//	bool compareKeys(pointer & n1, key_const_reference n2) const
	//	{
	//		return mCompare(n1->getKey(), n2);
	//	}
	//	bool compareKeys(key_const_reference n1, pointer & n2) const
	//	{
	//		return mCompare(n1, n2->getKey());
	//	}
	//	bool compareKeys(key_const_reference n1, key_const_reference n2) const
	//	{
	//		return mCompare(n1, n2);
	//	}
	//	bool operator()(TNodeType const & a, TValueType const & b) const
	//	{
	//		return (a.mKey == b);
	//	}
	//};
	//template <typename TNodeType, typename TValueType>
	//struct TTreeKeyComp<TNodeType *, TValueType>
	//{
	//	bool operator()(TNodeType * const & a, TValueType const & b) const
	//	{
	//		return ((*a).mKey == b);
	//	}
	//};
	//=============================================================================
	//template <typename TNodeType, typename TValueType>
	//struct TListCompPair
	//{
	//	bool operator()(TNodeType const & a, TValueType const & b) const
	//	{
	//		return (a.mValue.first == b.first);
	//	}
	//};
	//template <typename TNodeType, typename TValueType>
	//struct TListCompPair<TNodeType *, TValueType>
	//{
	//	bool operator()(TNodeType * const & a, TValueType const & b) const
	//	{
	//		return ((*a).mValue.first == b.first);
	//	}
	//};
//=============================================================================
} // namespace COMPTOOLS
//=============================================================================

#endif // __COMPARE_TOOLS_H__ 
