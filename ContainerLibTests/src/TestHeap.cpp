
#include "TestHeap.h"

//=============================================================================
	bool testHeap1( bool stream_results, const random_pool_type & TEST_VALUES )
	{
		//typedef TestType test_value_t;
		typedef double heap_value_t;

		//const heap_value_t radius = 1000.0;
		const size_t size = 1024;
		//const size_t size = 15;
		const size_t heap_size = size;
		//const size_t array_size = heap_size;
		//heap_value_t val[15] = {1, 2, 4, 7, 15, 5, 3, 6, 8, 14, 10, 11, 9, 13, 12};
		//if ( stream_results )
		//	STREAM << "1; 2; 3; 6; 10; 5; 4; 7; 8; 15; 14; 11; 9; 13; 12; " << std::endl;
		//TPool<heap_value_t>::Type values( array_size );
		//if ( values.empty() )
		//	return false;
		//for (size_t i = 0; i < 15; ++i)
		//{
		//	values[i] = val[i] * radius; 
		//}
		//RandomMT rand_gen;
		//for (size_t i = 15; i < array_size; ++i)
		//{
		//	values[i] = rand_gen.getNextDouble();// * radius; 
		//}
		//if ( stream_results )
		//	PRINT_ELEMENTS( values, "array values" );

		//TPool<heap_value_t>::Type values(TEST_VALUES.begin(), TEST_VALUES.begin() + heap_size);
		TPool<heap_value_t>::Type values;

		random_pool_type::const_iterator iter = TEST_VALUES.begin();
		random_pool_type::const_iterator iter_end = TEST_VALUES.begin() + heap_size;
		while (iter != iter_end)
		{
			values.push_back(heap_value_t(*iter));
			++iter;
		}

		if ( stream_results )
			STREAM << std::endl << "Test Case: TMinHeap " << std::endl;

		//ContainerTimer systimer2;
		//systimer2.beginTiming();

		TMinHeap< heap_value_t *>::Type min_heap;
		heap_value_t * val;
		for (size_t i = 0; i < heap_size; ++i)
		{
			val = new heap_value_t(values[i]);
			min_heap.push( val );
		}
		if ( min_heap.empty() )
			return false;
		if ( !min_heap.is_heap() )
			return false;
		const size_t min_heap_size = min_heap.size();
		if ( stream_results )
			PRINT_PELEMENTS( min_heap, "min_heap values" );
		TPool<heap_value_t *>::Type heap1_values( heap_size );
		if ( heap1_values.empty() )
			return false;
		for (size_t i = 0; i < heap_size; ++i)
		{
			heap1_values[i] = min_heap.top();
			min_heap.pop();
		}

		if ( !min_heap.empty() )
			return false;
		if ( stream_results )
			PRINT_PELEMENTS( heap1_values, "sorted min_heap values" );

		for (size_t i = 0; i < heap_size; ++i)
		{
			delete heap1_values[i];
			heap1_values[i] = nullptr;
		}

		//systimer2.endTiming();
		//systimer2.print("Test Binary Min Heap", STREAM);

		if ( stream_results )
			STREAM << std::endl << "Test Case: TMaxHeap: " << std::endl;

		//ContainerTimer systimer3;
		//systimer3.beginTiming();

		TMaxHeap< heap_value_t>::Type max_heap( values.begin(), values.end() );
		if ( max_heap.empty() )
			return false;
		if ( !max_heap.is_heap() )
			return false;
		const size_t max_heap_size = max_heap.size();
		if ( stream_results )
			PRINT_ELEMENTS( max_heap, "max_heap values" );
		TPool<heap_value_t>::Type max_heap_values(max_heap_size);

		for (size_t i = 0; i < max_heap_size; ++i)
		{
			max_heap_values[i] = max_heap.top();
			max_heap.pop();
		}
		if (stream_results)
			PRINT_ELEMENTS(max_heap_values, "sorted max_heap values");
		//max_heap.sort();
		//if ( stream_results )
		//	PRINT_ELEMENTS( max_heap, "sorted max_heap values" );
//		max_heap.assure_heap();
//		PRINT_ELEMENTS( max_heap );
		TPool<heap_value_t>::Type heap2_values( max_heap.begin(), max_heap.end() );
		if ( heap2_values.empty() )
			return false;

		TMaxHeap< heap_value_t >::Type max_heap2;
		max_heap.swap( max_heap2 );

		//for (size_t i = 0; i < heap_size; ++i)
		//{
		//	heap2_values[i] = max_heap.top();
		//	max_heap.pop();
		//}
		if ( !min_heap.empty() )
			return false;

		//systimer3.endTiming();
		//systimer3.print("Test Binary Max Heap", STREAM);

		return true;
	}
