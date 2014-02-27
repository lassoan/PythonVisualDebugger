#include "priv_coreprecompiled.h"

#ifdef	WIN32

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>

// Needed for GetUserName
#pragma comment(lib, "advapi32.lib")

/*
This source file contains the exception handler for recording error
information after crashes. See the Jan 1999 Game Developer Magazine
for information on how to hook it in.

It also contains a MiniDump creation routine that uses dbghelp.dll.

See AlwaysRun\ProcessRunner.cpp for an example of how to create mini dumps from
a debugger.
*/

// This source file requires the version of dbghelp.h from the Platform SDK.
// The November 2001 version is sufficiently recent - earlier versions may not
// work.
// Detect if the Platform SDK is installed
#if WINVER >= 0x0501
	#include <dbghelp.h>
#else
	// Dubious but powerful technique. If the PlatformSDK is not installed, define these
	// structures and enums. This is useful because the PlatformSDK is an annoying and
	// time consuming thing to install.
	typedef enum _MINIDUMP_TYPE {
		MiniDumpNormal         = 0x0000,
		MiniDumpWithDataSegs   = 0x0001,
		MiniDumpWithFullMemory = 0x0002,
		MiniDumpWithHandleData = 0x0004,
		MiniDumpFilterMemory   = 0x0008,
		MiniDumpScanMemory     = 0x0010,
	} MINIDUMP_TYPE;

	struct _MINIDUMP_USER_STREAM;
	typedef _MINIDUMP_USER_STREAM *PMINIDUMP_USER_STREAM;

	typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
		ULONG UserStreamCount;
		PMINIDUMP_USER_STREAM UserStreamArray;
	} MINIDUMP_USER_STREAM_INFORMATION, *PMINIDUMP_USER_STREAM_INFORMATION;

	typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
		DWORD ThreadId;
		PEXCEPTION_POINTERS ExceptionPointers;
		BOOL ClientPointers;
	} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;
	struct _MINIDUMP_CALLBACK_INPUT;
	typedef _MINIDUMP_CALLBACK_INPUT *PMINIDUMP_CALLBACK_INPUT;
	struct _MINIDUMP_CALLBACK_OUTPUT;
	typedef _MINIDUMP_CALLBACK_OUTPUT *PMINIDUMP_CALLBACK_OUTPUT;
	typedef
	BOOL
	(WINAPI * MINIDUMP_CALLBACK_ROUTINE) (
		IN PVOID CallbackParam,
		IN CONST PMINIDUMP_CALLBACK_INPUT CallbackInput,
		IN OUT PMINIDUMP_CALLBACK_OUTPUT CallbackOutput
		);

	typedef struct _MINIDUMP_CALLBACK_INFORMATION {
		MINIDUMP_CALLBACK_ROUTINE CallbackRoutine;
		PVOID CallbackParam;
	} MINIDUMP_CALLBACK_INFORMATION, *PMINIDUMP_CALLBACK_INFORMATION;
#endif

// This library required for GetUserName
#pragma comment(lib, "Advapi32.lib")

namespace HumongousEntertainment
{

const int kPaddedPathLength = MAX_PATH + 20;

// This is stored here so that the error log code can specify which minidump is associated
// with it.
static char s_dumpFileName[kPaddedPathLength];

const int NumCodeBytes = 16;	// Number of code bytes to record.
const int MaxStackDump = 2048;	// Maximum number of DWORDS in stack dumps.
const int StackColumns = 8;		// Number of columns in stack dump.

#define	ONEK			1024
#define	SIXTYFOURK		(64*ONEK)
#define	ONEM			(ONEK*ONEK)
#define	ONEG			(ONEK*ONEK*ONEK)

// hprintf behaves similarly to printf, with a few vital differences.
// It uses wvsprintf to do the formatting, which is a system routine,
// thus avoiding C run time interactions. For similar reasons it
// uses WriteFile rather than fwrite.
// The one limitation that this imposes is that wvsprintf, and
// therefore hprintf, cannot handle floating point numbers.

static void hprintf(HANDLE LogFile, char* Format, ...)
{
	char buffer[2000];	// wvsprintf never prints more than one K.

	va_list arglist;
	va_start( arglist, Format);
	wvsprintf(buffer, Format, arglist);
	va_end( arglist);

	DWORD NumBytes;
	WriteFile(LogFile, buffer, lstrlen(buffer), &NumBytes, 0);
}

// Print the specified FILETIME to output in a human readable format,
// without using the C run time.

static void PrintTime(char *output, FILETIME TimeToPrint)
{
	WORD Date, Time;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
				FileTimeToDosDateTime(&TimeToPrint, &Date, &Time))
	{
		// What a silly way to print out the file date/time. Oh well,
		// it works, and I'm not aware of a cleaner way to do it.
		wsprintf(output, "%d/%d/%d %02d:%02d:%02d",
					(Date / 32) & 15, Date & 31, (Date / 512) + 1980,
					(Time / 2048), (Time / 32) & 63, (Time & 31) * 2);
	}
	else
		output[0] = 0;
}

