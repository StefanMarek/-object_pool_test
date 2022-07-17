
#include "TestPool.h"

//=============================================================================
bool testLIFOPool(bool stream_results, const random_pool_type & TEST_VALUES)
{
	typedef TestType value_type;
	typedef TLIFOP<value_type>::Type stack_t;

	const size_t value_count = 1024;

	stack_t object_pool1;
	const size_t obj_size1 = object_pool1.size();
	if (!object_pool1.empty())
		return false;

	value_type val;
	for (size_t i = 0; i < value_count; ++i)
	{
		val = value_type(i);
		object_pool1.push(val);
	}
	for (size_t i = 0; i < value_count; ++i)
	{
		val = object_pool1.top();
		object_pool1.pop();
	}
	return true;
}

//=============================================================================
bool testFIFOPool(bool stream_results, const random_pool_type & TEST_VALUES)
{
	typedef TestType value_type;
	typedef TestType value_type;
	typedef TFIFOP<value_type>::Type queue_t;

	const size_t value_count = 1024;

	queue_t object_pool1;
	const size_t obj_size1 = object_pool1.size();
	if (!object_pool1.empty())
		return false;

	value_type val;
	for (size_t i = 0; i < value_count; ++i)
	{
		val = value_type(i);
		object_pool1.push(val);
	}
	for (size_t i = 0; i < value_count; ++i)
	{
		val = object_pool1.top();
		object_pool1.pop();
	}
	return true;
}
//=============================================================================
	bool testObjectPool( bool stream_results, const random_pool_type & TEST_VALUES )
	{
		typedef TestType value_type;

		typedef TPool< value_type >::Type pool_type;

		typedef std::vector< value_type > vector_type;

		const size_t value_count = 1024;

		vector_type vec_pool0;
		const size_t vec_poolobj_size0 = vec_pool0.size();
		const size_t vec_poolcap_size0 = vec_pool0.capacity();
		vector_type vec_pool1;
		const size_t vec_poolobj_size1 = vec_pool1.size();
		const size_t vec_poolcap_size1 = vec_pool1.capacity();
		vector_type vec_pool2;
		vec_pool2.reserve( value_count );
		const size_t vec_poolobj_size2 = vec_pool2.size();
		const size_t vec_poolcap_size2 = vec_pool2.capacity();
		vector_type vec_pool3;
		vec_pool3.resize( value_count );
		const size_t vec_poolobj_size3 = vec_pool3.size();
		const size_t vec_poolcap_size3 = vec_pool3.capacity();

		pool_type object_pool0;
		const size_t obj_size0 = object_pool0.size();
		const size_t cap_size0 = object_pool0.capacity();
		if ( !object_pool0.empty() )
			return false;
		pool_type object_pool1;
		const size_t obj_size1 = object_pool1.size();
		const size_t cap_size1 = object_pool1.capacity();
		if ( !object_pool1.empty() )
			return false;
		pool_type object_pool2;
		object_pool2.reserve( value_count );
		const size_t obj_size2 = object_pool2.size();
		const size_t cap_size2 = object_pool2.capacity();
		if ( !object_pool2.empty() )
			return false;
		pool_type object_pool4;
		object_pool4.resize( value_count );
		const size_t obj_size4 = object_pool4.size();
		const size_t cap_size4 = object_pool4.capacity();
		if ( object_pool4.empty() )
			return false;

		value_type val;
		for (size_t i = 0; i < value_count; ++i)
		{
			val = value_type(i);
			vec_pool1.push_back( val );
			object_pool1.push_back( val );
		}
		for (int i = 0; i < value_count; ++i)
		{
			val = value_type(i);
			vec_pool2.push_back( val );
			object_pool2.push_back( val );
		}
		for (int i = 0; i < value_count; ++i)
		{
			val = value_type(i);
			vec_pool3.push_back( val );
			object_pool4.push_back( val );
		}

		vec_pool0 = vector_type( vec_pool2.begin(), vec_pool2.end() );
		object_pool0 = pool_type( object_pool2.begin(), object_pool2.end() );
		if ( object_pool0.empty() )
			return false;

		const size_t vec_poolobj_size02 = vec_pool0.size();
		const size_t vec_poolcap_size02 = vec_pool0.capacity();
		const size_t object_poolobj_size02 = object_pool0.size();
		const size_t object_poolcap_size02 = object_pool0.capacity();

		const size_t vec_poolobj_size12 = vec_pool1.size();
		const size_t vec_poolcap_size12 = vec_pool1.capacity();
		const size_t object_poolobj_size12 = object_pool1.size();
		const size_t object_poolcap_size12 = object_pool1.capacity();

		const size_t vec_poolobj_size22 = vec_pool2.size();
		const size_t vec_poolcap_size22 = vec_pool2.capacity();
		const size_t object_poolobj_size22 = object_pool2.size();
		const size_t object_poolcap_size22 = object_pool2.capacity();

		const size_t vec_poolobj_size32 = vec_pool3.size();
		const size_t vec_poolcap_size32 = vec_pool3.capacity();
		const size_t object_poolobj_size33 = object_pool4.size();
		const size_t object_poolcap_size33 = object_pool4.capacity();

		object_pool1.resize( 0 );
		const size_t obj_size11 = object_pool1.size();
		const size_t cap_size11 = object_pool1.capacity();
		if ( !object_pool1.empty() )
			return false;

		object_pool2.clear();
		const size_t obj_size21 = object_pool2.size();
		const size_t cap_size21 = object_pool2.capacity();
		if ( !object_pool2.empty() )
			return false;
		object_pool4.reserve( 0 );
		const size_t obj_size32 = object_pool4.size();
		const size_t cap_size32 = object_pool4.capacity();
		if ( object_pool4.empty() )
			return false;

		vec_pool1.resize( 0 );
		const size_t vec_poolobj_size11 = vec_pool1.size();
		const size_t vec_poolcap_size11 = vec_pool1.capacity();
		vec_pool2.clear();
		const size_t vec_poolobj_size21 = vec_pool2.size();
		const size_t vec_poolcap_size21 = vec_pool2.capacity();
		vec_pool3.reserve( 0 );
		const size_t vec_poolobj_size31 = vec_pool3.size();
		const size_t vec_poolcap_size31 = vec_pool3.capacity();

		return true;
	}
