#ifndef	ERRORHANDLER_H
#define	ERRORHANDLER_H


// Function Prototypes.
// These functions provide methods to report errors and warnings in commandline
// applications.  They manipulate the ErrorHandler class, but are also usable from .c files.

#if defined (__cplusplus)
extern "C" {
#endif

extern void __cdecl HE_FatalError(const char *msg, ...);
extern void __cdecl HE_Error(const char *msg, ...);
extern void __cdecl HE_Warning(const char *msg, ...);
extern void __cdecl HE_DisableErrorSummary();

#if defined (__cplusplus)
} // end extern "C"
#endif



#endif // ERRORHANDLER_H