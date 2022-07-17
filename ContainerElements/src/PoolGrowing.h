/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
					struct TArrayGStrat
					struct TVecGStrat
					struct TBinTreeGStrat
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __OBJECT_POOL_GROWING_H__
#define __OBJECT_POOL_GROWING_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "MathTools.h"

namespace CELL { 

namespace OPOOLGROW { 

//=============================================================================
	#ifdef DEFAULT_GROWING_STRATEGY
		#undef DEFAULT_GROWING_STRATEGY
	#endif
	#define DEFAULT_GROWING_STRATEGY OPOOLGROW::TPoolGStrat
	//#define DEFAULT_GROWING_STRATEGY OPOOLGROW::TArrayGStrat
	//#define DEFAULT_GROWING_STRATEGY OPOOLGROW::TVecGStrat
//=============================================================================

//=============================================================================
//=========================== ITGrowingStrategy ===============================
//=============================================================================
/*! @brief ITGrowingStrategy 
	ITGrowingStrategy is the interface for all template growing strategy types. 
	Any growing strategy type has to implement the methods initialize and next 
	with a size parameter. The new growing size has to be returned by both 
	methods. 
	NOTE:
	I won't inherit because of the virtual table (vtable) overhead. 
	But i will leave this interface class for documentation and usage purposes.
*/
//=============================================================================
	//template < typename TSizeType, size_t TBUFELEMS > 
	//struct ITGrowingStrategyPOD
	//{
	//	typedef TSizeType size_type;

	//	enum BUFFER : size_type
	//	{
	//		TELEMENTS = TBUFELEMS
	//	};
	//	enum INIT : size_type
	//	{
	//		TINIT_VALUE = 0
	//	};
	//	enum RETRY : size_type
	//	{
	//		TMAX_RETRY = 2
	//	};

	//	CONSTEXPR virtual ~ITGrowingStrategyPOD()
	//	{
	//	}
	//	// Calculates and returns the object pool size at the time of the pool 
	//	// construction.
	//	CONSTEXPR virtual size_type initialize( const size_type _size ) const = 0;
	//	// Calculates and returns the new object capacity whenever the capacity 
	//	// size will change.
	//	CONSTEXPR virtual size_type next(const size_type _size) const = 0;
	//	CONSTEXPR virtual size_type prev( const size_type _size ) const = 0;
	//};
	//template < typename TSizeType >
	//struct ITGrowingStrategy
	//{
	//	typedef TSizeType size_type;

	//	enum INIT : size_type
	//	{
	//		TINIT_VALUE = 0
	//	};
	//	enum RETRY : size_type
	//	{
	//		TMAX_RETRY = 2
	//	};

	//	virtual ~ITGrowingStrategy()
	//	{
	//	}
	//	// Calculates and returns the object pool size at the time of the pool 
	//	// construction.
	//	CONSTEXPR virtual size_type initialize(const size_type _size) const = 0;
	//	// Calculates and returns the new object capacity whenever the capacity 
	//	// size will change.
	//	CONSTEXPR virtual size_type next(const size_type _size) const = 0;
	//	CONSTEXPR virtual size_type prev(const size_type _size) const = 0;
	//};
//=============================================================================
//=============================== TStaticGStrat ===============================
//=============================================================================
/*! @brief TStaticGStrat 
	This growing strategy allows no growing of the object pool at all.
	The object pool capacity will not grow because the next element size 
	will be the same size as at time of the object pool construction. 
	Therefore this growing strategy forces the pool to have a constant size.
*/
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TStaticGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize( const size_type _size ) const
		{
			return ( _size );
		}
		CONSTEXPR size_type next( const size_type _size ) const
		{
			return ( _size );
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			return (_size);
		}
	};
	template < typename TSizeType >
	struct TStaticGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			return (_size);
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			return (_size);
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			return (_size);
		}
	};
