/*****************************************************************************/
/*!
@author			Stefan Marek
@description
http://www.geeksforgeeks.org/iterative-quick-sort/
https://www.geeksforgeeks.org/quickselect-algorithm/
http://www.sourcetricks.com/2011/06/quick-select.html
http://www.techiedelight.com/iterative-merge-sort-algorithm-bottom-up/
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __SORTING_TOOLS_H__
#define __SORTING_TOOLS_H__

#include "Definitions.h"
#include <cstddef> // ptrdiff_t, size_t

//#include <execution>
//#include <algorithm>

namespace SORT {

	//=============================================================================
	template <typename TValueType>
	INLINE void TSwap(TValueType & a, TValueType & b)
	{
		const TValueType temp = a;
		a = b;
		b = temp;
	}
	//=============================================================================
	template <typename TValueType, typename TComparator>
	INLINE int TPartition(TValueType arr[], int low, int high, TComparator comp)
	{
		const TValueType xval = arr[high];
		int i = (low - 1);
		for (int j = low; j < high; ++j)
		{
			if (comp(arr[j], xval))
			{
				++i;
				TSwap<TValueType>(arr[i], arr[j]);
			}
		}
		TSwap<TValueType>(arr[i + 1], arr[high]);
		return (i + 1);
	}
	//=============================================================================
	// https://www.geeksforgeeks.org/quickselect-a-simple-iterative-implementation/
	template <typename TValueType, typename TComparator>
	TValueType TQuickSelectIterative(TValueType arr[], int low, int high, int nth, TComparator comp)
	{
		int pivotIndex = 0;
		int nthIndex = nth - 1;
		while (low <= high) 
		{
			pivotIndex = TPartition<TValueType, TComparator>(arr, low, high, comp);
			if (pivotIndex == nthIndex)
			{
				return arr[pivotIndex];
			}
			else if (pivotIndex > nthIndex)
			{
				high = pivotIndex - 1;
			}
			else
			{
				low = pivotIndex + 1;
			}
		}
		return static_cast<TValueType>(0);
	}
	//=============================================================================
	// https://www.geeksforgeeks.org/iterative-quick-sort/
	struct QuickSortStackItem
	{
		int low;
		int high;
	};
	//template <typename TIteratorType, typename TComparator>
	//INLINE void TSortParallel(TIteratorType beginIter, TIteratorType endIter, TComparator comp)
	//{
	//	std::sort(std::execution::par_unseq, beginIter, endIter, comp);
	//}
	//template <typename TIteratorType>
	//INLINE void TSortParallel(TIteratorType beginIter, TIteratorType endIter)
	//{
	//	std::sort(std::execution::par_unseq, beginIter, endIter);
	//}
	template <typename TValueType, typename TComparator>
	void TQuickSortIterative(TValueType arr[], int low, int high, TComparator comp)
	{
		QuickSortStackItem * stack = new QuickSortStackItem[high - low + 1];
		int top = 0;
		int part = 0;
		QuickSortStackItem item;
		QuickSortStackItem item2;
		item.low = low;
		item.high = high;
		stack[0] = item;
		while (top >= 0)
		{
			item = stack[top];
			--top;
			part = TPartition<TValueType, TComparator>(arr, item.low, item.high, comp);
			if (part - 1 > item.low)
			{
				item2.low = item.low;
				item2.high = part - 1;
				++top;
				stack[top] = item2;
			}
			if (part + 1 < item.high)
			{
				item2.low = part + 1;
				item2.high = item.high;
				++top;
				stack[top] = item2;
			}
		}
		delete[] stack;
	}
	//=============================================================================
	template <typename TValueType, typename TComparator>
	void TMergeSortedArrays(TValueType arr1[], const size_t & n1,
		TValueType arr2[], const size_t & n2,
		TValueType arr3[], TComparator comp)
	{
		size_t i = 0;
		size_t j = 0;
		size_t k = 0;
		while ((i < n1) && (j < n2))
		{
			if (comp(arr1[i], arr2[j]))
			{
				arr3[k++] = arr1[i++];
			}
			else
			{
				arr3[k++] = arr2[j++];
			}
		}
		while (i < n1)
		{
			arr3[k++] = arr1[i++];
		}
		while (j < n2)
		{
			arr3[k++] = arr2[j++];
		}
	}
//=============================================================================
	// everything after iter can be erased from the object pool
	template <typename TIteratorType, typename TComparator>
	INLINE TIteratorType TUniqueSorted(TIteratorType first, TIteratorType last, TComparator comp)
	{
		TIteratorType iter = first;
		++first;
		while (first != last)
		{
			if (!comp((*iter), (*first)))
			{
				++iter;
				(*iter) = (*first);
			}
			++first;
		}
		++iter;
		return (iter);
	}
	template <typename TPoolType, typename TComparator>
	INLINE void TUniqueSortedPool(TPoolType & pool, TComparator comp)
	{
		typedef typename TPoolType::iterator iterator;
		iterator iter = TUniqueSorted<iterator, TComparator>(pool.begin(), pool.end(), comp);
		pool.erase(iter, pool.end());
	}
//=============================================================================
} // namespace SORT
//=============================================================================

#endif // __SORTING_TOOLS_H__ 
