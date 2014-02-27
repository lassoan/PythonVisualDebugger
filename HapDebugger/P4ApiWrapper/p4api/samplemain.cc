/*
 * samplemain.cc - a p4 client API example
 *
 * This file is part of the p4api.tar distribution.
 *
 * This barebones example simply mimics the regular p4 command line
 * program.  
 *
 * Generally, compiling with the C++ compiler and linking with the
 * three provided libraries is sufficient to build this sample program.
 * 
 * See the Perforce Client API documentation (p4api.txt) at 
 * www.perforce.com/perforce/technical.html for further information.
 *
 * $Id: //depot/r01.1/p4/client/samplemain.cc#1 $
 */

# include "clientapi.h"

int main( int argc, char **argv );
int main( int argc, char **argv )
{
	ClientUser ui;
	ClientApi client;
	Error e;

	// Any special protocol mods

	// client.SetProtocol( "tag", "" );

	// Connect to server

	client.Init( &e );
	e.Abort();

	// Run the command "argv[1] argv[2...]"

	client.SetArgv( argc - 2, argv + 2 );
	client.Run( argv[1], &ui );

	// Close connection

	client.Final( &e );
	e.Abort();
	
	return 0;
}
