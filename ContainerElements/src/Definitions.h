/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	This file is only included in Global.h. 
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __GLOBAL_DEFINITIONS_H__
#define __GLOBAL_DEFINITIONS_H__

//=============================================================================
//=========================== GLOBAL ENABLE / DISABLE =========================
//=============================================================================
// The following enable / disable macro defines should be enabled or disabled 
// in the compiler settings or before the user includes the lib into a project.  
//=============================================================================
#ifndef USE_DATA_PACKING
	#define USE_DATA_PACKING
#endif
//=============================================================================
#ifndef USE_INITIALIZER_LIST
	#define USE_INITIALIZER_LIST
#endif
//=============================================================================
//=============================================================================
//=============================================================================
#ifndef CONSTEXPR
#define CONSTEXPR constexpr
// #define CONSTEXPR 
#endif
//=========================================================================
#ifndef CONSTEXPRDTOR
// #define CONSTEXPRDTOR constexpr
#define CONSTEXPRDTOR 
#endif
//=========================================================================
#ifndef CONSTEXPRDTOR_VIRTUAL
// #define CONSTEXPRDTOR_VIRTUAL constexpr
#define CONSTEXPRDTOR_VIRTUAL 
#endif
//=========================================================================
#ifndef THROW_NEVER
#define THROW_NEVER noexcept
#endif
//=============================================================================
#ifdef _MSC_VER
//=============================================================================
	#ifndef NOOP
		#define NOOP __noop
	#endif
	//=========================================================================
	#ifndef INLINE
		#define INLINE __inline
	#endif	
	//=========================================================================
	#ifndef FORCEINLINE
		#define FORCEINLINE __forceinline
	#endif
//=========================================================================
#else
//=========================================================================
	#ifndef NOOP
		#define NOOP 
	#endif
	//=========================================================================
	#ifndef INLINE
		#define INLINE inline
	#endif	
	//=========================================================================
	#ifndef FORCEINLINE
		#define FORCEINLINE inline
	#endif
//=============================================================================
#endif // _MSC_VER
//=============================================================================
#ifdef _CRT_PACKING
	#define CELL_CRT_PACKING _CRT_PACKING 
#else
	#define CELL_CRT_PACKING 8
#endif
//=============================================================================
//========================== Data Allignment ==================================
//=============================================================================
// http://msdn.microsoft.com/en-us/library/2e70t5y1(VS.80).aspx
//=============================================================================
// CRT_PACKING forces the allocation of any datatype to eight byte alignment.
// Data types will be allocated according to the given _CRT_PACKING value. 
// Which will be 8 byte in MS VC compiler on 32 bit operating systems. 
//=========================================================================
#ifndef PRAGMA_PACK_PUSH_CRT_PACKING
	//#define PRAGMA_PACK_PUSH_CRT_PACKING __pragma( pack(push, CELL_CRT_PACKING ) )
	#define PRAGMA_PACK_PUSH_CRT_PACKING pack(push, CELL_CRT_PACKING )
#endif  // PRAGMA_PACK_PUSH_CRT_PACKING
//=========================================================================
// pop restores the previously pushed packing information.
#ifndef PRAGMA_PACK_POP
	#ifdef USE_DATA_PACKING
		#define PRAGMA_PACK_POP	pack(pop) 		
	#else
		#define PRAGMA_PACK_POP 
	#endif // USE_DATA_PACKING
#endif  // PRAGMA_PACK_POP
//=========================================================================

#endif //__GLOBAL_DEFINITIONS_H__
