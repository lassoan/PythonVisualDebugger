/*
 * Copyright 1995, 1997 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * enviro.h - get/set environment variables/registry entries
 *
 * Note that there is no longer a global environment.  If
 * multiple threads wish to share the same enviroment, they'll
 * have to call Reload() to see any changes.  On UNIX, there
 * is no setting the environment so that isn't an issue.
 *
 * Public methods:
 *
 *	Enviro::BeServer() - get and set "system level" variables
 *	Enviro::BeService() - get and set service-specific variables (NT only)
 *	Enviro::Get() - get a variable from the environment
 *	Enviro::Set() - set a variable in the environment (NT only)
 *	Enviro::Config() - load $P4CONFIG file (if set)
 *	Enviro::List() - list variables in the environment 
 *	Enviro::SetOrPrint() - set or print a variable, depending
 *	Enviro::Reload() - flush values cached from NT registry
 */

class EnviroTable;
struct EnviroItem;
class Error;
class StrBuf;
class StrPtr;
struct KeyPair;

class Enviro {

    public:
			Enviro();
			~Enviro();

	enum ItemType { 
		NEW,	// not looked up yet
		UNSET,	// looked up and is empty
		UPDATE,	// set via the Update call
		ENV,	// set in environment
		CONFIG,	// via P4CONFIG
		SVC,	// set in service-specific registry
		USER,	// set in user registry
		SYS 	// set is machine registry
	};

	void		BeServer();
	void		BeService( const StrPtr *name );

	void		List();

	void		Print( const char *var );
	char		*Get( const char *var );
	void		Set( const char *var, const char *value, Error *e );
	void		Update( const char *var, const char *value);

	ItemType	GetType( const char *var );

	void		Config( const StrPtr &cwd );

	void		Reload();

	void		Save( const char *const *vars, Error *e );
	void		SaveConfig( const char *const *vars, Error *e, const StrPtr &cwd );

    private:

	EnviroTable	*symbolTab;
	EnviroItem	*GetItem( const char *var );

# ifdef OS_NT
	KeyPair		*setKey;
	KeyPair		*serviceKey;
	StrBuf		serviceKeyName;
# endif /* OS_NT */

} ;

