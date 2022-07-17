/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description   
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __CONTAINER_TEST_PROJECT_H__
#define __CONTAINER_TEST_PROJECT_H__

#include "TestPool.h"
#include "TestHash.h"

#include <cstddef> // ptrdiff_t, size_t
#include <random> 

template <typename TStringType>
void calcClusteredData(typename CELL::OPOOL::TPool<TStringType>::Type& data)
{
	const char length = 'e';
	const char start = 'a';
	TStringType sa;
	TStringType sb;
	TStringType sc;
	TStringType sd;
	TStringType se;
	TStringType str;
	for (char i = start; i < length; i++)
	{
		sa = { (i) };
		for (char j = start; j < length; j++)
		{
			sb = { (j) };
			for (char k = start; k < length; k++)
			{
				sc = { (k) };
				for (char l = start; l < length; l++)
				{
					sd = { (l) };
					for (char m = start; m < length; m++)
					{
						se = { (m) };
						str = sa + sb + sc + sd + se;
						data.push_back(str);
					}
				}
			}
		}
	}
}
void init_clustered_data_f(CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type& new_clustered_data,
	CELL::OPOOL::TPool<std::string>::Type& new_clustered_data_std);

bool testString(bool stream_results, const random_pool_type& TEST_VALUES);

int main(int argc, char * argv[]);

#endif // __CONTAINER_TEST_PROJECT_H__
