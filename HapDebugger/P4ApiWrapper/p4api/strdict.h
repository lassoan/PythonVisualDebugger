/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * StrDict.h - a set/get dictionary interface
 *
 * Classes:
 *
 *	StrDict - a GetVar/PutVar dictionary interface
 *
 * Methods:
 *
 */

class Error;

class StrDict {

    public:

	virtual	~StrDict();

	// Handy wrappers

	void	SetVar( const char *var );
	void	SetVar( const char *var, int value );
	void	SetVar( const char *var, const char *value );
	void	SetVar( const char *var, const StrPtr *value );
	void	SetVar( const char *var, const StrPtr &value );
	void	SetVar( const StrPtr &var, const StrPtr &value )
		{ VSetVar( var, value ); }

	void	SetVarV( const char *arg );
	void	SetArgv( int argc, char *const *argv );
	void	SetVar( const StrPtr &var, int x, const StrPtr &val );
	void	SetVar( const char *var, int x, const char *val );
	void	SetVar( const char *var, int x, int y, const char *val );

	void	ReplaceVar( const char *var, const char *value );
	void	RemoveVar( const char *var );
	StrPtr *GetVar( const char *var );
	StrPtr *GetVar( const char *var, Error *e );
	StrPtr *GetVar( const StrPtr &var, int x );
	StrPtr *GetVar( const StrPtr &var, int x, int y );
	StrPtr *GetVar( const StrPtr &var )
		{ return VGetVar( var ); }

	int	GetVar( int x, StrPtr &var, StrPtr &val )
		{ return VGetVarX( x, var, val ); }
	
	int Save( FILE * out );
	int Load( FILE * out );
	
    private:

	// Get/Set vars, provided by subclass

	virtual StrPtr *VGetVar( const StrPtr &var ) = 0;
	virtual void	VSetVar( const StrPtr &var, const StrPtr &val ) = 0;
	virtual void	VRemoveVar( const StrPtr &var );
	virtual int	VGetVarX( int x, StrPtr &var, StrPtr &val );

	StrBuf		varBuf;

} ;
