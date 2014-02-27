/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * ClientUser -- default implementation of user interface for p4 client
 *
 * This file implements the p4 client command line interface by defining
 * the methods of the ClientUser class.  This interface reads from stdin,
 * writes to stdout and stderr, and invokes the user's editor.
 */

# define NEED_FILE

# ifdef OS_NT
# include <process.h>
# define NEED_FCNTL
# endif

# include <stdhdrs.h>

# include <strbuf.h>
# include <strops.h>
# include <strdict.h>
# include <enviro.h>
# include <error.h>

# include <signaler.h>
# include <filesys.h>
# include <diff.h>

# include "clientuser.h"
# include "clienterror.h"

# ifdef OS_MAC
# include <strarray.h>
# include "clientusermac.h"
# endif

/*
 * NoEcho - turn on/off echoing
 */

// These next few lines will get ported to hell

# ifdef OS_NT
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
typedef DWORD TIO_T;
# define hstdin (HANDLE)_get_osfhandle( fileno( stdin ) )
# define TIO_GET(tio) GetConsoleMode( hstdin, &tio )
# define TIO_SET(tio) SetConsoleMode( hstdin, tio )
# define TIO_NOECHO(tio) tio &= ~ENABLE_ECHO_INPUT
# endif

# if defined( OS_OS2 ) || defined( OS_MAC ) || \
	defined( OS_VMS ) || defined( OS_AS400 ) || \
	defined( OS_MPEIX )
// no ops
typedef int TIO_T;
# define hstdin fileno( stdin )
# define TIO_GET(tio) {}
# define TIO_SET(tio) {}
# define TIO_NOECHO(tio) {}
# endif

// For now, default to termios

# ifndef TIO_SET
# define USE_TERMIOS
# endif

# ifdef USE_TERMIOS
# include <termios.h>
typedef struct termios TIO_T;
# define TIO_GET(tio) tcgetattr( fileno( stdin ), &tio )
# define TIO_SET(tio) tcsetattr( fileno( stdin ), TCSANOW, &tio )
# define TIO_NOECHO(tio) tio.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL)
# endif

# ifdef USE_SGTTY
# include <sgtty.h>
typedef struct sgttyb TIO_T;
# define TIO_GET(tio) ioctl( fileno( stdin ), TIOCGETP, &tio )
# define TIO_SET(tio) ioctl( fileno( stdin ), TIOCSETP, &tio )
# define TIO_NOECHO(tio) tio.sg_flags &= ~(ECHO)
# endif

class NoEcho;

void EchoCleanup( NoEcho *noEcho );

class NoEcho {

    public:
	NoEcho()
	{
	    TIO_GET( tio );
	    tio2 = tio;
	    TIO_NOECHO( tio );
	    TIO_SET( tio );
	    signaler.OnIntr( (SignalFunc)EchoCleanup, this );
	}

	~NoEcho()
	{
	    TIO_SET( tio2 );
	    fputc( '\n', stdout );
	    signaler.DeleteOnIntr( this );
	}

    private:
	TIO_T tio, tio2;

} ;

void 
EchoCleanup( NoEcho *noEcho )
{
	delete noEcho;
}

/*
 * ClientUser::Prompt() - prompt the user, and wait for a response.
 */

void
ClientUser::Prompt( const StrPtr &msg, StrBuf &buf, int noEcho, Error *e )
{
	printf( msg.Text() );
# ifdef OS_MAC
	fputc( '\n', stdout );
# endif

	fflush( stdout );
	fflush( stdin );

	// Turn off echoing

	NoEcho *setEcho = noEcho ? new NoEcho : 0;

	// Prompt'em

	buf.Clear();
	char *b = buf.Alloc( 1024 );

	if( !fgets( b, 1024, stdin ) )
	{
	    e->Set( ErrClient::Eof );
	    buf.SetEnd( b );
	}
	else
	{
	    // Set end and truncate any newline

	    buf.SetEnd( b += strlen( b ) );

	    if( buf.Length() && buf.End()[ -1 ] == '\n' )
	    {
		buf.SetEnd( buf.End() - 1 );
		buf.AppendNull();
	    }
	}

	delete setEcho;
}

void
ClientUser::Help( const_char *const *help )
{
	for( ; *help; help++ )
	    printf( "%s\n", *help );
}

void
ClientUser::HandleError( Error *err )
{
	// Dumb implementation is just to format 
	// the error and call (old) OutputError.

	// API callers against 99.1+ servers can do better by
	// having HandleError probe 'err' directly for detail.

	StrBuf msg;
	err->Fmt( &msg );
	OutputError( msg.Text() );
}

