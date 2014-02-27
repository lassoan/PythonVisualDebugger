#ifndef	INC_CORE_H
#define	INC_CORE_H

/*
This header defines a useful set of defines, types, pragmas and
includes that I like to always have available. It should very rarely
be changed, and should be included from everywhere.
*/

// Some generally useful header files that I want always available.
#include <stdio.h>
#include <stdlib.h>
//#include <memory.h>
#include <stdarg.h>	// Needed for va_list
#include <string> // Needed for std namespace for string printing functions

// Must be declared outside of the namespace
#ifdef	WIN32
	// Record detailed information about a crash, to give you a hope
	// of being able to diagnose it.
	// Forward declarations to let this compile even without windows.h
	struct _EXCEPTION_POINTERS;
	typedef _EXCEPTION_POINTERS* PEXCEPTION_POINTERS;

	struct _MINIDUMP_EXCEPTION_INFORMATION;
	typedef _MINIDUMP_EXCEPTION_INFORMATION* PMINIDUMP_EXCEPTION_INFORMATION;
#endif




// A long namespace name is used, and a short alias is created at the
// bottom of this header file. "he" can be used in place of
// HumongousEntertainment. I believe this gives us the safety of a long
// namespace with the convenience of a short one.
namespace HumongousEntertainment
{

#ifdef	_MSC_VER	// Visual C++
	// Disable the STUPID!!! STL in use warning. This is done here so that
	// it will be done before any of the STL headers are included - necessary
	// to making the warning go away.
	#pragma warning(disable : 4786)
	// This disgusting macro forces VisualC++ to use the ANSI C++ rules
	// for the scoping of variables declared inside for statements.
	// Trust me - it works. Using this evil macro makes it much easier
	// to write portable code. This will be unnecessary starting with
	// version 7.0 of VC++
	// This hack is actually documented (recommended?) in MSDN, q167748
	#define	for if (0) ; else for
	#define	HEDEBUG_BREAK()	__asm { int 3 }
	#define	HECORECALLCONV	__cdecl
#elif defined(VK_CUBE)
	#define	HECORECALLCONV
#elif defined(__MSL_CPP__)	// CodeWarrior - PC only?
	#define	HEDEBUG_BREAK() DebugBreak()
	#define	HECORECALLCONV
#else
	#error Unknown compiler - please implement HEDEBUG_BREAK and any other platform specific things.
#endif

// Entry point to allow initialization of various things, like loading error logs, etc.
void HECORECALLCONV HEInitializeLibrary();

// Cool compile time assert. Use for things like:
// CompileTimeAssert(sizeof(BITMAPINFOHEADER) == sizeof(PortableBITMAPINFOHEADER));
// From Writing Bug-Free C Code for Windows, Jerry Jongerius, Prentice Hall, 1995
#define CompileTimeAssert(exp) extern char _CompileTimeAssert[(exp)?1:-1]

// The COMPILERMSG macro is used to get the compiler to display a message whenever a
// file is compiled, typically a reminder to do something. The message is printed out
// in a format such that if you double click on it you are taken to the line and file
// it is in.
#define COMPILERMSGSTR(x)    #x	// Internally used macro.
#define COMPILERMSGSTR2(X)    COMPILERMSGSTR(X)	// Internally used macro.
#define COMPILERMSG(msg) message(__FILE__ "(" COMPILERMSGSTR2(__LINE__) ") : " msg)
// Here is an example of how to use the COMPILERMSG macro:
//#pragma COMPILERMSG("Don't forget to fix this code.")
// Note the '#pragma' at the beginning, and no semicolon at the end.

// Asserts are good. However Microsoft's implementation is not always good.
// It can very easily cause infinitely recursive asserts, because when the
// assert dialog appears it runs a message pump, which can cause messages
// to be delivered to your window - which can cause your assert to trigger
// again.
// Recursion: n. see recursion.
// Also, if you click 'Debug' on one of Microsoft's assert you get dropped
// into the assert.c source file, instead of into your own source code.
// This assert macro fixes these problems.
// Define this if you don't want the fixed assert dialogs.
#define	USEDEFAULTASSERTDEF
#ifdef	VK_CUBE
	// Viskit already defines a nice assert for Game Cube - thanks to David Galloway
	#define	USEDEFAULTASSERTDEF
#endif
#ifdef	USEDEFAULTASSERTDEF
}
	// We can't #include <assert.h> from within the HumongousEntertainment namespace - that
	// might end up putting C run-time symbols into the HE namespace!
	// So, we temporarily close the namespace, and then reopen it.
	#include <assert.h>
