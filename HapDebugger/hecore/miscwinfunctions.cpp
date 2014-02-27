#include "priv_coreprecompiled.h"

#ifdef	WIN32

/*
Copyright 2001 Bruce Dawson, Cygnus Software
For more information, visit http://www.cygnus-software.com/papers/
or e-mail: comments@cygnus-software.com

This code may be redistributed as long as this notice is kept intact.
*/

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <time.h>

namespace HumongousEntertainment
{

// DBWin32 has a limit of about 500 characters that you can copy over at one time.
// OutputDebugString has a larger limit - around 1024 characters - but we limit
// ourselves to the DBWin32 limit.
#define	MAXPRINTSIZE	500

typedef BOOL	(__stdcall * tIsDebuggerPresent)();
typedef BOOL	(WINAPI *tReadProcessMemory)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);

// Return true if running under a debugger.
bool HECORECALLCONV UniversalIsDebuggerPresent()
{
	// We might as well cache the addresses of Kernel32 and
	// IsDebuggerPresent, to make this function perform more
	// efficiently. However, we should not cache the results
	// of IsDebuggerPresent(), because that may change if the
	// user attaches to the process after it starts.
	static HINSTANCE Kernel32 = LoadLibrary("kernel32.dll");
	static tIsDebuggerPresent pIsDebuggerPresent = (tIsDebuggerPresent)GetProcAddress(Kernel32, "IsDebuggerPresent");
	assert(Kernel32);

	bool Result = false;

	if (pIsDebuggerPresent)
	{
		if (pIsDebuggerPresent())
			Result = true;
	}
	else
	{
		// This is a quirky, undocumented and unguaranteed method of identifying that we are running under
		// a debugger under Windows 95. ReadProcessMemory, as a member of Kernel32, should always be
		// contained within it, which means that it should always have an address higher than it. However,
		// when running under a debugger on Win95 it _doesn't_. I don't know why, but isn't that
		// convenient!
		// How did I discover this? It's a long story. It turns out it has been separately documented,
		// by Matt Pietrek and probably others.
		// Since Win98 and WinNT both have the IsDebuggerPresent() function, this hacked technique only
		// needs to work on Win95 - which it does. Bruce, June 1998.
		// There is also a variable in the task information block, accessible off of the FS segment,
		// which can be used to get the same information.
		tReadProcessMemory pReadProcessMemory = (tReadProcessMemory)GetProcAddress(Kernel32, "ReadProcessMemory");
		if ((void *)pReadProcessMemory < (void *)Kernel32)	//lint !e611	Ignore suspicious cast.
			Result = true;
	}

	return Result;
}

// Simple class to hold HANDLEs and automatically close them at the end of scope.
class AutoHandle
{
public:
	AutoHandle(HANDLE handle)
		: m_handle(handle)
	{
	}
	~AutoHandle()
	{
		if (m_handle)
			CloseHandle(m_handle);
	}
	operator HANDLE()
	{
		return m_handle;
	}
private:
	HANDLE	m_handle;
	AutoHandle& operator=(const AutoHandle&);	// Unimplemented.
	AutoHandle(const AutoHandle&);				// Unimplemented.
};

