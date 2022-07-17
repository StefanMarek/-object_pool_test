/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __HASH_POOL_NODE_H__
#define __HASH_POOL_NODE_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include <cstddef> // size_t

#include <cassert>

//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif  
//=============================================================================

namespace CELL {

namespace HPOOLNODE {

//=============================================================================
//============================== THashPoolNode ================================
//=============================================================================
	template 
	< 
		typename TKeyType, 
		typename TValueType 
	>
	class THashPoolNode
	{
	public:
		using this_type = THashPoolNode< TKeyType, TValueType >;

		using hash_type = size_t;
		using key_type = TKeyType;
		using value_type = TValueType;

		static this_type * NIL_NODE;

	private:
		this_type * mpNextHPN;
		hash_type mHash;
		key_type mKey;
		value_type mValue;

	public:
		CONSTEXPR THashPoolNode() :
			mHash(0),
			mKey(),
			mValue(),
			mpNextHPN(NIL_NODE)
		{
		}
		CONSTEXPR THashPoolNode(const key_type & key, const value_type & value) :
			mpNextHPN(NIL_NODE),
			mHash(0),
			mKey(key),
			mValue(value)
		{
		}
		CONSTEXPR THashPoolNode(const key_type & key) :
			mpNextHPN(NIL_NODE),
			mHash(0),
			mKey(key),
			mValue()
		{
		}
		CONSTEXPR THashPoolNode(const this_type & node) :
			mpNextHPN(node.mpNextHPN),
			mHash(node.mHash),
			mKey(node.mKey),
			mValue(node.mValue)
		{
		}
		CONSTEXPRDTOR ~THashPoolNode()
		{
			mpNextHPN = NIL_NODE;
		}
		CONSTEXPR this_type & operator = (const this_type & node)
		{
			if (this != &node)
			{
				mpNextHPN = node.mpNextHPN;
				mHash = node.mHash;
				mKey = node.mKey;
				mValue = node.mValue;
			}
			return (*this);
		}
		CONSTEXPR this_type * getNextHPN() const
		{
			return mpNextHPN;
		}
		CONSTEXPR void setNextHPN(this_type * hpn)
		{
			mpNextHPN = hpn;
		}
		CONSTEXPR value_type & getValue()
		{
			return mValue;
		}
		CONSTEXPR const value_type & getValue() const
		{
			return mValue;
		}
		CONSTEXPR key_type & getKey()
		{
			return mKey;
		}
		CONSTEXPR const key_type & getKey() const
		{
			return mKey;
		}
		CONSTEXPR void setHash(const hash_type & hash)
		{
			mHash = hash;
		}
		CONSTEXPR hash_type & getHash()
		{
			return mHash;
		}
		CONSTEXPR const hash_type & getHash() const
		{
			return mHash;
		}
		CONSTEXPR bool compare(const key_type & key, const hash_type & hash) const
		{
			return ((mHash == hash) && (mKey == key));
		}
	};
//=============================================================================
	template 
	< 
		typename TKeyType, 
		typename TValueType 
	>
	THashPoolNode< TKeyType, TValueType > * THashPoolNode< TKeyType, TValueType >::NIL_NODE = nullptr;
//=============================================================================
} // namespace HPOOLNODE
//=============================================================================
} // namespace CELL
//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_POP
#endif  
//=============================================================================

#endif // __HASH_POOL_NODE_H__
