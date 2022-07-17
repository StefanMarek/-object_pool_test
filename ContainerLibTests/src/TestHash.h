/*****************************************************************************/
/*!
@author			Stefan Marek
@description

*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __TEST_HASH_H__
#define __TEST_HASH_H__

#include "TestDefines.h"
#include "HashTools.h"

namespace CELL
{
	namespace TOOLS
	{
		namespace HASH
		{
			template<>
			struct THashFunc< std::string >
			{
				typedef size_t size_type;
				typedef std::string argument_type;
				typedef size_t return_type;

				size_t operator()(size_t const& hash, size_t const& size) const THROW_NEVER
				{
					return (hash & (size - static_cast<size_t>(1)));
				}
				size_t operator()(const std::string& value) const
				{
					//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value.c_str()));
					const size_t sizeofKey = value.size();
					return STL_Hash_seq(value.c_str(), sizeofKey);
				}
			};
			struct TMapHashFunc
			{
				typedef size_t size_type;
				typedef CELL::SPOOL::string_t argument_type;

				size_t operator()(const CELL::SPOOL::string_t& value) const THROW_NEVER
				{
					//const unsigned char * bytes = static_cast<const unsigned char *>(static_cast<const void *>(value.c_str()));
					const size_t sizeofKey = value.size();
					return STL_Hash_seq(value.c_str(), sizeofKey);
				}
			};
		} // namespace HASH
	} // namespace TOOLS
} // namespace CELL

bool testHashTable(bool stream_results, const random_pool_type & TEST_VALUES);
bool testHashTableSTDString(bool stream_results, const random_pool_type & TEST_VALUES);
bool testClusteredHashTable(bool stream_results, CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type & clustered_data);

#endif // __TEST_HASH_H__
