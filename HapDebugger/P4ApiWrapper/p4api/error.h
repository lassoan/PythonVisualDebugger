/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 */

/*
 * Error.h - accumulate and report layered errors
 *
 * Class Defined:
 *
 *	Error - holder of layered error
 *
 *	    The basic idea of Error is that the top level caller
 *	    should have one on its stack and hand it by reference
 *	    down to all lower layers.  If any operation fails, it
 *	    can add its description of the error to the structure
 *	    with Set().  After each operation that can potentially
 *	    fail, the caller should use Test() to check for errors.
 *	    The top level should do the check and then call Report()
 *	    if necessary.
 *
 * Public methods:
 *
 *	Error::Clear() - clean an Error struct
 *	Error::Test() - see if an error is present
 *	Error::IsFatal() - is most severe error fatal?
 *	Error::IsWarning() - is most severe error just a warning?
 *	Error::GetSeverity() - return ErrorSeverity of most severe error
 *	Error::GetGeneric() - return Generic code of most severe error
 *
 *	Error::Set() - add an error message into an Error struct
 *	Error::Sys() - add a system error message into an Error struct
 *	Error::Net() - add a network error message into an Error struct
 *
 *	Error::Report() - blurt out the contents of the Error to stderr
 *	Error::Abort() - blurt out an error and exit
 *	Error::Fmt() - format an error message
 *
 *	Error::operator = - copy Error structs
 *	Error::Marshall() - pack an Error into a StrBuf
 *	Error::UnMarshall() - unpack an Error from a StrBuf
 *
 *	Error::SetLog() - redirect Abort() and Report() to named file
 *	Error::SetTag() - replace standard tag used by Report()
 *
 *	Error::SetSyslog() - redirect error messages to syslog on UNIX.
 *	Error::UnsetSyslog() - Cancel syslog redirection. Revert to log file.
 */

# ifndef __ERROR_H__
# define __ERROR_H__

class StrBuf;
class StrPtr;

const int ErrorMax = 8;

/*
 * ErrorSeverity - how bad is the error?
 */

enum ErrorSeverity {

	E_EMPTY = 0,	// nothing yet
	E_INFO = 1,	// something good happened
	E_WARN = 2,	// something not good happened
	E_FAILED = 3,	// user did somthing wrong
	E_FATAL = 4	// system broken -- nothing can continue

} ;

/*
 * ErrorID - an error code and message
 * ErrorOf() - construct an ErrorID from bits
 * ErrorSev() - get severity from ErrorID (internal use only!)
 *
 *	e - ErrorSeverity (4 bits)
 *	a - # of arguments, error specific (4 bits)
 *	eg - generic error, defined in errornum.h (8 bits)
 *	es - subsystem id, defined in errornum.h (8 bits)
 *	ec - code within subsystem, error specific (8 bits)
 */

// daim-bread gcc 9 on beos and cygwin perverts this const typedef if
// it has seen anonymous enum declarations!
// We don't use the const anymore -- it just ain't worth the trouble.

typedef /* const */ struct {
	int		code;		// ErrorOf
	const char	*fmt;
} ErrorId ;

# define ErrorOf( e, eg, es, ec, a ) ((e<<28)|(a<<24)|(eg<<16)|(es<<8)|ec)
# define ErrorSev( code ) (ErrorSeverity)(code>>28)

/*
 * class Error - hold layered errors.
 */

class Error {

    public:
			Error();
			~Error();

	void 		operator =( Error &source );

	void		Clear() { severity = E_EMPTY; }
	int		Test() { return severity != E_EMPTY; }
	int		IsFatal() { return severity == E_FATAL; }
	int		IsWarning() { return severity == E_WARN; }
	int		GetSeverity() { return severity; }
	int		GetGeneric() { return generic; }

	// Set errors, the new way

	Error &		Set( const ErrorId &id );
	Error &		Set( ErrorSeverity s, const char *fmt );

	Error &		operator <<( int arg );
	Error &		operator <<( const char *arg );
	Error &		operator <<( const StrPtr &arg );

	// Save system errors

	void		Sys( const char *op, const char *arg );
	void		Net( const char *op, const char *arg );

	// Output

	void		Fmt( StrBuf *buf );

	void		Abort();
	void		Report();

	// Moving across client/server boundary

	void		Marshall( StrBuf *out );
	void		UnMarshall( const StrPtr *in );

	// Global settings

	void		SetLog( const char *file );
	void		SetSyslog();
	void		UnsetSyslog();
	void		SetTag( const char *tag ) { errorTag = tag; }

    private:

	Error &		Set( int code, const char *fmt );

	void		ReportLine( int errNo, StrBuf *dst );

	static const char *errorTag;
	static void	*errorLog;
	static int	useSyslog;

	// Remainder is the actual error info

	ErrorSeverity	severity;	// of worst error
	int		generic;	// of worst error
	char 		errorCount;
	StrBuf		*errorBuf;	// all fmts and params

	int		errorCode[ ErrorMax ];		// code for each
	int		errorOffset[ ErrorMax ];	// into errorBuf
} ;

/*
 * AssertError() - in case you need a global error to Abort() on
 */

extern Error AssertError;

# endif /* __ERROR_H__ */
