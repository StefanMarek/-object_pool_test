/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	

*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __TEST_DEFINES_H__
#define __TEST_DEFINES_H__

#include "FIFOPool.h"
#include "LIFOPool.h"
#include "ContainerTools.h"
#include "MathTools.h"
#include "Allocator.h"
#include "ObjectPoolBuffer.h"
#include "PODObjectPoolBuffer.h"
#include "ObjectPool.h"
#include "StringPool.h"
#include "IndexHeap.h"
#include "PairValues.h"
#include "HashTools.h"
#include "HashPool.h"
#include "ContainerTimer.h"

using namespace CELL::ALLOC;
using namespace CELL::ALLOC::BUFFERPOD;
using namespace CELL::ALLOC::PBUFFER;
using namespace CELL::OPOOL;
using namespace CELL::SPOOL;
using namespace CELL::TOOLS;
using namespace CELL::LIFO_POOL;
using namespace CELL::FIFO_POOL;
using namespace CELL::HPOOL;
using namespace CELL::BINARY_INDEX_HEAP;
using namespace CELL::PAIR;

#include <cstddef> // ptrdiff_t, size_t
#include <limits> // std::numeric_limits<TValueType>::max()
#include <cassert>
#include <random>
#include <ctime> // time
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
typedef double random_value_type;
typedef typename TPool< random_value_type >::Type random_pool_type;

//#define STRING_STREAMING
#ifndef STRING_STREAMING
#define STREAM std::cout
#endif

//=============================================================================
//========================== TEST TYPE ========================================
//=============================================================================
	class TestType
	{
	public:
		//typedef std::string string_t;
		typedef CELL::SPOOL::string_t string_t;
		union
		{
			int int_array[3];
			struct //int_element_type
			{
				int int_value1;
				int int_value2;
				int int_value3;
			};// element;
		};
		double double_value;
		//BYTE chunk[36];
		byte * chunk;
		size_t chunk_size;
		string_t mKey;

	public:
		TestType() : double_value(0.0), chunk(nullptr), chunk_size(0)
		{
			int_value1 = 0;
			int_value2 = 0;
			int_value3 = 0; 
		}
		TestType( int v ) : chunk(nullptr), chunk_size(0)
		{
			set(v);
		}
		TestType(int v, const string_t & key) : chunk(nullptr), chunk_size(0)
		{
			set(v, key);
		}
		TestType( const TestType & vt ) : chunk(nullptr), chunk_size(0)
		{
			assign( vt );
		}
		TestType( int v1, int v2, int v3, double v = 0.0 ) : 
			double_value(v), chunk(nullptr), chunk_size(0)
		{
			int_value1 = (v1);
			int_value2 = (v2);
			int_value3 = (v3); 
		}
		~TestType()
		{
			releaseChunck();
			int_value1 = 0;
			int_value2 = 0;
			int_value3 = 0;
			double_value = 0;
		}

		TestType & operator = ( const TestType & vt )
		{
			return ( assign( vt ) );
		}
		TestType & assign ( const TestType & vt )
		{
			int_value1 = vt.int_value1;
			int_value2 = vt.int_value2;
			int_value3 = vt.int_value3;
			double_value = vt.double_value;
			if ( vt.chunk_size )
			{
				setChunk( vt.chunk_size );
				memcpy( chunk, vt.chunk, chunk_size * sizeof(byte) );
			}
			mKey = vt.mKey;
			return (*this);
		}
		const double & get() const
		{
			return double_value;
		}
		const int & operator [] ( const size_t &  i ) const
		{
			return int_array[i];
		}
		void set( int v1, int v2, int v3, double v = 0.0 ) 
		{
			int_value1 = v1;
			int_value2 = v2;
			int_value3 = v3;
			double_value = v;
		}
		void releaseChunck()
		{
			if (chunk)
			{
				delete[] (chunk);
				chunk = nullptr;
				chunk_size = 0;
			}
		}
		void setChunk( size_t size )
		{
			if (size)
			{
				releaseChunck();
				chunk_size = size;
				chunk = ::new byte[chunk_size];
				memset(chunk, 0, chunk_size * sizeof(byte) );
			}
		}
		void set(int v)
		{
			int_value1 = v;
			int_value2 = v;
			int_value3 = v;
			double_value = v;
			setChunk( 4 );
			mKey = SPTOOLS::number_to_string(v);
			//mKey = to_string(v);
		}
		void set(int v, const string_t & key)
		{
			int_value1 = v;
			int_value2 = v;
			int_value3 = v;
			double_value = v;
			setChunk(4);
			mKey = key;
		}
		void print(std::ostream & os = std::cout) const
		{
			//os << "Print TestType: " << std::endl;
			os  << "( " << int_value1 
				<< "; " << int_value2 
				<< "; " << int_value3 
				<< " )" << std::endl;
			//os << double_value << std::endl;
		}
		friend std::ostream & operator << ( std::ostream & os, const TestType & vt )
		{
			vt.print( os );
			return os;
		}
		friend bool operator < (const TestType & lhs, const TestType & rhs)
		{
			return (lhs.get() < rhs.get());
		}
		friend bool operator > (const TestType & lhs, const TestType & rhs)
		{
			return (lhs.get() > rhs.get());
		}
		friend bool operator == (const TestType & lhs, const TestType & rhs)
		{
			return (lhs.get() == rhs.get());
		}
	};