//=============================================================================
	bool testHeap2( bool stream_results, const random_pool_type & TEST_VALUES )
	{
		//ContainerTimer systimer;
		//systimer.beginTiming();

		typedef LAVector3d value_t;
		const double radius = 1000.0;
		const size_t size = 1024;
		const size_t heap_size = size;
		const size_t array_size = heap_size;

		size_t index = 0;
		size_t z = 0;
		TPool< value_t >::Type array_values( 10 );
		array_values[index++] = value_t( 35, 60, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 20, 45, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 60, 80, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 10, 35, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 80, 40, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 20, 20, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 50, 30, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 90, 65, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 70, 25, static_cast<float>( ++z ) ) * radius;
		array_values[index++] = value_t( 60, 10, static_cast<float>( ++z ) ) * radius;
		size_t next_val = 0;
		for (size_t i = 10; i < array_size; ++i)
		{
			//array_values.push_back( value_t( rand_gen.getNextDouble(), 
			//	rand_gen.getNextDouble(), rand_gen.getNextDouble() ) );
			array_values.push_back( value_t( TEST_VALUES[next_val++], 
				TEST_VALUES[next_val++], TEST_VALUES[next_val++] ) );
		}
		//if ( stream_results )
		//	PRINT_ELEMENTS( array_values, "array values" );
		if ( array_values.empty() )
			return false;
		const size_t arr_sz = array_values.size();

		TPool< value_t >::Type::iterator iter_begin = array_values.begin();
		TPool< value_t >::Type::iterator iter_end = array_values.end();
		const size_t dist = std::distance( iter_begin, iter_end );
		const size_t sovt = sizeof( value_t );
		typedef TBinaryHeap< value_t, CompareHeapValues< value_t > > heap_type;
		unsigned int main_axis = 1;
		CompareHeapValues< value_t > comparer( main_axis );
		heap_type heap_values( comparer );
		//heap_values.assign( array_values.begin(), array_values.end() );
		while ( iter_begin != iter_end )
		{
			heap_values.push( *iter_begin );
			++iter_begin;
		}
		if ( heap_values.empty() )
			return false;
		if ( !heap_values.is_heap() )
			return false;
		if ( stream_results )
			PRINT_ELEMENTS( heap_values, "heap values heap axis " + to_string<unsigned int>( main_axis ) );
		TPool<value_t>::Type vec_t;
		value_t heap_val;
		for (size_t i = 0; i < heap_size; ++i)
		{
			heap_val = heap_values.front();
			vec_t.push_back( heap_val );
			heap_values.pop();
		}
		if ( stream_results )
			PRINT_ELEMENTS( vec_t, "sorted heap values");

		return true;
	}
