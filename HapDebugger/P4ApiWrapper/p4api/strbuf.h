/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * StrBuf.h - multipurpose buffers
 *
 * StrPtr, StrRef, and StrBuf are used throughout the system, as buffers
 * for storing just about any variable length byte data. 
 *
 * StrPtr is a low-cost (no constructor, no destructor, 8 byte) 
 * pointer/length pair to mutable data.  It has a variety of methods
 * to mangle it.
 *
 * StrRef is a kind-of StrPtr that allows the buffer pointer to be set.
 * As StrPtr doesn't allow this, a StrPtr object itself isn't useful.
 *
 * StrNum is a kind-of StrPtr with a temporary buffer whose only purpose
 * is to hold the string representation of an int.
 *
 * StrBuf is a kind-of StrPtr that allocates and extends it own buffer.
 *
 * Classes:
 *
 *	StrPtr - a pointer/length for arbitrary data
 *	StrRef - StrPtr that can be set
 *	StrNum - StrPtr that holds a string of an int
 *	StrBuf - StrPtr of privately allocated data
 *
 * Methods:
 *
 *	StrPtr::Clear() - set length = 0
 *	StrPtr::Text() - return buffer pointer
 *	StrPtr::Value() - return buffer pointer (old name)
 *	StrPtr::Length() - return buffer length
 *	StrPtr::GetEnd() - return pointer to character past end
 *	StrPtr::Atoi() - convert to integer and return
 *	StrPtr::SetLength() - set only length
 *	StrPtr::SetEnd() - set length by calculating from start
 *	StrPtr::[] - get a single character
 *	StrPtr::XCompare() - case exact string compare
 *	StrPtr::CCompare() - case folding string compare
 *	StrPtr::Contains() - finds a substring. Returns NULL if no substring found.
 *	StrPtr::Compare() - compare depending on OS
 *	StrPtr::== - compare contents with buffer
 *	StrPtr::!= - compare contents with buffer
 *	StrPtr::< - compare contents with buffer
 *	StrPtr::<= - compare contents with buffer
 *	StrPtr::> - compare contents with buffer
 *	StrPtr::>= - compare contents with buffer
 *	StrPtr::Copy() - fill buffer from another
 *	StrPtr::Words() - break buffer into whitespace-separated words
 *	StrPtr::Lines() - break buffer into \r\n or \n separated lines
 *	StrPtr::Lower() - lowercase each character (in place) in a string
 *	StrPtr::Caps() - uppercase each character (in place) in a string
 *	StrPtr::Dump() - pretty print contents, for debugging
 *
 *	StrPtr::UnpackInt() - extract an integer from front of buffer
 *	StrPtr::UnpackIntA() - extract an integer encoded in ascii
 *	StrPtr::UnpackChar() - extract a char string from front of buffer
 *	StrPtr::UnpackOctet() - extract a byte string from front of buffer
 *	StrPtr::UnpackStrBuf() - extract a char string from front of buffer
 *	StrPtr::UnpackStrBufA() - extract a char string with length in ascii
 *
 *	---
 *
 *	StrRef::Set() - set pointer/length
 *
 *	---
 *
 *	StrBuf::StringInit() - mimic actions of constructor
 *	StrBuf::Set() - allocate and fill from buffer
 *	StrBuf::Append() - append contents from buffer
 *	StrBuf::AppendNull() - terminate buffer
 *	StrBuf::Alloc() - allocate space in buffer and return pointer
 *	StrBuf::<< - Append contents from buffer or number
 *	StrBuf::Indent() - fill by indenting contents of another buffer
 *	StrBuf::Expand() - expand a string doing %var substitutions
 *	
 *	StrBuf::PackInt() - append a formatted int to buffer
 *	StrBuf::PackIntA() - append a formatted int to buffer in ascii
 *	StrBuf::PackChar() - append a formatted string to buffer
 *	StrBuf::PackOctet() - append byte array to buffer
 *	StrBuf::PackStrBuf() - append a formatted string to buffer
 *	StrBuf::PackStrBufA() - append a formatted string with len in ascii
 */

class StrBuf;
class StrDict;

class StrPtr {

    public:
	// Setting, getting

	char *	Text() const
		{ return buffer; }

	char *	Value() const
		{ return buffer; }

	int 	Length() const
		{ return length; }

	char *	End() const
		{ return buffer + length; }

	int	Atoi() const
		{ return atoi( buffer ); }

	void	SetLength() 
		{ length = strlen( buffer ); }

	void	SetLength( int len ) 
		{ length = len; }