// Print information about a code module (DLL or EXE) such as its size,
// location, time stamp, etc.

static void ShowModuleInfo(HANDLE LogFile, HINSTANCE ModuleHandle)
{
	char ModName[MAX_PATH];
	__try
	{
		if (GetModuleFileName(ModuleHandle, ModName, sizeof(ModName)) > 0)
		{
			// If GetModuleFileName returns greater than zero then this must
			// be a valid code module address. Therefore we can try to walk
			// our way through its structures to find the link time stamp.
			IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER*)ModuleHandle;
		    if (IMAGE_DOS_SIGNATURE != DosHeader->e_magic)
	    	    return;
			IMAGE_NT_HEADERS *NTHeader = (IMAGE_NT_HEADERS*)((char *)DosHeader
						+ DosHeader->e_lfanew);
		    if (IMAGE_NT_SIGNATURE != NTHeader->Signature)
	    	    return;
			// Open the code module file so that we can get its file date
			// and size.
			HANDLE ModuleFile = CreateFile(ModName, GENERIC_READ,
						FILE_SHARE_READ, 0, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, 0);
			char TimeBuffer[100] = "";
			DWORD FileSize = 0;
			if (ModuleFile != INVALID_HANDLE_VALUE)
			{
				FileSize = GetFileSize(ModuleFile, 0);
				FILETIME	LastWriteTime;
				if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
				{
					wsprintf(TimeBuffer, " - file date is ");
					PrintTime(TimeBuffer + lstrlen(TimeBuffer), LastWriteTime);
				}
				CloseHandle(ModuleFile);
			}
			hprintf(LogFile, "%s, loaded at 0x%08x - %d bytes - %08x%s\r\n",
						ModName, ModuleHandle, FileSize,
						NTHeader->FileHeader.TimeDateStamp, TimeBuffer);
		}
	}
	// Handle any exceptions by continuing from this point.
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

// Scan memory looking for code modules (DLLs or EXEs). VirtualQuery is used
// to find all the blocks of address space that were reserved or committed,
// and ShowModuleInfo will display module information if they are code
// modules.

static void RecordModuleList(HANDLE LogFile)
{
	hprintf(LogFile, "\r\n"
					 "\tModule list: names, addresses, sizes, time stamps "
			"and file times:\r\n");
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	const size_t PageSize = SystemInfo.dwPageSize;
	// Set NumPages to the number of pages in the 4GByte address space,
	// while being careful to avoid overflowing ints.
	const size_t NumPages = 4 * size_t(ONEG / PageSize);
	size_t pageNum = 0;
	void *LastAllocationBase = 0;
	while (pageNum < NumPages)
	{
		MEMORY_BASIC_INFORMATION	MemInfo;
		if (VirtualQuery((void *)(pageNum * PageSize), &MemInfo,
					sizeof(MemInfo)))
		{
			if (MemInfo.RegionSize > 0)
			{
				// Adjust the page number to skip over this block of memory.
				pageNum += MemInfo.RegionSize / PageSize;
				if (MemInfo.State == MEM_COMMIT && MemInfo.AllocationBase >
							LastAllocationBase)
				{
					// Look for new blocks of committed memory, and try
					// recording their module names - this will fail
					// gracefully if they aren't code modules.
					LastAllocationBase = MemInfo.AllocationBase;
					ShowModuleInfo(LogFile, (HINSTANCE)LastAllocationBase);
				}
			}
			else
				pageNum += SIXTYFOURK / PageSize;
		}
		else
			pageNum += SIXTYFOURK / PageSize;
		// If VirtualQuery fails we advance by 64K because that is the
		// granularity of address space doled out by VirtualAlloc().
	}
}