// This is a replacement for OutputDebugString that also sends messages to
// DBWin32 under Win9x as well as NT, and avoids calling OutputDebugString
// when running under the debugger.
static void OutputDebugStringW95( const char* achBuffer)
{
	// DO NOT DO ANY ASSERTS OR C run time calls in this code - it has to
	// be safe to call this from ANYWHERE!!!

	/* 
	Do a regular OutputDebugString so that the output is 
	still seen in the debugger window when running under
	the debugger.

	Don't call OutputDebugString when not running under
	the debugger, because that fails inside of microthreads.
	*/
	if (UniversalIsDebuggerPresent())
	{
		// Use the explicit names so that if a project wants to
		// globally #define OutputDebugString() to point at
		// OutputDebugStringW95 there won't be any recursion
		// here.
		#ifdef _UNICODE
		::OutputDebugStringW(achBuffer);
		#else
		::OutputDebugStringA(achBuffer);
		#endif

		// The normal OutputDebugString behaviour on WinNT is to go either
		// to the debugger or to DBWin32. Not both. We will get that behaviour
		// if we do a return here, in the NT case. However, it is actually
		// very cool to have the output going to both DBWin32 and the debugger,
		// so I don't do a return.
	}

	/* See if DBWIN is open and waiting */
	AutoHandle heventDBWIN(OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_BUFFER_READY"));
	if ( !heventDBWIN )
		return;			

	/* get a handle to the data synch object */
	AutoHandle heventData(OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_DATA_READY"));
	if ( !heventData )
		return;			

	// Open the shared memory buffer.
	AutoHandle hSharedFile(CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, 4096, "DBWIN_BUFFER"));
	if (!hSharedFile) 
		return;

	LPSTR lpszSharedMem = (LPSTR)MapViewOfFile(hSharedFile, FILE_MAP_WRITE, 0, 0, MAXPRINTSIZE);
	if (!lpszSharedMem) 
		return;

	/* wait for buffer event */
	(void)WaitForSingleObject(heventDBWIN, INFINITE);

	/* write it to the shared memory */
	*((int *)lpszSharedMem) = _getpid();
	// Use lstrcpyn because it is safer - it won't overwrite the buffer.
	lstrcpyn(lpszSharedMem + sizeof(DWORD), achBuffer, MAXPRINTSIZE);

	/* signal data ready event */
	(void)SetEvent(heventData);
}

// Get the 'time' in seconds, with unspecified precision that should be at least
// one millisecond. The start time is unspecified.
double HECORECALLCONV GetSeconds()
{
	// On some (old) versions of Windows NT, multiple CPUs are not properly
	// handled in regards to keeping timers consistent.  However I believe
	// that this is no longer a problem, so we should be able to use
	// QueryPerformanceCounter() on machines that report that it works.
	static LARGE_INTEGER	iFrequency;
	static BOOL FreqResult = QueryPerformanceFrequency(&iFrequency);
	LARGE_INTEGER			iTime;
	BOOL CounterResult = QueryPerformanceCounter(&iTime);
	if ((FreqResult == TRUE) && (CounterResult == TRUE))
	{
		double Time = (double)iTime.QuadPart / (double)iFrequency.QuadPart;
		return Time;
	}

	// It is not guaranteed that QueryPerformanceCounter() will work on all
	// machines. Therefore we need this fallback code. It will probably
	// never be used.
	
	// The problem with these functions is that they wrap around after 49.7 days.  Bummer.
	//	return timeGetTime() / 1000.0;

	SYSTEMTIME	SysTime;
	GetSystemTime(&SysTime);
	FILETIME	FileTime;
	(void)SystemTimeToFileTime(&SysTime, &FileTime);	// Ignoring return value.
	double Result = (FileTime.dwHighDateTime * 65536.0 * 65536.0 + FileTime.dwLowDateTime) / 10000000.0;
	return Result;
}

// Do a debug print.
void HECORECALLCONV rdprintv(const char *format, va_list ap)
{
	char tempstr[8000] = "";
	// I subtract one from the buffer length to ensure proper behaviour on
	// the boundary case.
	if (_vsnprintf(tempstr, sizeof(tempstr) - 1, format,ap) < 0) 
	{
		// would have exceeded max
		tempstr[ sizeof(tempstr) - 2 ] = '\n'; // force a new-line
	}
	// BUG! _vsnprintf() does not always null terminate the output. If the number
	// of characters printed, not including the terminating null, exactly fits in
	// the buffer, it returns the 'success' code (positive) even though a badly
	// formed string is in the buffer. Yucch.  Bruce, Feb 1998.
	tempstr[ sizeof(tempstr) - 1 ] = '\0'; // force a terminating null
	/*
	BUGBUG
	Code to create a longer string for test purposes.
	for (int i = 0; i < 20; ++i)
	{
		sprintf(tempstr + strlen(tempstr), "%d ", i);
		strcat(tempstr, "This is just a test of a reasonably long string - wahoo!\n");
	}*/
	// Print in small pieces, to avoid hitting the DBWin32 or OutputDebugString
	// limitations.
	char *outputstring = tempstr;
	while (outputstring[0])
	{
		char	copy[MAXPRINTSIZE];
		lstrcpyn(copy, outputstring, MAXPRINTSIZE);
		OutputDebugStringW95(copy);
		outputstring += strlen(copy);
	}
}

void HECORECALLCONV rdprintf(const char* format, ...)
{
	va_list ap;

	va_start(ap,format);
	rdprintv(format, ap);
	va_end(ap);
}

// Does nothing right now...
void HECORECALLCONV HEInitializeLibrary()
{
}









///////////////////////////////////////////////////////////////////////////////
// Function:	GetLinkDate
//
// Purpose:		Given a module base address, extract the link date (as a string)
//
// Inputs:
//		ModuleAddress - base address of the module (dll or exe)
//
// Outputs:
//
// Return:
//		String containing the link time and date
//
///////////////////////////////////////////////////////////////////////////////
static const char* GetLinkDate(unsigned int ModuleAddress)
{
	static char	datebuffer[100];

#ifdef	WIN32

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)ModuleAddress;	// Read the DOS header.
	assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE);
	
	// Skip to the NT header.
	PIMAGE_NT_HEADERS pNTHeader = PIMAGE_NT_HEADERS((char *)dosHeader + dosHeader->e_lfanew);
	
	// The time stamp for an image indicates the date and time that the image was created by the
	// Win32 linker. The value is represented in the number of seconds elapsed since midnight
	// (00:00:00), January 1, 1970, Universal Coordinated Time, according to the system clock.
	// The time stamp can be printed using the C run-time (CRT) function ctime or equivalent.
	struct tm LinkTime = *localtime((time_t *)(&pNTHeader->FileHeader.TimeDateStamp));
	
	//sprintf(datebuffer, "UTC link time: %08lx - %s", pNTHeader->FileHeader.TimeDateStamp, asctime(&LinkTime));
	sprintf(datebuffer, "%s", asctime(&LinkTime));
	
	assert(strlen(datebuffer) > 0);
	datebuffer[strlen(datebuffer) - 1] = 0;	// Knock off the terminating \n character.

