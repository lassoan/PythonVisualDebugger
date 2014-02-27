/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * StrOps.h - operations on strings
 *
 *	StrOps - member-less object with static functions
 *
 * Methods:
 *
 *	StrOps::Caps() - uppercase each character (in place) in a string
 *	StrOps::Dump() - pretty print contents, for debugging
 *	StrOps::Sub() - replace one character with another
 *	StrOps::Expand() - expand a string doing %var substitutions
 *	StrOps::Indent() - fill by indenting contents of another buffer
 *	StrOps::Replace() - replace all occurences of a string
 *	StrOps::Lines() - break buffer into \r\n or \n separated lines
 *	StrOps::Lower() - lowercase each character (in place) in a string
 *	StrOps::Words() - break buffer into whitespace-separated words
 *	StrOps::OtoX() - turn an octet stream into hex
 *	StrOps::XtoO() - turn hex into an octet stream
 *
 *	StrOps::UnpackInt() - extract an integer from front of buffer
 *	StrOps::UnpackIntA() - extract an integer encoded in ascii
 *	StrOps::UnpackChar() - extract a char string from front of buffer
 *	StrOps::UnpackOctet() - extract a byte string from front of buffer
 *	StrOps::UnpackStrBuf() - extract a char string from front of buffer
 *	StrOps::UnpackStrBufA() - extract a char string with length in ascii
 *
 *	StrOps::PackInt() - append a formatted int to buffer
 *	StrOps::PackIntA() - append a formatted int to buffer in ascii
 *	StrOps::PackChar() - append a formatted string to buffer
 *	StrOps::PackOctet() - append byte array to buffer
 *	StrOps::PackStrBuf() - append a formatted string to buffer
 *	StrOps::PackStrBufA() - append a formatted string with len in ascii
 */

class StrPtr;
class StrRef;
class StrBuf;
class StrDict;

class StrOps {

    public:

	// Manipulating

	static void	Caps( StrBuf &o );
	static void	Dump( const StrPtr &o );
	static void	Sub( StrPtr &string, char target, char replacement );
	static void	Expand( StrBuf &o, const StrPtr &s, StrDict &d );
	static void	Indent( StrBuf &o, const StrPtr &s );
	static void	Replace( StrBuf &o, const StrPtr &i, const StrPtr &s, const StrPtr &r );
	static int 	Lines( StrBuf &o, char *vec[], int maxVec );
	static void	Lower( StrBuf &o );
	static int	Words( StrBuf &o, char *vec[], int maxVec );
	static void	OtoX( const unsigned char *octet, int len, StrBuf &x );
	static void 	XtoO( char *x, unsigned char *octet, int octLen );

	static char	OtoX( unsigned char o )
			{ return o >= 10 ? o - 10 + 'A' : o + '0'; }

	static unsigned char XtoO( char h )
			{ return h - ( h > '9' ? 'A' - 10 : '0' ); }

	// Marshalling

	static void	PackInt( StrBuf &o, int v );
	static void	PackIntA( StrBuf &o, int v );
	static void	PackChar( StrBuf &o, const char *c, int len );
	static void	PackOctet( StrBuf &o, const unsigned char *c, int len );
	static void	PackStrBuf( StrBuf &o, const StrPtr *s );
	static void	PackStrBufA( StrBuf &o, const StrPtr *s );

	static int	UnpackInt( StrRef &o );
	static int	UnpackIntA( StrRef &o );
	static void	UnpackChar( StrRef &o, char *c, int length );
	static void	UnpackOctet( StrRef &o, unsigned char *c, int length );
	static void	UnpackStrBuf( StrRef &o, StrBuf *s );
	static void	UnpackStrBufA( StrRef &o, StrBuf *s );

} ;