// Record information about the user's system, such as processor type, amount
// of memory, etc.

static void RecordSystemInformation(HANDLE LogFile)
{
	FILETIME	CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	char TimeBuffer[100];
	PrintTime(TimeBuffer, CurrentTime);
	hprintf(LogFile, "Error occurred at %s.\r\n", TimeBuffer);
	char	ModuleName[MAX_PATH];
	if (GetModuleFileName(0, ModuleName, sizeof(ModuleName)) <= 0)
		lstrcpy(ModuleName, "Unknown");
	char	UserName[200];
	DWORD UserNameSize = sizeof(UserName);
	if (!GetUserName(UserName, &UserNameSize))
		lstrcpy(UserName, "Unknown");
	hprintf(LogFile, "%s, run by %s.\r\n", ModuleName, UserName);

	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	hprintf(LogFile, "%d processor(s), type %d.\r\n",
				SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);

	MEMORYSTATUS	MemInfo;
	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatus(&MemInfo);
	// Print out the amount of physical memory, rounded up.
	hprintf(LogFile, "%d MBytes physical memory.\r\n", (MemInfo.dwTotalPhys +
				ONEM - 1) / ONEM);
}

// Translate the exception code into something human readable.

static const char *GetExceptionDescription(DWORD ExceptionCode)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		char*	ExceptionName;
	};

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, "a Control-C"},
		{0x40010008, "a Control-Break"},
		{0x80000002, "a Datatype Misalignment"},
		{0x80000003, "a Breakpoint"},
		{0xc0000005, "an Access Violation"},
		{0xc0000006, "an In Page Error"},
		{0xc0000017, "a No Memory"},
		{0xc000001d, "an Illegal Instruction"},
		{0xc0000025, "a Noncontinuable Exception"},
		{0xc0000026, "an Invalid Disposition"},
		{0xc000008c, "a Array Bounds Exceeded"},
		{0xc000008d, "a Float Denormal Operand"},
		{0xc000008e, "a Float Divide by Zero"},
		{0xc000008f, "a Float Inexact Result"},
		{0xc0000090, "a Float Invalid Operation"},
		{0xc0000091, "a Float Overflow"},
		{0xc0000092, "a Float Stack Check"},
		{0xc0000093, "a Float Underflow"},
		{0xc0000094, "an Integer Divide by Zero"},
		{0xc0000095, "an Integer Overflow"},
		{0xc0000096, "a Privileged Instruction"},
		{0xc00000fD, "a Stack Overflow"},
		{0xc0000142, "a DLL Initialization Failed"},
		{0xe06d7363, "a Microsoft C++ Exception"},
	};

	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return ExceptionMap[i].ExceptionName;

	return "Unknown exception type";
}

static char* GetFilePart(char *source)
{
  return (char*)he::getfilename(source);
	/*
  char *result = strrchr(source, '\\');
	if (result)
		result++;
	else
		result = source;
	return result;
  */
}

static int RecordExceptionInfo(PEXCEPTION_POINTERS data, const char *Message)
{	
	return EXCEPTION_CONTINUE_SEARCH;
}

