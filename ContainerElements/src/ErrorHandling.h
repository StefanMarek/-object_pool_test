/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	There is no additional header included in this file. 
					So if you want to use the macros you have to include the 
					files first. ( crtdbg.h or cassert )

*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __ERROR_HANDLING_H__
#define __ERROR_HANDLING_H__

//=============================================================================
//=========================== GLOBAL ENABLE / DISABLE =========================
//=============================================================================
// The following enable / disable macro defines should be enabled or disabled 
// in the compiler settings or before the user includes the lib into a project.  
//=============================================================================
	#ifndef USE_STD_ASSERT
		#define USE_STD_ASSERT
	#endif
//=============================================================================
	#ifndef USE_DISABLE_WARNINGS
		#define USE_DISABLE_WARNINGS
	#endif
//=============================================================================
//========================== Disable Warnings =================================
//=============================================================================
	#ifndef UNREFERENCED_PARAMETER // from winnt.h
		#define UNREFERENCED_PARAMETER(P)          (P)
	#endif
//=============================================================================
#ifdef _MSC_VER //&& defined( USE_DISABLE_WARNINGS )
	//=========================================================================
	#ifndef PRAGMA_WARNING_PUSH_LEVEL3
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_PUSH_LEVEL3 warning(push, 3 )
		#else
			#define PRAGMA_WARNING_PUSH_LEVEL3 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_PUSH_LEVEL3
	//=========================================================================
	#ifndef PRAGMA_WARNING_PUSH_LEVEL4
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_PUSH_LEVEL4 warning(push, 4 )
		#else
			#define PRAGMA_WARNING_PUSH_LEVEL4 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_PUSH_LEVEL4
	//=========================================================================
	#ifndef PRAGMA_WARNING_POP
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_POP warning(pop)
		#else
			#define PRAGMA_WARNING_POP 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_POP
	//=========================================================================
	// Compiler Warning (level 3) C4996: 
	// This function or variable may be unsafe. 
	// To disable deprecation, use _CRT_SECURE_NO_WARNINGS. 
	#ifndef PRAGMA_WARNING_DISABLE4996
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DISABLE4996 warning(disable: 4996 )
		#else
			#define PRAGMA_WARNING_DISABLE4996 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DISABLE4996
	#ifndef PRAGMA_WARNING_DEFAULT4996
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DEFAULT4996 warning(default: 4996 )
		#else
			#define PRAGMA_WARNING_DEFAULT4996 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DEFAULT4996
	//=========================================================================
	// Compiler Warning (level 1) C4503: 
	// decorated name length exceeded, name was truncated
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(C4503)&rd=true
	//#ifndef PRAGMA_WARNING_DISABLE4503
	//	#ifdef USE_DISABLE_WARNINGS
	//		#define PRAGMA_WARNING_DISABLE4503 warning(disable: 4503 )
	//	#else
	//		#define PRAGMA_WARNING_DISABLE4503 
	//	#endif // USE_DISABLE_WARNINGS
	//#endif // PRAGMA_WARNING_DISABLE4503
	//#ifndef PRAGMA_WARNING_DEFAULT4503
	//	#ifdef USE_DISABLE_WARNINGS
	//		#define PRAGMA_WARNING_DEFAULT4503 warning(default: 4503 )
	//	#else
	//		#define PRAGMA_WARNING_DEFAULT4503
	//	#endif // USE_DISABLE_WARNINGS
	//#endif // PRAGMA_WARNING_DEFAULT4503
	//=========================================================================
	// Compiler Warning (level 4) C4100: 
	// unreferenced formal parameter. 
	#ifndef PRAGMA_WARNING_DISABLE4100
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DISABLE4100 warning(disable: 4100 )
		#else
			#define PRAGMA_WARNING_DISABLE4100 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DISABLE4100
	#ifndef PRAGMA_WARNING_DEFAULT4100
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DEFAULT4100 warning(default: 4100 )
		#else
			#define PRAGMA_WARNING_DEFAULT4100 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DEFAULT4100
	//=========================================================================
	// Compiler Warning (level 4) C4201: 
	// nonstandard extension used : nameless struct/union
	//#pragma warning(disable : 4201) 
	#ifndef PRAGMA_WARNING_DISABLE4201
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DISABLE4201 warning(disable: 4201 )
		#else
			#define PRAGMA_WARNING_DISABLE4201 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DISABLE4201
	#ifndef PRAGMA_WARNING_DEFAULT4201
		#ifdef USE_DISABLE_WARNINGS
			#define PRAGMA_WARNING_DEFAULT4201 warning(default: 4201 )
		#else
			#define PRAGMA_WARNING_DEFAULT4201 
		#endif // USE_DISABLE_WARNINGS
	#endif // PRAGMA_WARNING_DEFAULT4201
	//=========================================================================	
	// Compiler Warning (level 4) C4127
	// conditional expression is constant
	//#pragma warning(disable : 4127) 
	//#ifndef PRAGMA_WARNING_DISABLE4127
	//	#ifdef USE_DISABLE_WARNINGS
	//		#define PRAGMA_WARNING_DISABLE4127 warning(disable: 4127 )
	//	#else
	//		#define PRAGMA_WARNING_DISABLE4127 
	//	#endif // USE_DISABLE_WARNINGS
	//#endif // PRAGMA_WARNING_DISABLE4127
	//#ifndef PRAGMA_WARNING_DEFAULT4127
	//	#ifdef USE_DISABLE_WARNINGS
	//		#define PRAGMA_WARNING_DEFAULT4127 warning(default: 4127 )
	//	#else
	//		#define PRAGMA_WARNING_DEFAULT4127 
	//	#endif // USE_DISABLE_WARNINGS
	//#endif // PRAGMA_WARNING_DEFAULT4127
	//=========================================================================
