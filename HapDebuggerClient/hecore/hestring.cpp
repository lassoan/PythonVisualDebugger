//=============================================================================
//
//	Filename:	  hestring.cpp
//	Contributors: 
//	Notes:		  
//
//	Copyright© 2002 Humongous Entertainment
//
//	All rights reserved. Copying or other reproduction except for archival
//	purposes is prohibited without prior written consent.
//
//=============================================================================

//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

#include "priv_coreprecompiled.h"
#include "hestring.h"

// Make these functions available - in CodeWarrior 1.3.2 they are in the
// std namespace.
#ifdef	VK_CUBE
	using std::strncpy;
	using std::strlen;
#endif

//-----------------------------------------------------------------------------
//	Global Data
//-----------------------------------------------------------------------------


namespace { // anonymous namespace restricts to translation unit

//-----------------------------------------------------------------------------
//	Local Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Local Type Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Local Structure Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Local Class Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Local Data
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Local Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Begin Local Code
//-----------------------------------------------------------------------------


} // end anonymous namespace

//-----------------------------------------------------------------------------
//	Begin Extern Code
//-----------------------------------------------------------------------------

/*
Documentation of the BSD functions strlcpy and strlcat from:
	http://www.rocketaware.com/man/man3/strlcpy.3.htm
  
NAME
     strlcpy, strlcat - size-bounded string copying and concatenation



SYNOPSIS
     #include <string.h>

     size_t
     strlcpy(char *dst, const char *src, size_t size);

     size_t
     strlcat(char *dst, const char *src, size_t size);

DESCRIPTION
     The strlcpy() and strlcat() functions copy and concatenate strings re-
     spectively.  They are designed to be safer, more consistent, and less er-
     ror prone replacements for strncpy(3) and strncat(3).  Unlike those func-
     tions, strlcpy() and strlcat() take the full size of the buffer (not just
     the length) and guarantee to NUL-terminate the result (as long as size is
     larger than 0).  Note that you should include a byte for the NUL in size.

     The strlcpy() function copies up to size - 1 characters from the NUL-ter-
     minated string src to dst, NUL-terminating the result.

     The strlcat() function appends the NUL-terminated string src to the end
     of dst. It will append at most size - strlen(dst) - 1 bytes, NUL-termi-
     nating the result.

RETURN VALUES
     The strlcpy() and strlcat() functions return the total length of the
     string they tried to create.  For strlcpy() that means the length of src.
     For strlcat() that means the initial length of dst plus the length of
     src. While this may seem somewhat confusing it was done to make trunca-
     tion detection simple.

EXAMPLES
     The following code fragment illustrates the simple case:

           char *s, *p, buf[BUFSIZ];

           ...

           (void)strlcpy(buf, s, sizeof(buf));
           (void)strlcat(buf, p, sizeof(buf));

     To detect truncation, perhaps while building a pathname, something like
     the following might be used:

           char *dir, *file, pname[MAXPATHNAMELEN];

           ...

           if (strlcpy(pname, dir, sizeof(pname)) >= sizeof(pname))
                   goto toolong;
           if (strlcat(pname, file, sizeof(pname)) >= sizeof(pname))
                   goto toolong;

     Since we know how many characters we copied the first time, we can speed
     things up a bit by using a copy instead on an append:

           char *dir, *file, pname[MAXPATHNAMELEN];
           size_t n;

           ...

           n = strlcpy(pname, dir, sizeof(pname));
           if (n >= sizeof(pname))
                   goto toolong;
           if (strlcpy(pname + n, file, sizeof(pname) - n) >= sizeof(pname) - n)
                   goto toolong;

     However, one may question the validity of such optimizations, as they de-
     feat the whole purpose of strlcpy() and strlcat().  As a matter of fact,
     the first version of this manual page got it wrong.
*/

namespace HumongousEntertainment
{

size_t strlcpy(char *dst, const char *src, size_t size)
{
	// Passing a size of zero is not really valid, since it makes it impossible
	// to null-terminate the string.
	assert(size >= 0);
	// Make sure we handle a size of zero properly - can't null terminate the string.
	if (size <= 0)
		return 0;
	strncpy(dst, src, size);
	// We need to find out how big the result is. We can't do strlen(dst) because
	// it isn't necessarily null terminated.
	size_t result = strlen(src);
	if (result >= size)
		result = size - 1;
	// Null-terminate the string. strncpy does not promise to do this.
	dst[result] = 0;
	return result;
}

size_t strlcat(char *dst, const char *src, size_t size)
{
	size_t startLength = strlen(dst);
	// If the destination string is already too long then we probably have
	// a bug in the calling code.
	assert(startLength < size);
	// Handle this unfortunate case without crashing. If we were to subtract
	// startLength from size then size would wrap around - since it is probably
	// unsigned.
	if (startLength >= size)
		return startLength;
	// Adjust for the amount of the dest buffer that is already used. size is
	// guaranteed to be greater than zero after this adjustment.
	dst += startLength;
	size -= startLength;
	// Copy as much of the string over as possible.
	size_t result = startLength + strlcpy(dst, src, size);
	return result;
}



size_t strlcpyAssert(char *dst, const char *src, size_t size)
{
	// Assert that the string will fit.
	size_t srcLength = strlen(src);
	assert(srcLength < size);

	return strlcpy(dst, src, size);
}

size_t strlcatAssert(char *dst, const char *src, size_t size)
{
	// Assert that the string will fit.
	size_t dstLength = strlen(dst);
	size_t srcLength = strlen(src);
	assert(dstLength + srcLength < size);

	return strlcat(dst, src, size);
}


// This is an implementation of the ANSI C/C++ version of this function.
// It behaves differently from Microsoft's _snprintf function because
// snprintf always null terminates the buffer.
// The return value is not quite right, since snprintf() is supposed to
// return the number of characters that would have been written out,
// which is not practical in this case, so I clip it to the buffer
// size. That is, if the string doesn't fit then this function returns
// count.
#ifdef	_MSC_VER
	int snprintf(char *buffer, size_t count, const char *format, ...)
	{
		assert(count > 0);
		if (count <= 0)
			return 0;
		va_list args; 
		va_start(args, format);

		int result = _vsnprintf(buffer, count, format, args);
		if (result < 0 || result >= int(count))
		{
			result = count;
			buffer[count-1] = 0;
		}

		va_end(args);

		return result;
	}

	// See the comment for snprintf
	int vsnprintf(char *buffer, size_t count, const char *format, va_list args)
	{
		assert(count > 0);
		if (count <= 0)
			return 0;
		int result = _vsnprintf(buffer, count, format, args);
		// Make sure the result is null terminated
		if (result < 0 || result >= int(count))
		{
			result = count;
			buffer[count-1] = 0;
		}
		return result;
	}
#endif

int snprintfAssert(char *buffer, size_t count, const char *format, ...)
{
	va_list args; 
	va_start(args, format);
	int result = vsnprintf(buffer, count, format, args);
	assert(result < int(count));
	va_end(args);
	return result;
}

int vsnprintfAssert(char *buffer, size_t count, const char *format, va_list args)
{
	int result = vsnprintf(buffer, count, format, args);
	assert(result < int(count));
	return result;
}


}	// End namespace HumongousEntertainment