// Returns true if the dump succeeded, false if it failed for any reason.
// This function can be used for in our out of process minidump creation.
bool HECORECALLCONV WriteMiniDump(HANDLE hProcess, DWORD processID, PMINIDUMP_EXCEPTION_INFORMATION exceptionParam)
{
	// By default indicate that no minidump was saved.
	s_dumpFileName[0] = 0;
	// Make it a bit longer than path max just to guarantee that there is always enough room
	// for the dbghelp dll name.
	char	moduleName[kPaddedPathLength];
	// Should we find the module name of this process or the client? Finding the module name
	// of the client's process is a pain - it is very operating system dependent - and it's
	// not clearly enough better to justify the hassle.
	//(void)GetModuleFileNameEx(hProcess, 0, moduleName, MAX_PATH);
	(void)GetModuleFileName(0, moduleName, MAX_PATH);  
	char* filePart = (char*)getfilename(moduleName);
	const char* const dbgHelpName = "dbghelp.dll";
	strcpy(filePart, dbgHelpName);

	// This will get loaded multiple times if you call this routine multiple times. Oh well.
	const char* loadedName = moduleName;
	// Try to load a special HE named version, so that we can search through our path.
	HMODULE dbgHelpModule = LoadLibrary("hedbghelp.dll");
	if (!dbgHelpModule)
		dbgHelpModule = LoadLibrary(loadedName);
	if (!dbgHelpModule)
	{
		loadedName = dbgHelpName;
		dbgHelpModule = LoadLibrary(loadedName);
		if (!dbgHelpModule)
		{
			dprintf("Couldn't load '%s' or '%s'\n", moduleName, dbgHelpName);
			return false;
		}
	}

	typedef BOOL (WINAPI *tMiniDumpWriteDump)(HANDLE hProcess, DWORD processID,
				HANDLE hFile, MINIDUMP_TYPE dumpType,
				PMINIDUMP_EXCEPTION_INFORMATION exceptionParam,
				PMINIDUMP_USER_STREAM_INFORMATION userStreamParam,
				PMINIDUMP_CALLBACK_INFORMATION callbackParam);
	tMiniDumpWriteDump MiniDumpWriteDump = (tMiniDumpWriteDump)GetProcAddress(dbgHelpModule, "MiniDumpWriteDump");
	if (!MiniDumpWriteDump)
	{
		dprintf("Couldn't find function MiniDumpWriteDump in '%s'\n", loadedName);
		return false;
	}

	HANDLE outputFile = INVALID_HANDLE_VALUE;
	// Arbitrary maximum number of tries, in case we're on read-only media (in which
	// case it's really too many)
	const int kMaxTries = 500;
	char dumpFileName[kPaddedPathLength];
	strcpy(dumpFileName, moduleName);
	filePart = (char*)getfilename(dumpFileName);
	for (int i = 1; i <= kMaxTries && INVALID_HANDLE_VALUE == outputFile; ++i)
	{
		sprintf(filePart, "errorlog%03d.dmp", i);
		// This create call will fail if the file already exists. This is used
		// to create sequentially numbered dump files.
		outputFile = CreateFile(dumpFileName, GENERIC_WRITE, 0,
					0, CREATE_NEW,
					FILE_ATTRIBUTE_NORMAL,
					0);
		DWORD errorCode = GetLastError();
		if (INVALID_HANDLE_VALUE == outputFile && errorCode != ERROR_FILE_EXISTS)
		{
			// The file creation failed, and it's not because that number has been used
			// already. This usually means that the application is being run off of a
			// CD or other write-protected medium.
			// We should try to do something intelligent here, like create the mini
			// dump elsewhere - on the hard drive in the temp directory?
			dprintf("Error creating output file '%s', error code %d.\n", dumpFileName, errorCode);
			return false;
		}
	}

	if (INVALID_HANDLE_VALUE == outputFile)
	{
		dprintf("Couldn't create output file '%s'.\n", dumpFileName);
		return false;
	}

	BOOL result = MiniDumpWriteDump(hProcess, processID,
				outputFile, MiniDumpNormal,
				exceptionParam,
				0, 0);
	CloseHandle(outputFile);

	if (result)
	{
		dprintf("Successfully wrote out error log.\n");
		// Indicate that we successfully saved an error log, and what its name is.
		lstrcpyn(s_dumpFileName, dumpFileName, NUMELEMENTS(s_dumpFileName));
	}
	else
	{
		dprintf("Minidump write failed.\n");
	}

	// Return true on success.
	return TRUE == result;
}

// threadDescription is currently unused, but could be helpful.
static int HECORECALLCONV WriteMiniDumpThisProcess(EXCEPTION_POINTERS* pExceptionInfo, const char* threadDescription)
{
	MINIDUMP_EXCEPTION_INFORMATION exInfo;

	exInfo.ThreadId = ::GetCurrentThreadId();
	exInfo.ExceptionPointers = pExceptionInfo;
	exInfo.ClientPointers = FALSE;

	WriteMiniDump(GetCurrentProcess(), GetCurrentProcessId(), &exInfo);

	// Oops - we didn't handle the exception - continue on...
	return EXCEPTION_CONTINUE_SEARCH;
}

int HECORECALLCONV HandleException(PEXCEPTION_POINTERS data, const char *message)
{
	// First write a minidump
	WriteMiniDumpThisProcess(data, message);
	// Then update errorlog.txt.
	return RecordExceptionInfo(data, message);
}

}	// End of he namespace

#endif	// WIN32