//=============================================================================
	class TestKeyType
	{
	public:
		//typedef std::string string_t;
		typedef CELL::SPOOL::string_t string_t;

		typedef TestType value_type;
		typedef string_t key_type;

		TestType mValue;
		string_t mKey;

	public:
		TestKeyType()
		{
		}
		TestKeyType(const string_t & key, const TestType & value) : mKey(key), mValue(value)
		{
		}
		TestKeyType(const TestKeyType & vt)
		{
			assign(vt);
		}

		TestKeyType & operator = (const TestKeyType & vt)
		{
			return (assign(vt));
		}
		TestKeyType & assign(const TestKeyType & vt)
		{
			mValue = vt.mValue;
			mKey = vt.mKey;
			return (*this);
		}
		value_type & getValue()
		{
			return mValue;
		}
		const value_type & getValue() const
		{
			return mValue;
		}
		key_type & getKey()
		{
			return mKey;
		}
		const key_type & getKey() const
		{
			return mKey;
		}
	};
	class TestKeyTypeInt
	{
	public:
		//typedef std::string string_t;
		typedef CELL::SPOOL::string_t string_t;

		typedef TestType value_type;
		typedef uint32_t key_type;

		TestType mValue;
		uint32_t mKey;

	public:
		TestKeyTypeInt()
		{
		}
		TestKeyTypeInt(const uint32_t& key, const TestType& value) : mKey(key), mValue(value)
		{
		}
		TestKeyTypeInt(const TestKeyTypeInt& vt)
		{
			assign(vt);
		}

		TestKeyTypeInt& operator = (const TestKeyTypeInt& vt)
		{
			return (assign(vt));
		}
		TestKeyTypeInt& assign(const TestKeyTypeInt& vt)
		{
			mValue = vt.mValue;
			mKey = vt.mKey;
			return (*this);
		}
		value_type& getValue()
		{
			return mValue;
		}
		const value_type& getValue() const
		{
			return mValue;
		}
		key_type& getKey()
		{
			return mKey;
		}
		const key_type& getKey() const
		{
			return mKey;
		}
	};
	class TestTypeSTDString
	{
	public:
		typedef std::string string_t;

		union
		{
			int int_array[3];
			struct //int_element_type
			{
				int int_value1;
				int int_value2;
				int int_value3;
			};// element;
		};
		double double_value;
		//BYTE chunk[36];
		byte * chunk;
		size_t chunk_size;
		string_t mKey;

	public:
		TestTypeSTDString() : double_value(0.0), chunk(nullptr), chunk_size(0)
		{
			int_value1 = 0;
			int_value2 = 0;
			int_value3 = 0;
		}
		TestTypeSTDString(int v) : chunk(nullptr), chunk_size(0)
		{
			set(v);
		}
		TestTypeSTDString(int v, const string_t & key) : chunk(nullptr), chunk_size(0)
		{
			set(v, key);
		}
		TestTypeSTDString(const TestTypeSTDString & vt) : chunk(nullptr), chunk_size(0)
		{
			assign(vt);
		}
		TestTypeSTDString(int v1, int v2, int v3, double v = 0.0) :
			double_value(v), chunk(nullptr), chunk_size(0)
		{
			int_value1 = (v1);
			int_value2 = (v2);
			int_value3 = (v3);
		}
		~TestTypeSTDString()
		{
			releaseChunck();
			int_value1 = 0;
			int_value2 = 0;
			int_value3 = 0;
			double_value = 0;
		}

		TestTypeSTDString & operator = (const TestTypeSTDString & vt)
		{
			return (assign(vt));
		}
		TestTypeSTDString & assign(const TestTypeSTDString & vt)
		{
			int_value1 = vt.int_value1;
			int_value2 = vt.int_value2;
			int_value3 = vt.int_value3;
			double_value = vt.double_value;
			if (vt.chunk_size)
			{
				setChunk(vt.chunk_size);
				memcpy(chunk, vt.chunk, chunk_size * sizeof(byte));
			}
			mKey = vt.mKey;
			return (*this);
		}
		const double & get() const
		{
			return double_value;
		}
		const int & operator [] (const size_t &  i) const
		{
			return int_array[i];
		}
		void set(int v1, int v2, int v3, double v = 0.0)
		{
			int_value1 = v1;
			int_value2 = v2;
			int_value3 = v3;
			double_value = v;
		}
		void releaseChunck()
		{
			if (chunk)
			{
				delete[](chunk);
				chunk = nullptr;
				chunk_size = 0;
			}
		}
		void setChunk(size_t size)
		{
			if (size)
			{
				releaseChunck();
				chunk_size = size;
				chunk = ::new byte[chunk_size];
				memset(chunk, 0, chunk_size * sizeof(byte));
			}
		}
		void set(int v)
		{
			int_value1 = v;
			int_value2 = v;
			int_value3 = v;
			double_value = v;
			setChunk(4);
			mKey = to_string(v);
		}
		void set(int v, const string_t & key)
		{
			int_value1 = v;
			int_value2 = v;
			int_value3 = v;
			double_value = v;
			setChunk(4);
			mKey = key;
		}
		void print(std::ostream & os = std::cout) const
		{
			//os << "Print TestType: " << std::endl;
			os << "( " << int_value1
				<< "; " << int_value2
				<< "; " << int_value3
				<< " )" << std::endl;
			//os << double_value << std::endl;
		}
		friend std::ostream & operator << (std::ostream & os, const TestTypeSTDString & vt)
		{
			vt.print(os);
			return os;
		}
		friend bool operator < (const TestTypeSTDString & lhs, const TestTypeSTDString & rhs)
		{
			return (lhs.get() < rhs.get());
		}
		friend bool operator > (const TestTypeSTDString & lhs, const TestTypeSTDString & rhs)
		{
			return (lhs.get() > rhs.get());
		}
		friend bool operator == (const TestTypeSTDString & lhs, const TestTypeSTDString & rhs)
		{
			return (lhs.get() == rhs.get());
		}
	};
	class TestKeyTypeSTDString
	{
	public:
		typedef std::string string_t;
		
		typedef TestTypeSTDString value_type;
		typedef string_t key_type;

		TestTypeSTDString mValue;
		string_t mKey;

	public:
		TestKeyTypeSTDString()
		{
		}
		TestKeyTypeSTDString(const string_t & key, const TestTypeSTDString & value) : mKey(key), mValue(value)
		{
		}
		TestKeyTypeSTDString(const TestKeyTypeSTDString & vt)
		{
			assign(vt);
		}

		TestKeyTypeSTDString & operator = (const TestKeyTypeSTDString & vt)
		{
			return (assign(vt));
		}
		TestKeyTypeSTDString & assign(const TestKeyTypeSTDString & vt)
		{
			mValue = vt.mValue;
			mKey = vt.mKey;
			return (*this);
		}
		value_type & getValue()
		{
			return mValue;
		}
		const value_type & getValue() const
		{
			return mValue;
		}
		key_type & getKey()
		{
			return mKey;
		}
		const key_type & getKey() const
		{
			return mKey;
		}
	};