//=============================================================================
	void growingTestsForPool()
	{
		STREAM << std::endl << "Test object pool" << std::endl;

		typedef TestType value_type;
		//typedef long value_type;

		const size_t value_count = 516683;
		//const size_t value_count = 1024*1024;

		STREAM << std::endl << "Test Case vector" << std::endl;
		typedef std::vector< value_type > vector_type;
		growingTestObjectPoolType< vector_type >( value_count );

		STREAM << std::endl << "Test Case TPool" << std::endl;
		typedef TPool< value_type >::Type object_pool_type;
		growingTestObjectPoolType< object_pool_type >( value_count );

		STREAM << std::endl << "Test Case TPO2Pool" << std::endl;
		typedef TPO2Pool< value_type >::Type object_pool_type3;
		growingTestObjectPoolType< object_pool_type3 >(value_count);

		//STREAM << std::endl << "Test Case TArrayPool" << std::endl;
		//typedef TArrayPool< value_type >::Type array_pool_type;
		//growingTestObjectPoolType< array_pool_type >( value_count );

		//STREAM << std::endl << "Test Case TExpPool" << std::endl;
		//typedef TExpPool< value_type >::Type exp_pool_type;
		//growingTestObjectPoolType< exp_pool_type >( value_count );

		//STREAM << std::endl << "Test Case TBTPool" << std::endl;
		//typedef TBTPool< value_type >::Type tree_pool_type;
		//growingTestObjectPoolType< tree_pool_type >( value_count );
	}
//=============================================================================
	void performanceTestsForPool()
	{
		// All pool types will be tested against std vector:
		const size_t test_array_size = 1024 * 1024;
		const size_t testsCT = 32;
		const size_t testsST = 7;

		// test case TestType: CONDEST_TEST is similar, STRESS_TEST similar
		STREAM << "Performance Tests for type TestType: " << std::endl;
		performanceCONDEST_TEST< TestType >( test_array_size, testsCT );
		performanceSTRESS_TEST< TestType >(test_array_size, testsST);

		// test case long: CONDEST_TEST is fast, STRESS_TEST slower
		//STREAM << "Performance Tests for type long: " << std::endl;
		//performanceCONDEST_TEST< long >( test_array_size, testsCT );
		//performanceSTRESS_TEST< long >( test_array_size, testsST );
	}
//=============================================================================
