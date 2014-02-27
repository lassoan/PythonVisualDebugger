//=============================================================================
//
//	Filename:	  hestring.h
//	Contributors: 
//	Notes:		  
//
//	Copyright© 2002 Humongous Entertainment
//
//	All rights reserved. Copying or other reproduction except for archival
//	purposes is prohibited without prior written consent.
//
//=============================================================================

#ifndef __HESTRING_H__
#define __HESTRING_H__

#if _MSC_VER >= 1000
#pragma once
#endif /* _MSC_VER >= 1000 */

//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

#include <stdarg.h>

namespace HumongousEntertainment
{

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Type Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Structure Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Class Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Extern Data
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Extern Prototypes
//-----------------------------------------------------------------------------

// The regular strcpy and strcat functions are dangerous because they don't
// check the size of the destination and can cause memory corruption and crashes.
// strncpy and strncat are theoretically 'safe' but are very difficult to use
// properly.
// strlcpy and strlcat are recommended replacements, standardized by OpenBSD.
// See http://www.rocketaware.com/man/man3/strlcpy.3.htm or the .cpp file for
// more information.
// If there is not enough room these functions will truncate as needed, but the
// result will always be null terminated.
size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t size);

// These non-standard variants are just like strlcpy and strlcat except that
// if there is not enough room they trigger an assert.
// These versions should be preferred anytime your code requires the copy to
// succeed.
size_t strlcpyAssert(char *dst, const char *src, size_t size);
size_t strlcatAssert(char *dst, const char *src, size_t size);

// If it is really important to detect when these string functions fail it will
// be necessary to write custom variants that print error messages and then exit,
// or hook into these functions to do this.

// Other good options are these utility functions:
// std::string HECORECALLCONV stringprintf(char const *format, ...);
// std::string HECORECALLCONV vstringprintf(const char* format, va_list args);
// declared in hecore.h

// These non-standard variants are just like snprintf and vsnprintf except that
// if there is not enough room they trigger an assert.
// These versions should be preferred anytime your code requires the copy to
// succeed.
int snprintfAssert(char *buffer, size_t count, const char *format, ...);
int vsnprintfAssert(char *buffer, size_t count, const char *format, va_list args);

//-----------------------------------------------------------------------------
//	Inlines
//-----------------------------------------------------------------------------

}	// End namespace HumongousEntertainment

// snprintf is a BSD/C99x function that is therefore implicitly part of the C++
// standard. However, VC++ does not implement it, and it's implementation of
// _snprintf is dangerously different. _snprintf is like strncpy in that it
// doesn't promise to null-terminate the result. snprintf does.
// Ditto with vsnprintf and _vsnprintf.
// These implementations of snprintf and vsnprintf behave properly and will
// always null-terminate the result.
// In non-VC++ platforms these should be defined already.
#ifdef	_MSC_VER
	// _snprintf does *not* behave like snprintf, so we should not pretend that one
	// is the other.
	#undef	snprintf
	#undef	vsnprintf
	int snprintf(char *buffer, size_t count, const char *format, ...);
	//int vsnprintf(char *buffer, size_t count, const char *format, va_list args);
#endif

#endif // !defined(__HESTRING_H__)