void
ClientUser::OutputError( const_char *errBuf )
{
	fwrite( errBuf, 1, strlen( errBuf ), stderr );
}

void
ClientUser::OutputInfo( char level, const_char *data )
{
	switch( level )
	{
	default:
	case '0': break;
	case '1': printf( "... " ); break;
	case '2': printf( "... ... " ); break;
	}

	fwrite( data, 1, strlen( data ), stdout );
	fputc( '\n', stdout );
}

void
ClientUser::OutputText( const_char *data, int length )
{
	fwrite( data, 1, length, stdout );
}

void
ClientUser::OutputBinary( const_char *data, int length )
{
# ifdef OS_NT
	// we rely on a trailing zero length buffer to
	// tell us to turn off binary output.

	if( binaryStdout == !length )
	{
	    // toggle
	    binaryStdout = !!length;
	    fflush( stdout );
	    setmode( fileno( stdout ), binaryStdout ? O_BINARY : O_TEXT );
	}
# endif

	fwrite( data, 1, length, stdout );
}

void
ClientUser::InputData( StrBuf *buf, Error *e )
{
	int n;

	buf->Clear();

	do {
# ifdef OS_MAC
	    // CW PRO 5 fread/read from stdin hangs machine
	    char *b = buf->Alloc( FileBufferSize + 1 );
	    n = fgets( b, FileBufferSize + 1, stdin ) ? strlen( b ) : 0;
# else
	    char *b = buf->Alloc( FileBufferSize );
	    n = read( 0, b, FileBufferSize);
# endif
	    buf->SetEnd( b + n );
	} while( n > 0 );

	buf->AppendNull();
}

void
ClientUser::OutputStat( StrDict *varList )
{
	int i;
	StrBuf msg;
	StrPtr var, val;

	// Dump out the variables, using the GetVar( x ) interface.
	// Don't display the function (duh), which is only relevant to rpc.

	for( i = 0; varList->GetVar( i, var, val ); i++ )
	{
	    if( var == "func" ) continue;

	    // otherAction and otherOpen go at level 2, as per 99.1 + earlier

	    msg.Clear();
	    msg << var << " " << val;
	    char level = strncmp( var.Text(), "other", 5 ) ? '1' : '2';
	    OutputInfo( level, msg.Text() );
	}

	// blank line

	OutputInfo( '0', "" );
}

void
ClientUser::ErrorPause( char *errBuf, Error *e )
{
	StrBuf buf;
	OutputError( errBuf );
# if defined( OS_MAC )
	Prompt( StrRef( "Hit enter to continue..." ), buf, 0, e );
# else
	Prompt( StrRef( "Hit return to continue..." ), buf, 0, e );
# endif
}

void
ClientUser::Edit( FileSys *f1, Error *e )
{
	Edit( f1, enviro, e );
}

void
ClientUser::Edit( FileSys *f1, Enviro * env, Error *e )
{
	// The presence of $SHELL on NT implies MKS, which has vi.
	// On VMS, we look for POSIX$SHELL - if set, we'll use vi.
	// Otherwise, EDIT.

	if( !f1->IsTextual() )
	{
	    e->Set( ErrClient::CantEdit ) << f1->Name();
	    return;
	}

# ifdef OS_MAC
	EditWithExternalEditor( f1->Name(), env, e );
# else
	StrBuf n;
	const char *editor;

	if( !( editor = env->Get( "P4EDITOR" ) ) )
	    editor = getenv( "EDITOR" );

# ifdef OS_NT
	if( !editor && !getenv( "SHELL" ) )
	    editor = "notepad";
# endif

# ifdef OS_VMS
	if( !editor && !getenv( "POSIX$SHELL" ) )
	    editor = "edit";
# endif

# ifdef OS_AS400
  if( !editor )
      editor = "edtf";
# endif

	// Bill Joy rules, Emacs maggots!

	if( !editor )
	    editor = "vi";

	RunCmd( editor, f1->Name(), 0, 0, 0, 0, e );

# endif /* OS_MAC */
}