//=============================================================================
	bool testMinHeapOfPairs( bool stream_results, const random_pool_type & TEST_VALUES )
	{
		typedef double heap_value_t;
		typedef TestType test_value_t;
		if ( stream_results )
		{
			STREAM << std::endl << "Test MinHeapOfPairs" << std::endl;
			STREAM << "1; 2; 3; 6; 10; 5; 4; 7; 8; 15; 14; 11; 9; 13; 12; " << std::endl;
		}
		const heap_value_t radius = 1000.0;
		//const size_t size = 1024;
		const size_t size = 15;
		const size_t heap_size = size;
		const size_t array_size = heap_size;
		heap_value_t val[15] = {1, 2, 4, 7, 15, 5, 3, 6, 8, 14, 10, 11, 9, 13, 12 };
		TPool<heap_value_t>::Type values( val, val+15 );
		for (size_t i = 0; i < 15; ++i)
		{
			values[i] *= radius; 
		}
		if ( stream_results )
			STREAM << std::endl << "Test TMinHeapOfPairs: " << std::endl << std::endl;

		//ContainerTimer systimer9;
		//systimer9.beginTiming();

		typedef CELL::PAIR::TPair< heap_value_t, size_t> HeapPair;
		typedef TMinHeapOfPairs< heap_value_t, size_t >::Type HeapOfPairs;
		HeapOfPairs heap_of_pairs;
		for (size_t i = 0; i < heap_size; ++i)
		{
			heap_of_pairs.push( HeapPair( values[i], i ) );
		}
		if ( heap_of_pairs.empty() )
			return false;
		if ( !heap_of_pairs.is_heap() )
			return false;

		//systimer9.endTiming();
		//systimer9.print("Test TMinHeapOfPairs", STREAM);
		if ( stream_results )
		{
			HeapOfPairs::iterator iter = heap_of_pairs.begin();
			HeapOfPairs::iterator iter_end = heap_of_pairs.end();
			HeapPair heap_pair;
			while (iter != iter_end) 
			{
				heap_pair = static_cast< HeapPair >( *iter );
				STREAM << "heap_pair: (";
				STREAM << to_string<double>(heap_pair.first) << ", " << to_string<size_t>(heap_pair.second);
				STREAM << "); " << std::endl;
				++iter;
			}
			STREAM << std::endl;
		}
		HeapOfPairs heap_of_pairs2;
		heap_of_pairs.swap(heap_of_pairs2);
		
		return true;
	}
//=============================================================================
	bool testMinIndexHeap( bool stream_results, const random_pool_type & TEST_VALUES )
	{
		typedef double heap_value_t;
		typedef TestType test_value_t;
		if ( stream_results )
		{
			STREAM << std::endl << "Test TMinIndexHeap" << std::endl;
			STREAM << "1; 2; 3; 6; 10; 5; 4; 7; 8; 15; 14; 11; 9; 13; 12; " << std::endl;
		}
		const heap_value_t radius = 1000.0;
		//const size_t size = 1024;
		const size_t size = 15;
		const size_t heap_size = size;
		const size_t array_size = heap_size;
		heap_value_t val[15] = {1, 2, 4, 7, 15, 5, 3, 6, 8, 14, 10, 11, 9, 13, 12 };
		typedef TPool<heap_value_t>::Type pool_type;
		pool_type values( val, val+15 );
		for (size_t i = 0; i < 15; ++i)
		{
			values[i] *= radius; 
		}
		if ( stream_results )
			STREAM << std::endl << "Test TMinIndexHeap: " << std::endl << std::endl;

		//ContainerTimer systimer10;
		//systimer10.beginTiming();

		typedef TMinBinaryIndexHeap< heap_value_t, size_t >::Type index_heap_type;
		index_heap_type index_heap( &values[0] );
		for (size_t i = 0; i < heap_size; ++i)
		{
			index_heap.push( i );
		}
		if ( index_heap.empty() )
			return false;
		if ( !index_heap.is_heap() )
			return false;

		//systimer10.endTiming();
		//systimer10.print("Test TMinIndexHeap", STREAM);

		//PRINT_ELEMENTS( index_heap );
		if ( stream_results )
		{
			index_heap_type::iterator iter1 = index_heap.begin();
			index_heap_type::iterator iter_end1 = index_heap.end();
			while (iter1 != iter_end1) 
			{
				STREAM << "indexed heap value: (";
				STREAM << to_string<size_t>( *iter1 );
				STREAM << ", " << to_string<size_t>(values[( *iter1 )]);
				STREAM << "); " << std::endl;
				++iter1;
			}
			STREAM << std::endl;
		}
		return true;
	}
//=============================================================================
