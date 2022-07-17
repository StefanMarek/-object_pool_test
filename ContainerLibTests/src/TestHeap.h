/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	

*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __TEST_HEAP_H__
#define __TEST_HEAP_H__

#include "TestDefines.h"

bool testHeap1( bool stream_results, const random_pool_type & TEST_VALUES );
bool testHeap2( bool stream_results, const random_pool_type & TEST_VALUES );
bool testMinHeapOfPairs( bool stream_results, const random_pool_type & TEST_VALUES );
bool testMinIndexHeap( bool stream_results, const random_pool_type & TEST_VALUES );

#endif // __TEST_HEAP_H__
