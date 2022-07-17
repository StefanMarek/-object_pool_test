/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "Definitions.h"
#include "ErrorHandling.h"

//=============================================================================
#ifdef DEFAULT_OBJECT_ALLOCATOR
	#undef DEFAULT_OBJECT_ALLOCATOR
#endif
//=============================================================================
#ifdef DEFAULT_BUFFER_ALLOCATOR
	#undef DEFAULT_BUFFER_ALLOCATOR
#endif
//=============================================================================
#ifdef USE_STL_ALLOCATOR_AS_DEFAULT_OBJECT_ALLOCATOR
	#undef USE_STL_ALLOCATOR_AS_DEFAULT_OBJECT_ALLOCATOR
#endif
#define USE_STL_ALLOCATOR_AS_DEFAULT_OBJECT_ALLOCATOR
//=============================================================================
#ifdef USE_STL_ALLOCATOR_AS_DEFAULT_BUFFER_ALLOCATOR
	#undef USE_STL_ALLOCATOR_AS_DEFAULT_BUFFER_ALLOCATOR
#endif
#define USE_STL_ALLOCATOR_AS_DEFAULT_BUFFER_ALLOCATOR
//=============================================================================
#ifdef USE_STL_ALLOCATOR_AS_DEFAULT_OBJECT_ALLOCATOR
// I use std allocator for now because it uses _Allocate and _Deallocate which 
// make use of big allocation allignment calculations
	#include <memory> // allocator
	#define DEFAULT_OBJECT_ALLOCATOR std::allocator
#endif // USE_STL_ALLOCATOR_AS_DEFAULT_OBJECT_ALLOCATOR
//=============================================================================
#ifdef USE_STL_ALLOCATOR_AS_DEFAULT_BUFFER_ALLOCATOR
// I use std allocator for now because it uses _Allocate and _Deallocate which 
// make use of big allocation allignment calculations
	#include <memory> // allocator
	#define DEFAULT_BUFFER_ALLOCATOR std::allocator
#endif // USE_STL_ALLOCATOR_AS_DEFAULT_BUFFER_ALLOCATOR
//=============================================================================

#endif // __ALLOCATOR_H__