namespace HumongousEntertainment
{
#else
	#undef	assert	// Make sure we've wiped out any previous definitions.
	bool HECORECALLCONV heassert(const char* expr, const char* filename, unsigned int lineno);
	#if defined(_DEBUG) || defined(ASSERTSENABLED)
		// In debug mode we need an assert macro and an assert function.
		// heassert() returns true if the user requests debugging.
		#define assert(exp) \
				do { \
					if (!(exp)) { \
						dprintf("%s(%d): Assertion failed\n%s\n", __FILE__, __LINE__, #exp); \
						if (he::heassert(#exp, __FILE__, __LINE__)) \
							HEDEBUG_BREAK(); \
						} \
				} while (0)
	#else
		// In release mode, asserts do nothing.
		#define assert(exp)     ((void)0)
	#endif
#endif
#ifdef  _DEBUG
	#define verify(exp) assert(exp)
#else
	#define	verify(exp)	exp
#endif

// I love this macro for finding the the number of elements in an array.
// What can I say - I know the preprocessor is evil, but it works so
// damned well for a few key things!
#define	NUMELEMENTS(x)	(sizeof(x) / sizeof(x[0]))

// Some generally useful functions that I want to have always available.

// Get the 'time' in seconds, with unspecified precision that should be at least
// one millisecond. The start time is unspecified.
double HECORECALLCONV GetSeconds();

// The 'r' versions of these functions continue working in
// release builds. The versions without the r disappear in
// release builds.
// Send formatted debugging information to the debugger output
// window and DBWin32.
// If you don't do a "using namespace he" then rdprintf and rdprintv
// need to be fully qualified with an he:: prefix.
// dprintf and dprintv never need an he:: prefix.
void HECORECALLCONV rdprintf(const char* format, ...);
void HECORECALLCONV rdprintv(const char *format, va_list ap);
#if defined(_DEBUG) || defined(_PCLINT) || defined(DPRINTFENABLED)
	#define	dprintf he::rdprintf
	#define	dprintv he::rdprintv
#else
	// Magic to make these disappear in release builds.
	#define dprintf 0 && 
	#define dprintv(x, y) ((void)0)
#endif

#ifdef	WIN32
	// Are we running under a debugger? Works on Win95 also.
	bool HECORECALLCONV UniversalIsDebuggerPresent();

	// Record detailed information about a crash, to give you a hope
	// of being able to diagnose it.
	// Use it like this:
	// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
	// {
	// 	// Wrap WinMain in a structured exception handler (different from C++
	// 	// exception handling) in order to make sure that all access violations
	// 	// and other exceptions are displayed - regardless of when they happen.
	// 	// This should be done for each thread, if at all possible, so that exceptions
	// 	// will be reliably caught, even inside the debugger.
	// 	__try
	// 	{
	// 		// Your program goes here!
	// 	}
	// 	__except(HandleException(GetExceptionInformation(), "Main Thread"))
	// 	{
	// 		// We don't actually do anything inside the handler. All of the
	// 		// work is done by Cavedog_HandleException()
	// 	}
	// 	return 0;
	// }
	// For more information (albeit about the Cavedog library version of this) see:
	// http://humdog/programming/library/extras/gettingstarted.html and
	// http://humdog/programming/courses/course2/default.html
	// This function probably cannot be implemented on platforms other than the PC.
	// The HandleException functions creates both errorlog.txt and a minidump file.
	// This articles explains how and why to use minidumps:
	// http://www.codeproject.com/debug/postmortemdebug_standalone1.asp
	int HECORECALLCONV HandleException(PEXCEPTION_POINTERS data, const char *Message);
	// WriteMiniDump is automatically called by HandleException. For sample code showing
	// how to call WriteMiniDump() from a debugger, see AlwaysRun\AlwaysRun.cpp
	#ifdef	WINVER
		// This declaration can only be compiled if you have previously included windows.h,
		// since it uses a HANDLE as a parameter.
		bool HECORECALLCONV WriteMiniDump(HANDLE hProcess, DWORD processID, PMINIDUMP_EXCEPTION_INFORMATION exceptionParam);
	#endif
#endif

//the following two functions allow you to get the link time from either a dll
//or from an executable - very useful in version labelling.
const char* HECORECALLCONV GetExeLinkDate();
const char* HECORECALLCONV GetDllLinkDate();

// Everybody has types used for specified sized ints (even though
// you should mostly just use int), however everybody spells them
// differently. int8, INT8, Int8, etc. The spelling below is the
// CORRECT spelling. Don't believe me? Check the spec for the new
// version of C, currently available in v:\systems\docs\c9x-draft.txt
// Remember that in most cases 'int' is preferable to these types.
typedef	signed char int8_t;
typedef	unsigned char uint8_t;
typedef	signed short int16_t;
typedef	unsigned short uint16_t;
typedef	signed int int32_t;
typedef	unsigned int uint32_t;
#if	defined(VK_CUBE) || defined(__MWERKS__)
	// No __int64 available, or I'm not sure what it is called.
#else
	typedef	signed __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#endif



// String printing functions
// These allow you to do a variable argument printf style call to 
// print formatted output into a C++ string.
// 
// They're not exceptionally fast, so don't use them for games, but they
// do remove any arbitrary limitations by not having any fixed-length buffers.

std::string HECORECALLCONV stringprintf(char const *format, ...);
std::string HECORECALLCONV vstringprintf(const char* format, va_list args);

const char* HECORECALLCONV getfilename(const char* path);



}	// End namespace HumongousEntertainment

namespace he = HumongousEntertainment;

template <typename T>
T HAP_MIN(const T& lhs, const T& rhs)
{
	return lhs < rhs ? lhs : rhs;
}



// These have to be imported into the global namespace so that the dprintf macro
// can find them. This is still better than having them not in a namespace at all
// because you still avoid some name conflicts.
// Unfortunately they can't be imported into the global namespace or else when you
// do "using namespace he;" it becomes ambiguous to use these functions. That is
// probably a compiler bug. So, the dprintf macro needs to explicitly mention the
// namespace, and if you don't do a using directive then you need to explicitly
// specify the namespace on rdprintf but not dprintf. Joy. The alternative is that
// you always have to specify the namespace on rdprintf, even if you do a using directive.
//using HumongousEntertainment::rdprintf;
//using HumongousEntertainment::rdprintv;

#endif	// INC_CORE_H