#else
	// This method of getting the build date gets compile date for this file.
	strcpy(datebuffer, __DATE__ " " __TIME__);
#endif

	return datebuffer;

} // GetLinkDate


///////////////////////////////////////////////////////////////////////////////
// Function:	GetDllLinkDate
//
// Get the link date of a DLL - this function should be called from within the 
// DLL itself in order to get the correct date.
//
// Inputs:
// Outputs:
//
// Return:
//		String containing the link time and date
//
///////////////////////////////////////////////////////////////////////////////
const char* GetDllLinkDate()
{
#ifdef	WIN32
	unsigned int uiCodePtr = 0;

	//a little assembly to get a pointer to some code - if this library
	//becomes a dll, this function needs to be inline allowing to get
	//the base address of the dll that is using this function.

	__asm
	{
CODE_PTR_LABEL:
		lea EAX, CODE_PTR_LABEL		//load the address of the label into EAX
		mov uiCodePtr, EAX			//store the address into uiCodePtr
	}

	//Using the code ptr, get the allocation base - this is the same as the 
	//HMODULE and is also the location of the PIMAGE_DOS_HEADER in memory
	//which we can use to extract the link date.
	
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((const void*)uiCodePtr, &mbi, sizeof(mbi));
	return GetLinkDate((unsigned int) mbi.AllocationBase);

#else
	//non win32, just use the __DATE__ and __TIME__ macros
	return GetLinkDate(0);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Function:	GetExeLinkDate
//
// Purpose:		Get the link date of an executable
//
// Inputs:		None.
//
// Outputs:		None.
//
// Return:
//		String containing the link time and date
//
///////////////////////////////////////////////////////////////////////////////
const char* GetExeLinkDate()
{
#ifdef	WIN32
	//The module handle (HMODULE) is also a pointer to the base of the executable
	//so it points to the PIMAGE_DOS_HEADER for the module.
	return GetLinkDate((unsigned int) GetModuleHandle(0));
#else
	//non win32, just use the __DATE__ and __TIME__ macros
	return GetLinkDate(0);
#endif
}










}	// End of he namespace

#endif	// WIN32