//=============================================================================
//=========================== Object Pool Growing Strategy ====================
//=============================================================================
/*! @brief grow_object_pool 
	Template parameter TSizeType has to be an integer type.
	Returns the growing factor.

	The growing factor is given by the simple formula: 
	value / adapted_log_dualis(value) 
	in mathematical terms: 
	value / ( ( log_dualis(value)  / TRANGE ) + TRANGE_ADD ) 
	or optimized:   
	value >> ( ( log_dualis(value) >> TRANGE ) + TRANGE_ADD ) 

	The following three ways of calculating the growing of the pool 
	will result in an identical solution:
	1. Case differentiation
	static const double rlog2 = 1.0 / std::log(2.0);
	const TSizeType log_dualis_n = 
		static_cast<TSizeType>( std::log( n ) * rlog2 );
	switch ( log_dualis_n )
	{
		case 0:  case 1:  case 2:  case 3:  case 4:  case 5:  case 6:  case 7: 
			return ( n >> 0 ); // n / 1 ==> exponential growing in the beginning
		case 8:  case 9: case 10: case 11: case 12: case 13: case 14: case 15: 
			return ( n >> 1 ); // n / 2
		case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: 
			return ( n >> 2 ); // n / 4
		case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: 
			return ( n >> 3 ); // n / 8
		case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39:
			return ( n >> 4 ); // n / 16
		default:
			return ( n >> 5 ); // n / 32
	}
	2. Plain mathematical equation based on the case differentiation.
	The parameter n can be an arbitrary values.
	static const double rlog2 = 1.0 / std::log(2.0);
	return ( n / 
		( ( static_cast<TSizeType>( std::log( n ) * rlog2 ) / TRANGE ) + TRANGE_ADD )  
	) ;
	3. Optimized version of second way: 
	The parameter n has to be a power of two value.
	return ( n >> 
		( ( bit_scan_reverse< TSizeType >( n ) >> TRANGE ) + TRANGE_ADD ) );

	As you can see the strategy is to divide the given value n by an adapted 
	logarithm dualis of the value.
	The adapted logarithm dualis represents a class of n-logarithm dualis 
	values. This is basically the same strategy as it is used to identify 
	in which mudulo class a value belongs. 
	The term TRANGE_ADD can be used to advance the divisor. If TRANGE_ADD is 
	zero the first range class will have exponential growing, because 
	it will return value. 
	If you want to change the strategy than you can adjust the 
	value TRANGE to your prefered case differentiation.
	Or you can use the value RANGE_ADD to advance the divisor. 
*/
//=============================================================================
	template 
	< 
		typename TSizeType, 
		TSizeType TRANGE,
		TSizeType TRANGE_ADD
	> 
	INLINE TSizeType 
	grow_object_pool( const TSizeType n ) 
	{
		// 3. Optimized version of second way:
		//return 
		//( n >> ( 
		//	( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( n ) >> TRANGE ) + TRANGE_ADD 
		//) );
		//n >> ( static_cast< TSizeType >(
		//CELL::TOOLS::BIT_SCAN::bit_scan_reverse_shift_right( n, TRANGE ) ) + 
		//TRANGE_ADD ) 
		//return ( n >> ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( n ) >> TRANGE ) );
		//static const float RANGE_SHIFT = 1.0f / static_cast<float>( TRANGE );
		const TSizeType range_class = 
			( CELL::TOOLS::LOG2::log_dualis_int_floor< TSizeType >( n ) / TRANGE ) + TRANGE_ADD;
		return ( n >> range_class );
	}
//=============================================================================
//=========================== TExponentialGrowingStrategy =====================
//=============================================================================
/*! @brief TPoolGStrat
This growing strategy calculates an growing series of the form
ni+1 = ni * 2.
Fast growing strategy, greedy memory consumption.
Hint: Beat the devil
Greediest growing strategy but very few pool reallocations.
This strategy should beat the vector.
*/
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TPoolGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize( const size_type _size ) const
		{
			return ( _size );
		}
		CONSTEXPR size_type next( const size_type _size ) const
		{
			// exponential growing
			return ( _size << static_cast<size_type>(1) );
		}
		CONSTEXPR size_type prev( const size_type _size ) const
		{
			// exponential growing
			return ( _size >> static_cast<size_type>(1) );
		}
	};
	template < typename TSizeType >
	struct TPoolGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			return (_size);
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			// exponential growing
			return (_size << static_cast<size_type>(1));
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			// exponential growing
			return (_size >> static_cast<size_type>(1));
		}
	};
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TStringGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			if (_size > 0)
			{
				return (_size + static_cast<size_type>(1));
			}
			else
			{
				return (0);
			}
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			// exponential growing
			return (_size << static_cast<size_type>(1));
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			// exponential growing
			return (_size >> static_cast<size_type>(1));
		}
	};
	template < typename TSizeType >
	struct TStringGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			if (_size > 0)
			{
				return (_size + static_cast<size_type>(1));
			}
			else
			{
				return (0);
			}
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			// exponential growing
			return (_size << static_cast<size_type>(1));
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			// exponential growing
			return (_size >> static_cast<size_type>(1));
		}
	};