//=============================================================================
	class CompareValueType //: public std::binary_function<TestType, TestType, bool>
	{
	public:
		CompareValueType()
		{
		}
		bool operator () (TestType & lhs, TestType & rhs) 
		{
			return (lhs.int_value1 < rhs.int_value1);
		}
	};
//=============================================================================

//=============================================================================
	template <typename TValueType>
	struct CompareHeapValues //: std::binary_function<TValueType, TValueType, bool>
	{
		typedef unsigned int axis_type;
		axis_type mAxis;
		CompareHeapValues( axis_type axis = 2 ) : mAxis( axis )
		{
		}
		bool operator () (const TValueType & lhs, const TValueType & rhs) const
		{
			return ( lhs[mAxis] < rhs[mAxis] );
		}
	};
//=============================================================================

//=============================================================================
//========================== Testing routines =================================
//=============================================================================

#define DUMP_ELEMENTS

	template <class T>
	INLINE void PRINT_ELEMENTS (const T & coll, const std::string & optcstr = "" ) // const char * optcstr = "")
	{
	#ifdef DUMP_ELEMENTS
		STREAM << "PRINT_ELEMENTS: "  << optcstr << std::endl;
		typename T::const_iterator iter = coll.begin();
		typename T::const_iterator iter_end = coll.end();
		//typedef typename T::value_type value_type;
		while (iter != iter_end)
		{
			STREAM << to_string<typename T::value_type>( *iter ) << std::endl;
			++iter;
		}
		STREAM << "elements printed: " << to_string( coll.size() ) << std::endl;
		STREAM << std::endl;
	#endif
	}
	template <class T>
	INLINE void PRINT_PELEMENTS (const T & coll, const std::string & optcstr = "" ) // const char * optcstr = "")
	{
	#ifdef DUMP_ELEMENTS
		STREAM << "PRINT_PELEMENTS: "  << optcstr << std::endl;
		typename T::const_iterator iter = coll.begin();
		typename T::const_iterator iter_end = coll.end();
		typedef typename T::value_type value_type;
		value_type val;
		while (iter != iter_end)
		{
			val = *iter;
			STREAM << to_string( *val ) << std::endl;
			++iter;
		}
		STREAM << "elements printed: " << to_string( coll.size() ) << std::endl;
		STREAM << std::endl;
	#endif
	}
	template <class T>
	INLINE void PRINT_PAIR_ELEMENTS (const T & coll, const std::string & optcstr = "" ) // const char * optcstr = "")
	{
	#ifdef DUMP_ELEMENTS
		STREAM << "PRINT_PAIR_ELEMENTS: "  << optcstr << std::endl;
		typename T::const_iterator iter = coll.begin();
		typename T::const_iterator iter_end = coll.end();
		while (iter != iter_end) 
		{
			STREAM << "first: " << (*iter).first
				<< " - second: " << (*iter).second << "; " << std::endl;
			++iter;
		}
		STREAM << "pair elements printed: " << to_string( coll.size() ) << std::endl;
		STREAM << std::endl;
	#endif
	}
	template < typename TPoolType >
	INLINE double STRESS_TEST( TPoolType & pool, 
		const size_t & maxInsertions, const size_t & numTests, 
		size_t & pool_size, size_t & pool_cap )
	{
		CELL::TOOLS::MEMORY::free_container< TPoolType >( pool );
		TIMER_TOOLS::ContainerTimer sysTimer;
		sysTimer.beginTiming();
		//clock_t start = clock();
		pool_size = 0;
		pool_cap = 0;
		for ( size_t test = 0; test < numTests; ++test )
		{
			for ( size_t i = 0; i < maxInsertions; ++i )
			{
				pool.push_back( rand() );
			}
			pool_size += pool.size(); 
			pool_cap += pool.capacity(); 
			pool.clear();
		}
		//clock_t end = clock();
		sysTimer.endTiming();
		return sysTimer.getElapsedTiming();
		//return (end - start);
	}
	template < typename TPoolType >
	INLINE double CONDEST_TEST( const size_t test_array_size, const size_t tests, 
		size_t & pool_size, size_t & pool_cap )
	{
		TIMER_TOOLS::ContainerTimer systimer1;
		systimer1.beginTiming();
		pool_size = 0;
		pool_cap = 0;
		for (size_t i = 0; i < tests ; ++i)
		{
			TPoolType pool( test_array_size );
			pool_size += pool.size(); 
			pool_cap += pool.capacity(); 
		}
		systimer1.endTiming();
		return systimer1.getElapsedTiming();
	}
	template <typename TPoolType>
	INLINE double STRESS_QUEUE( TPoolType & pool, const size_t & maxInsertions, const size_t & numTests )
	{
		TIMER_TOOLS::ContainerTimer systimer1;
		systimer1.beginTiming();
		//clock_t start = clock();	
		for (  size_t test = 0; test < numTests; ++test )
		{
			for (  size_t i = 0; i < maxInsertions; ++i )
			{
				pool.push( rand() );
			}
			for (  size_t i = 0; i < maxInsertions; ++i )
			{
				pool.pop();
			}
		}
		//clock_t end = clock();
		//return (end - start);
		systimer1.endTiming();
		return systimer1.getElapsedTiming();
	}

#endif // __TEST_DEFINES_H__
