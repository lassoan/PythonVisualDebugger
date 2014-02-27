/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

# include "stdhdrs.h"

# include "strbuf.h"
# include "strdict.h"
# include "error.h"

# include "filesys.h"

# include "p4tags.h"

# include "clientuser.h"

/*
 * ClientApi - the Perforce client API
 *
 * Basic flow:
 *
 *	ClientUser ui;
 *	ClientApi client;
 *
 *	// SetPort(), SetProtocol() must happen _before_ the Init().
 *
 *	client.SetPort( somefunctionof( client.GetPort() ) ); //optional
 *	client.SetProtocol( "var", "value" ); //optional
 *
 *	client.Init( e );
 *
 *	// GetClient() must happen _after_ the Init().
 *
 *	client.SetClient( somefunctionof( client.GetClient() ) ); //optional
 *	client.SetCwd( somefunctionof( client.GetCwd() ) ); //optional
 *	client.SetUser( somefunctionof( client.GetUser() ) ); //optional
 *
 *	while( !client.Dropped() )
 *	{
 *	    client.SetArgv( argc, argv )
 *	    client.Run( func, &ui )
 *	}
 *
 *	return client.Final( e );
 *
 * Public methods:
 *
 *	ClientApi::SetUi() - reset the ClientUser object used
 *	ClientApi::SetMerger() - specify a custom ClientMerger (unsupported)
 *	ClientApi::SetProtocol() - ask for special server treatment
 *	ClientApi::GetProtocol() - get a protocol capability
 *		SetProtocol() is called before Init(); GetProtocol() after.
 *
 *	ClientApi::Init() - establish connection and prepare to run commands.
 *
 *	ClientApi::SetVar() - set variable
 *	ClientApi::SetVarV() - set variable using var=value syntax
 *	ClientApi::SetArgv() - set unnamed variables (args for Run())
 *	ClientApi::GetVar() - get variable
 *
 *	ClientApi::Run() - run a single command
 *	ClientApi::Final() - clean up end of connection, returning error count.
 *	ClientApi::Dropped() - check if connection is no longer serviceable
 *
 *	ClientApi::RunTag() - run a single command (potentially) asynchronously.
 *	ClientApi::WaitTag() - wait for a RunTag()/all RunTag()s to complete.
 *
 *	ClientApi::SetCwd()
 *	ClientApi::SetClient()
 *	ClientApi::SetHost()
 *	ClientApi::SetPassword()
 *	ClientApi::SetPort()
 *	ClientApi::SetUser() - set current directory, client, host, port, or 
 *		user, overridding all defaults.  SetPort() must be called 
 *		before Init() in order to take effect.  The others must be 
 *		set before Run() to take effect.
 *
 *		SetCwd() additionally searches for a new P4CONFIG file.
 *
 *	ClientApi::DefineClient()
 *	ClientApi::DefineHost()
 *	ClientApi::DefinePassword()
 *	ClientApi::DefinePort()
 *	ClientApi::DefineUser() - sets client, port, or user in the registry
 *		and (so as to take permanent effect) then calls SetClient(),
 *		etc. to take immediate effect.
 *
 *	ClientApi::GetCwd()
 *	ClientApi::GetClient()
 *	ClientApi::GetHost()
 *	ClientApi::GetOs()
 *	ClientApi::GetPassword()
 *	ClientApi::GetPort()
 *	ClientApi::GetUser() - get current directory, client, OS, port or user,
 *		as determined by defaults or by corresponding set value.
 *		GetClient()/GetHost() are not valid until after Init() 
 *		establishes the connection, because the hostname of the 
 *		local endpoint may serve as the default client name.
 */

class Client;
class ClientUser;
class ClientMerger;

class ClientApi : public StrDict {

    public:
	// caller's main interface

			ClientApi();
			~ClientApi();

	void		SetMerger( ClientMerger *m );

	void		SetProtocol( char *p, char *v );
	void		SetProtocolV( const char *p );
	StrPtr *	GetProtocol( const char *v );

	void		Init( Error *e );
	void		Run( const char *func, ClientUser *ui );
	int		Final( Error *e );
	int		Dropped();

	void		RunTag( const char *func, ClientUser *ui );
	void		WaitTag( ClientUser *ui = 0 );

	void		SetCwd( const char *c );
	void		SetClient( const char *c );
	void		SetHost( const char *c );
	void		SetPassword( const char *c );
	void		SetPort( const char *c );
	void		SetUser( const char *c );

	void		SetCwd( const StrPtr *c );
	void		SetClient( const StrPtr *c );
	void		SetHost( const StrPtr *c );
	void		SetPassword( const StrPtr *c );
	void		SetPort( const StrPtr *c );
	void		SetUser( const StrPtr *c );

	void		DefineClient( const char *c, Error *e );
	void		DefineHost( const char *c, Error *e );
	void		DefinePassword( const char *c, Error *e );
	void		DefinePort( const char *c, Error *e );
	void		DefineUser( const char *c, Error *e );

	const StrPtr	&GetCwd();
	const StrPtr	&GetClient();
	const StrPtr	&GetHost();
	const StrPtr	&GetOs();
	const StrPtr	&GetPassword();
	const StrPtr	&GetPort();
	const StrPtr	&GetUser();

    public:
	// The old interface, where ui was held from the start

			ClientApi( ClientUser *ui );
	void		SetUi( ClientUser *i );
	void		Run( const char *func );

    private:
	// Our StrDict implementation
	// Set strdict.h for various GetVar/SetVar calls

	StrPtr 		*VGetVar( const StrPtr &var );
	void		VSetVar( const StrPtr &var, const StrPtr &val );

    private:
	Client		*client;	// wrapped up RPC
	ClientUser	*ui;		// the old way

} ;