void
ClientUser::Diff( FileSys *f1, FileSys *f2, int doPage, char *df, Error *e )
{
	if( !f1->IsTextual() || !f2->IsTextual() )
	{
	    if( f1->Compare( f2, e ) )
		printf( "(... files differ ...)\n" );
	    return;
	}

	// Call diff to do text compare

	const char *diff;
	const char *pager;

	if( !( diff = enviro->Get( "P4DIFF" ) ) )
	    diff = getenv( "DIFF" );

	if( !doPage )
	    pager = 0;
	else if( !( pager = enviro->Get( "P4PAGER" ) ) )
	    pager = getenv( "PAGER" );

	// Do our own diff
	// VMS ReadFile is busted.

# ifdef OS_VMS

	if( !diff )
	    diff = "diff";

# else /* not VMS */

	if( !diff )
	{
	    if( pager )
	    {
		FileSys *t = FileSys::CreateGlobalTemp( f1->GetType() );

		{
# ifndef OS_NEXT
		    ::
# endif
		    Diff d;
		    d.SetInput( f1->Name(), f2->Name(), df, e );
		    if( !e->Test() ) d.SetOutput( t->Name(), e );
		    if( !e->Test() ) d.DiffWithFlags( df );
		    d.CloseOutput( e );
		}

		if( !e->Test() ) RunCmd( pager, t->Name(), 0, 0, 0, 0, e );

		delete t;
	    }
	    else
	    {
# ifndef OS_NEXT
		::
# endif
	    	Diff d;

		d.SetInput( f1->Name(), f2->Name(), df, e );
		if( !e->Test() ) d.SetOutput( stdout );
		if( !e->Test() ) d.DiffWithFlags( df );
		d.CloseOutput( e );
	    }

	    return;
	}

# endif /* VMS */

	// Build up flags args
	// Yuk.

	if( !df || !*df )
	{
	    RunCmd( diff, 0, f1->Name(), f2->Name(), 0, pager, e );
	}
	else
	{
	    StrBuf flags;
	    flags.Set( "-", 1 );
	    flags << df;
	    RunCmd( diff, flags.Text(), f1->Name(), f2->Name(), 0, pager, e );
	}
}

void
ClientUser::Merge( 
	FileSys *base,
	FileSys *leg1,
	FileSys *leg2,
	FileSys *result,
	Error *e )
{
	char *merger;
	
	if( !( merger = enviro->Get( "P4MERGE" ) ) && 
	    !( merger = getenv( "MERGE" ) ) )
	{
	    e->Set( ErrClient::NoMerger );
	    return;
	}

	RunCmd( merger, base->Name(), leg1->Name(), 
		leg2->Name(), result->Name(), 0, e );
}

void
ClientUser::RunCmd(
	const char *command,
	const char *arg1,
	const char *arg2,
	const char *arg3,
	const char *arg4,
	const char *pager,
	Error *e )
{
	StrBuf n;

	fflush( stdout );

	signaler.Block();	// reset SIGINT to SIGDFL

# ifdef OS_MAC

	n << command;

	if( arg1 ) n << " '" << arg1 << "'";
	if( arg2 ) n << " '" << arg2 << "'";
	if( arg3 ) n << " '" << arg3 << "'";
	if( arg4 ) n << " '" << arg4 << "'";

	ToolServerExec( n.Text(), e );

# else
# ifdef OS_NT

	if( !pager )
	{
	    n << command;

	    const char * argv[32];
	    int c = StrOps::Words( n, (char **)argv, 32 );

	    if( arg1 ) argv[ c++ ] = arg1;
	    if( arg2 ) argv[ c++ ] = arg2;
	    if( arg3 ) argv[ c++ ] = arg3;
	    if( arg4 ) argv[ c++ ] = arg4;

	    argv[ c++ ] = 0;

	    if( spawnvp( _P_WAIT, argv[0], argv ) < 0 )
		e->Sys( argv[0], arg1 );

	    signaler.Catch();	// catch SIGINT again

	    return;
	}

# endif /* OS_NT */

# ifdef OS_VMS
# define QUOTE ""
# else
# ifdef OS_OS2
# define QUOTE "\""
# else
# define QUOTE "'"
# endif
# endif

	n << command;

	if( arg1 ) n << " " QUOTE << arg1 << QUOTE;
	if( arg2 ) n << " " QUOTE << arg2 << QUOTE;
	if( arg3 ) n << " " QUOTE << arg3 << QUOTE;
	if( arg4 ) n << " " QUOTE << arg4 << QUOTE;
	if( pager ) n << " | " << pager;

	system( n.Text() );

# endif /* OS_MAC */

	signaler.Catch();	// catch SIGINT again
}

FileSys *
ClientUser::File( FileSysType type )
{
	return FileSys::Create( type );
}
