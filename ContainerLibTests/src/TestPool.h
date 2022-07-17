/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#include "TestDefines.h"

#ifndef __TEST_POOL_H__
#define __TEST_POOL_H__

//=============================================================================
template <typename pool_type>
INLINE void growingTestObjectPoolType(const size_t value_count)
{
	typedef typename pool_type::value_type value_type;

	typedef std::pair< size_t, size_t > old_new_pair;

	typedef TPool< old_new_pair >::Type old_new_pool;

	//TIMER_TOOLS::ContainerTimer sys_timer;
	//sys_timer.beginTiming();

	pool_type object_pool1;

	value_type val;
	size_t old_pool_cap = 0;
	size_t new_pool_cap = 0;
	bool print_pool_cap = false;

	old_new_pool vec_cap1;
	old_new_pool pool_cap1;
	for (size_t i = 0; i < value_count; ++i)
	{
		val = value_type(i);

		if (object_pool1.size() + 1 > object_pool1.capacity())
		{
			print_pool_cap = true;
		}
		old_pool_cap = object_pool1.capacity();
		object_pool1.push_back(val);
		new_pool_cap = object_pool1.capacity();
		if (print_pool_cap)
		{
			pool_cap1.push_back(old_new_pair(old_pool_cap, new_pool_cap));
			print_pool_cap = false;
		}
	}
	PRINT_PAIR_ELEMENTS(pool_cap1, "old - new pool capacity size");

	//sys_timer.endTiming();
	//sys_timer.print("Test pool growing", STREAM);
}
//=============================================================================
template <typename test_type>
INLINE void performanceCONDEST_TEST(const size_t test_array_size, const size_t tests)
{
	STREAM << "Performance construction/destruction tests for pooled arrays:"
		<< " size: " << to_string(test_array_size)
		<< " tests: " << to_string(tests) << std::endl;

	size_t pool_size = 0;
	size_t pool_cap = 0;

	//typedef TObjectPool<test_type > pool1_type;
	//typedef TObjectPool<test_type, std::allocator< test_type > > pool1_type;
	typedef typename TPool<test_type>::Type pool1_type;
	const double tval2 =
		CONDEST_TEST< pool1_type >(test_array_size, tests, pool_size, pool_cap);
	STREAM << "CONDEST_TEST TPool: " << "\t" << to_string(tval2) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;

	typedef typename TPO2Pool<test_type>::Type pool2_type;
	const double tval21 =
		CONDEST_TEST< pool2_type >(test_array_size, tests, pool_size, pool_cap);
	STREAM << "CONDEST_TEST TPO2Pool: " << "\t" << to_string(tval21) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;

	typedef std::vector<test_type> vec_type;
	const double tval1 =
		CONDEST_TEST< vec_type >(test_array_size, tests, pool_size, pool_cap);
	STREAM << "CONDEST_TEST STL Vector: " << "\t" << to_string(tval1) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;


	//typedef TArrayPool<test_type>::Type pool2_type;
	//const double tval3 = 
	//	CONDEST_TEST< pool2_type >( test_array_size, tests, pool_size, pool_cap );
	//STREAM << "CONDEST_TEST TArrayPool: " << "\t" << to_string<double>( tval3 ) << "\t"
	//	<< " size: " << to_string( pool_size )
	//	<< " capacity: " << to_string( pool_cap ) << std::endl;

	//typedef TVectorPool<test_type>::Type pool3_type;
	//const double tval4 = 
	//	CONDEST_TEST< pool3_type >( test_array_size, tests, pool_size, pool_cap );
	//STREAM << "CONDEST_TEST TVectorPool: " << "\t" << to_string<double>( tval4 ) << "\t"
	//	<< " size: " << to_string( pool_size )
	//	<< " capacity: " << to_string( pool_cap ) << std::endl;
}
//=============================================================================
template <typename test_type>
INLINE void performanceSTRESS_TEST(const size_t test_array_size, const size_t tests)
{
	STREAM << "Performance push_back/clear tests for pooled arrays:"
		<< " size: " << to_string(test_array_size)
		<< " tests: " << to_string(tests) << std::endl;

	size_t pool_size = 0;
	size_t pool_cap = 0;

	typedef typename TPool<test_type>::Type pool1_type;
	pool1_type pool1;
	double pool_timings1 =
		STRESS_TEST< pool1_type >(pool1, test_array_size, tests, pool_size, pool_cap);
	STREAM << "STRESS_TEST TPool: " << "\t" << to_string(pool_timings1) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;

	typedef typename TPO2Pool<test_type>::Type pool2_type;
	pool2_type pool12;
	double pool_timings12 =
		STRESS_TEST< pool2_type >(pool12, test_array_size, tests, pool_size, pool_cap);
	STREAM << "STRESS_TEST TPO2Pool: " << "\t" << to_string(pool_timings12) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;

	typedef std::vector<test_type> vec_type;
	vec_type vec;
	double vec_timings =
		STRESS_TEST< vec_type >(vec, test_array_size, tests, pool_size, pool_cap);
	STREAM << "STRESS_TEST STL Vector: " << "\t" << to_string(vec_timings) << "\t"
		<< " size: " << to_string(pool_size)
		<< " capacity: " << to_string(pool_cap) << std::endl;

	//typedef TVectorPool<test_type>::Type pool3_type;
	//pool3_type pool3;
	//double pool_timings3 = 
	//	STRESS_TEST< pool3_type >( pool3, test_array_size, tests, pool_size, pool_cap );
	//STREAM << "STRESS_TEST TVectorPool: " << "\t" << to_string<double>( pool_timings3 ) << "\t"
	//	<< " size: " << to_string( pool_size ) 
	//	<< " capacity: " << to_string( pool_cap ) << std::endl;

	//typedef TArrayPool<test_type>::Type pool2_type;
	//pool2_type pool2;
	//double pool_timings2 = 
	//	STRESS_TEST< pool2_type >( pool2, test_array_size, tests, pool_size, pool_cap );
	//STREAM << "STRESS_TEST TArrayPool: " << "\t" << to_string<double>( pool_timings2 ) << "\t"
	//	<< " size: " << to_string( pool_size ) 
	//	<< " capacity: " << to_string( pool_cap ) << std::endl;

	//typedef TBTPool<test_type>::Type pool4_type;
	//pool4_type pool4;
	//double pool_timings4 = 
	//	STRESS_TEST< pool4_type >( pool4, test_array_size, tests, pool_size, pool_cap );
	//STREAM << "STRESS_TEST TBTPool: " << "\t" << "\t" << to_string<double>( pool_timings4 ) << "\t"
	//	<< " size: " << to_string( pool_size ) 
	//	<< " capacity: " << to_string( pool_cap ) << std::endl;
}
//=============================================================================
	void performanceTestsForPool();
	void growingTestsForPool();

	bool testLIFOPool(bool stream_results, const random_pool_type & TEST_VALUES);
	bool testFIFOPool(bool stream_results, const random_pool_type & TEST_VALUES);
	bool testObjectPool(bool stream_results, const random_pool_type& TEST_VALUES);
//=============================================================================

#endif // __TEST_POOL_H__
