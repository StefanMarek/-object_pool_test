/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __MATH_TOOLS_H__
#define __MATH_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include <cmath>

namespace CELL {

namespace TOOLS {

	template < typename TREAL >
	INLINE bool is_odd(const TREAL& value) THROW_NEVER
	{
		return ((value % static_cast<TREAL>(2.0)) != static_cast<TREAL>(0.0));
	}
	template < typename TREAL >
	INLINE bool is_even(const TREAL& value) THROW_NEVER
	{
		return ((value % static_cast<TREAL>(2.0)) == static_cast<TREAL>(0.0));
	}

//=============================================================================
//================================ Prime Numbers ==============================
//http://stackoverflow.com/questions/30052316/find-next-prime-number-algorithm
//=============================================================================
namespace PRIME { // namespace for protection against ADL
	//=============================================================================
	//template <typename TSizeType>
	//bool TIsPrime(const TSizeType number)
	//{
	//	if (number == 2 || number == 3)
	//	{
	//		return true;
	//	}
	//	if (number % 2 == 0 || number % 3 == 0)
	//	{
	//		return false;
	//	}
	//	TSizeType divisor = 6;
	//	// while (divisor - 1) ^ 2 <= n
	//	while (divisor * divisor - 2 * divisor + 1 <= number)
	//	{
	//		if (number % (divisor - 1) == 0)
	//		{
	//			return false;
	//		}
	//		if (number % (divisor + 1) == 0)
	//		{
	//			return false;
	//		}
	//		divisor += 6;
	//	}
	//	return true;
	//}
	//=============================================================================
	template <typename TSizeType>
	bool TIsPrime(TSizeType n)
	{
		if (n == 2 || n == 3)
		{
			return true;
		}
		if (n == 1 || n % 2 == 0)
		{
			return false;
		}
		for (TSizeType i = 3; i * i <= n; i += 2)
		{
			if (n % i == 0)
			{
				return false;
			}
		}
		return true;
	}
	//=============================================================================
	template <typename TSizeType>
	TSizeType TNextPrime(TSizeType n)
	{
		if (n % 2 == 0)
		{
			++n;
		}
		//for (; !TIsPrime(n); n += 2);
		while (!TIsPrime(n))
		{
			n += 2;
		}
		return n;
	}
	//=============================================================================
} // PRIME
//=============================================================================
//================================ LOG DUALIS =================================
//=============================================================================
namespace LOG2 { // namespace for protection against ADL
	//=============================================================================
	INLINE float log2_t(float value)
	{
		static const float RLOG2F_VALUE = 1.0f / ::log(2.0f);
		return (value ? ::log(value) * RLOG2F_VALUE : 0.0f);
	}
	//=============================================================================
	INLINE double log2_t(double value)
	{
		static const double RLOG2D_VALUE = 1.0 / ::log(2.0);
		return (value ? ::log(value) * RLOG2D_VALUE : 0.0);
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE log_dualis_int_ceil(TINT_TYPE value)
	{
		return static_cast<TINT_TYPE>(::ceil(log2_t(static_cast<double>(value))));
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE log_dualis_int_floor(TINT_TYPE value)
	{
		return static_cast<TINT_TYPE>(::floor(log2_t(static_cast<double>(value))));
	}
	//=============================================================================
} // LOG2
//=============================================================================
//================================ POWER OF TWO ===============================
//=============================================================================
namespace POW2 { // namespace for protection against ADL
	//size_t mpi1 = CELL::TOOLS::POW2::max_power_of_two_int<size_t>(127);
	//size_t npi1 = CELL::TOOLS::POW2::next_power_of_two_int<size_t>(127);
	//size_t fpi1 = CELL::TOOLS::POW2::find_power_of_two_int<size_t>(127);

	//size_t mpi2 = CELL::TOOLS::POW2::max_power_of_two_int<size_t>(128);
	//size_t npi2 = CELL::TOOLS::POW2::next_power_of_two_int<size_t>(128);
	//size_t fpi2 = CELL::TOOLS::POW2::find_power_of_two_int<size_t>(128);
	//=============================================================================
	INLINE float pow2_t(int value)
	{
		return static_cast<float>(::pow(2.0f, value));
	}
	//=============================================================================
	INLINE float pow2_t(float value)
	{
		return static_cast<float>(::pow(2.0f, value));
	}
	//=============================================================================
	INLINE double pow2_t(double value)
	{
		return ::pow(2.0, value);
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE bool is_power_of_two(const TINT_TYPE & value)
	{
		return (!(value & (value - static_cast<TINT_TYPE>(1))) && value);
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE power_of_two_int(TINT_TYPE value)
	{
		return static_cast<TINT_TYPE>(static_cast<TINT_TYPE>(1) << value);
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE find_power_of_two_int(TINT_TYPE value)
	{
		if (!is_power_of_two(value))
		{
			TINT_TYPE pow2int = 1;
			while (pow2int < value)
			{
				pow2int = pow2int << static_cast<TINT_TYPE>(1);
			}
			return (pow2int);
		}
		else
		{
			return (value);
		}
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE max_power_of_two_int(TINT_TYPE value)
	{
		return (power_of_two_int<TINT_TYPE>(LOG2::log_dualis_int_floor<TINT_TYPE>(value)));
	}
	//=============================================================================
	template <typename TINT_TYPE>
	INLINE TINT_TYPE next_power_of_two_int(TINT_TYPE value)
	{
		return (power_of_two_int<TINT_TYPE>(LOG2::log_dualis_int_floor<TINT_TYPE>(value) + 1));
	}
//=============================================================================
} // POW2
//=============================================================================
} // namespace TOOLS
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __MATH_TOOLS_H__ 