	void	SetEnd( char *p ) 
		{ length = p - buffer; }

	char	operator[]( int x ) const
		{ return buffer[x]; }

	// Comparing

	int	Compare( const StrPtr &s ) const
		{ return zstrcmp( buffer, s.buffer ); }

	int	XCompare( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ); }

	int	CCompare( const StrPtr &s ) const
		{ return strcasecmp( buffer, s.buffer ); }

	const char *	Contains( const StrPtr &s ) const
		{ return strstr( Text(), s.Text() ); }

	int	operator ==( const char *buf ) const
		{ return strcmp( buffer, buf ) == 0; }

	int	operator !=( const char *buf ) const
		{ return strcmp( buffer, buf ) != 0; }

	int	operator <( const char *buf ) const
		{ return strcmp( buffer, buf ) < 0; }

	int	operator <=( const char *buf ) const
		{ return strcmp( buffer, buf ) <= 0; }

	int	operator >( const char *buf ) const
		{ return strcmp( buffer, buf ) > 0; }

	int	operator >=( const char *buf ) const
		{ return strcmp( buffer, buf ) >= 0; }

	int	operator ==( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) == 0; }

	int	operator !=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) != 0; }

	int	operator <( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) < 0; }

	int	operator <=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) <= 0; }

	int	operator >( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) > 0; }

	int	operator >=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) >= 0; }

	// Manipulating

	void	Copy( const StrPtr *s )
		{ Copy( s->buffer, s->length ); }

	void	Copy( const char *buf, int len ) 
		{ memcpy( buffer, buf, len ); }

    friend class StrBuf;
    friend class StrRef;

    protected:
	char	*buffer;
	int	length;

	static char nullStrBuf[];
} ;

class StrRef : public StrPtr {

    public:

		StrRef() {}

		StrRef( const StrPtr &s )
		{ Set( &s ); }

		StrRef( const char *buf )
		{ Set( (char *)buf ); }

		StrRef( const char *buf, int len )
		{ Set( (char *)buf, len ); }

	static StrPtr Null()
		{ return StrRef( nullStrBuf, 0 ); }

	void	operator =(const StrPtr &s)
		{ Set( &s ); }

	void	operator =(const char *buf)
		{ Set( (char *)buf ); }

	void 	Set( char *buf )
		{ buffer = buf; length = strlen( buf ); }
		 
	void	Set( char *buf, int len )
		{ buffer = buf; length = len; }

	void	Set( const StrPtr *s )
		{ Set( s->buffer, s->length ); }

	void	Set( const StrPtr &s )
		{ Set( s.buffer, s.length ); }

} ;

class StrNum : public StrPtr {

    public:
		StrNum( int v );

    private:
		char buf[24];

} ;

class StrBuf : public StrPtr {

    public:
		StrBuf() 
		{ StringInit(); }

	void	StringInit()
		{ length = size = 0; buffer = nullStrBuf; }

		~StrBuf()
		{ if( buffer != nullStrBuf ) delete []buffer; }

	// copy constructor, assignment

		StrBuf( const StrBuf &s )
		{ StringInit(); Set( &s ); }

	void	operator =(const StrBuf &s)
		{ Set( &s ); }

	void	operator =(const StrRef &s)
		{ Set( &s ); }

	void	operator =(const StrPtr &s)
		{ Set( &s ); }

	void	operator =(const char *buf)
		{ Set( buf ); }

	// Setting, getting

	void 	Clear( void )
		{ length = 0; }

	void	Set( const char *buf )
		{ Clear(); Append( buf ); }

	void	Set( const StrPtr *s )
		{ Clear(); Append( s ); }

	void	Set( const StrPtr &s )
		{ Clear(); Append( &s ); }

	void	Set( const char *buf, int len )
		{ Clear(); Append( buf, len ); }

	void	AppendNoNull( const char *buf, int len )
		{ memcpy( Alloc( len ), buf, len ); }

	void 	AppendNull() 
		{ *Alloc(1) = '\0'; --length; }

	void	Append( const char *buf );

	void	Append( const StrPtr *s );

	void	Append( const char *buf, int len );

	char *	Alloc( int len );

	// string << -- append string/number

	StrBuf& operator <<( const char *s )
		{ Append( s ); return *this; }

	StrBuf& operator <<( const StrPtr *s )
		{ Append( s ); return *this; }

	StrBuf& operator <<( const StrPtr &s )
		{ Append( &s ); return *this; }

	StrBuf& operator <<( int v );

    private:
	int	size;
} ;