//=============================================================================
//=============================== TArrayGStrat ================================
//=============================================================================
/*! @brief TArrayGStrat 
	This growing strategy calculates an growing series of the form 
	ni+1 = ni + grow_object_pool( ni ).
	Default values are TRANGE = 3 and TRANGE_ADD = 0 for the optimized version.
	Which means that one class has 18 log dualis values.
	Hint: 
	Moderate growing strategy. Similar to STL vector with small object sizes 
	but will get better with large object sizes. 
*/
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TArrayGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize( const size_type _size ) const
		{
			return ( _size );
		}
		CONSTEXPR size_type next( const size_type _size ) const
		{
  			// n + n/2 until 2^16, n + n/4 until 2^32, 
			// n + n/8 until 2^48 ...  
			//return ( _size + grow_object_pool< size_type, 16, 1 >( _size ) );
  			// exponential until 2^16, n + n/2 until 2^32, 
			// n + n/4 until 2^48 ...  
			return ( _size + grow_object_pool< size_type, 16, 0 >( _size ) );
		}
		CONSTEXPR size_type prev( const size_type _size ) const
		{
  			// n + n/2 until 2^16, n + n/4 until 2^32, 
			// n + n/8 until 2^48 ...  
			//return ( _size + grow_object_pool< size_type, 16, 1 >( _size ) );
  			// exponential until 2^16, n + n/2 until 2^32, 
			// n + n/4 until 2^48 ...  
			return ( _size - grow_object_pool< size_type, 16, 0 >( _size ) );
		}
	};
	template < typename TSizeType >
	struct TArrayGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			return (_size);
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			// n + n/2 until 2^16, n + n/4 until 2^32, 
			// n + n/8 until 2^48 ...  
			//return ( _size + grow_object_pool< size_type, 16, 1 >( _size ) );
			// exponential until 2^16, n + n/2 until 2^32, 
			// n + n/4 until 2^48 ...  
			return (_size + grow_object_pool< size_type, 16, 0 >(_size));
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			// n + n/2 until 2^16, n + n/4 until 2^32, 
			// n + n/8 until 2^48 ...  
			//return ( _size + grow_object_pool< size_type, 16, 1 >( _size ) );
			// exponential until 2^16, n + n/2 until 2^32, 
			// n + n/4 until 2^48 ...  
			return (_size - grow_object_pool< size_type, 16, 0 >(_size));
		}
	};
//=============================================================================
//=============================== TVecGStrat ==================================
//=============================================================================
/*! @brief TVecGStrat 
	This growing strategy calculates an growing series of the form 
	ni+1 = ni + (ni / 2).
	Hint: 
	STL vector growing strategy.
*/
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TVecGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize( const size_type _size ) const
		{
			// start with at least one element
			return ( _size );//? _size : 1 );
		}
		CONSTEXPR size_type next( const size_type _size ) const
		{
			// return capacity sized by half of the element size
			//return ( _size + grow_vector< size_type >( _size ) );
			//const size_type grow_size = (_size >> 1);
			return ( _size + (_size >> static_cast<size_type>(1)) );
			//return _size + _size / 2;
		}
		CONSTEXPR size_type prev( const size_type _size ) const
		{
			// return capacity sized by half of the element size
			//return ( _size + grow_vector< size_type >( _size ) );
			//const size_type grow_size = (_size >> 1);
			return ( _size >> static_cast<size_type>(1));
			//return _size - _size / 2;
		}
	};
	template < typename TSizeType >
	struct TVecGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			// start with at least one element
			return (_size);//? _size : 1 );
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			// return capacity sized by half of the element size
			//return ( _size + grow_vector< size_type >( _size ) );
			//const size_type grow_size = (_size >> 1);
			return (_size + (_size >> static_cast<size_type>(1)));
			//return _size + _size / 2;
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			// return capacity sized by half of the element size
			//return ( _size + grow_vector< size_type >( _size ) );
			//const size_type grow_size = (_size >> 1);
			return (_size >> static_cast<size_type>(1));
			//return _size - _size / 2;
		}
	};