#endif // _MSC_VER
//=============================================================================
//============================ __DEBUG_AT_STRING__ ============================
//=============================================================================
	#ifdef STRINGIFY
		#undef STRINGIFY
	#endif
	#define STRINGIFY(EXPRESSION) #EXPRESSION
	//=========================================================================
	#ifdef TOSTRING
		#undef TOSTRING
	#endif
	#define TOSTRING(EXPRESSION) STRINGIFY(EXPRESSION)
//=============================================================================
//======================= DEBUG FUNCTIONS DEFINITIONS =========================
//=============================================================================
//#if defined(USE_STD_ASSERT)
//	#include <assert.h> // assert
//#endif
//=============================================================================
	#ifdef GLOBAL_ASSERT_FUNCTION
		#undef GLOBAL_ASSERT_FUNCTION
	#endif 
	#if defined(USE_STD_ASSERT)
		#define GLOBAL_ASSERT_FUNCTION assert
	#else 
		#define GLOBAL_ASSERT_FUNCTION 
	#endif // _MSC_VER
//=============================================================================
	#ifdef GLOBAL_ASSERT
		#undef GLOBAL_ASSERT
	#endif 
	#define GLOBAL_ASSERT(EXPRESSION) GLOBAL_ASSERT_FUNCTION(EXPRESSION)
//=============================================================================
	#ifdef GLOBAL_STATIC_ASSERT_FUNCTION
		#undef GLOBAL_STATIC_ASSERT_FUNCTION
	#endif 
	#if _DEBUG
		#define GLOBAL_STATIC_ASSERT_FUNCTION static_assert
	#else 
		#define GLOBAL_STATIC_ASSERT_FUNCTION
	#endif // _MSC_VER
//=============================================================================
	#ifdef GLOBAL_STATIC_ASSERT
		#undef GLOBAL_STATIC_ASSERT
	#endif 
	#define GLOBAL_STATIC_ASSERT(EXPRESSION, MESSAGE) GLOBAL_STATIC_ASSERT_FUNCTION(EXPRESSION, MESSAGE)
//=============================================================================
//=============================== HANDLE ERRORS ===============================
//=============================================================================
	#ifdef GLOBAL_ASSERT_NOT_INITIALIZED
		#undef GLOBAL_ASSERT_NOT_INITIALIZED
	#endif
	#define GLOBAL_ASSERT_NOT_INITIALIZED(element) \
		GLOBAL_ASSERT( nullptr != element && "pointer is not initialized: " TOSTRING(element) )
//=============================================================================
	#ifdef GLOBAL_ASSERT_INDEX_OUT_OF_RANGE
		#undef GLOBAL_ASSERT_INDEX_OUT_OF_RANGE
	#endif
	#define GLOBAL_ASSERT_INDEX_OUT_OF_RANGE(index, range) \
		GLOBAL_ASSERT( index < range && "index: " TOSTRING(index) " is out of range: " TOSTRING(range) );
//=============================================================================
	#ifdef GLOBAL_ASSERT_RANGE_ERROR
		#undef GLOBAL_ASSERT_RANGE_ERROR
	#endif
	#define GLOBAL_ASSERT_RANGE_ERROR(first, last) \
		GLOBAL_ASSERT( first <= last && "first: " TOSTRING(first) " is greater than or equals: " TOSTRING(last) );
//=============================================================================

#endif // __ERROR_HANDLING_H__ 