//=============================================================================
//=============================== TBinTreeGStrat ==============================
//=============================================================================
/*! @brief BinaryTreeGrowingStrategy 
	This growing strategy calculates an growing series of the form 
	ni+1 = grow_binary_tree(ni). 
	Based on a given element index it grows by the nearest full level of 
	a binary tree. 
	Hint: 
	Exponential growing strategy which is only usefull for binary tree 
	based datastructures which tend to fill a complete level before they 
	start a new one. See implicit datastructures.
	Warning: 
	One element at a new level will allocate the whole new binary tree level.
	(Sick)
*/
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TBinTreeGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize( const size_type _size ) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ? _size : 1 ) + 1 ) );
			// start with at least 2 elements: 
			//return ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) );
			return (_size);
		}
		CONSTEXPR size_type next( const size_type _size ) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) + 1 ) ) + 1;
			//return ( ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) ) + 1 );
			//const size_type grow_size = (_size << 1);
			return ( (_size << static_cast<size_type>(1)) + static_cast<size_type>(1));
			//return _size * 2 + 1;
		}
		CONSTEXPR size_type prev( const size_type _size ) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) + 1 ) ) + 1;
			//return ( ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) ) + 1 );
			//const size_type grow_size = (_size << 1);
			return ( (_size - static_cast<size_type>(1)) << static_cast<size_type>(1)) + static_cast<size_type>(1);
		}
	};
	template < typename TSizeType >
	struct TBinTreeGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR size_type initialize(const size_type _size) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ? _size : 1 ) + 1 ) );
			// start with at least 2 elements: 
			//return ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) );
			return (_size);
		}
		CONSTEXPR size_type next(const size_type _size) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) + 1 ) ) + 1;
			//return ( ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) ) + 1 );
			//const size_type grow_size = (_size << 1);
			return ((_size << static_cast<size_type>(1)) + static_cast<size_type>(1));
			//return _size * 2 + 1;
		}
		CONSTEXPR size_type prev(const size_type _size) const
		{
			//return grow_binary_tree< size_type >( _size );
			//return ( 1 << ( CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) + 1 ) ) + 1;
			//return ( ( 1 << CELL::TOOLS::TOOLS::log_dualis_int_floor< TSizeType >( _size ) ) + 1 );
			//const size_type grow_size = (_size << 1);
			return ((_size - static_cast<size_type>(1)) << static_cast<size_type>(1)) + static_cast<size_type>(1);
		}
	};
//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TPrimeGStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR TSizeType initialize(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size;// << 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType next(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size << static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType prev(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size >> static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		//TSizeType maxRetry() const
		//{
		//	return 5;
		//}
	};
	template < typename TSizeType >
	struct TPrimeGStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};

		CONSTEXPR TSizeType initialize(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size;// << 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType next(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size << 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType prev(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size >> 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::PRIME::TNextPrime(newSize);
			}
			return nextSize;
		}
		//TSizeType maxRetry() const
		//{
		//	return 5;
		//}
	};
	//=============================================================================
	template < typename TSizeType, size_t TBUFELEMS >
	struct TPO2GStratPOD
	{
		typedef TSizeType size_type;

		enum BUFFER : size_type
		{
			TELEMENTS = TBUFELEMS
		};
		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};
		CONSTEXPR TSizeType initialize(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size;// << 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType next(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size << static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType prev(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size >> static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
	};
	template < typename TSizeType >
	struct TPO2GStrat
	{
		typedef TSizeType size_type;

		enum INIT : size_type
		{
			TINIT_VALUE = 0
		};
		enum RETRY : size_type
		{
			TMAX_RETRY = 0
		};
		CONSTEXPR TSizeType initialize(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size;// << 1;
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType next(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size << static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
		CONSTEXPR TSizeType prev(const TSizeType _size) const
		{
			size_type nextSize = INIT::TINIT_VALUE;
			const TSizeType newSize = _size >> static_cast<size_type>(1);
			if (newSize > INIT::TINIT_VALUE)
			{
				nextSize = CELL::TOOLS::POW2::find_power_of_two_int(newSize);
			}
			return nextSize;
		}
	};
//=============================================================================
} // namespace OPOOLGROW
//=============================================================================
} // namespace CELL
//=============================================================================

#endif // __OBJECT_POOL_GROWING_H__
