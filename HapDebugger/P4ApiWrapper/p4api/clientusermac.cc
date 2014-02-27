/*
 * Copyright 1997 Perforce Software.  All rights reserved.
 * Copyright 1997 Bent Spoon Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */
# include <stdhdrs.h>

# include <error.h>
# include <strarray.h>
# include <enviro.h>

# include <AppleEvents.h>

# include <macfilespec.h>
# include <Processes.h>

# include "clienterror.h"
# include <strbuf.h>
# include <filesys.h>
# include <macutil.h>
# include <clientusermac.h>

/*
 * Information about MPW ToolServer.
 */

const OSType kMPWToolServerSignature = 'MPSX'; // signature of the ToolServer application
const AEEventClass kAEMPWEventClass = 'MPS '; // class of the single Apple Event
const AEEventID kAEMPWDoScript = 'scpt'; // id of the single Apple Event

const OSType kP4SIOUXToolSignature = 'p4st'; // signature of the P4SIOUXTool application

/*
 * Information about SimpleText.
 */

const OSType kSimpleTextSignature = 'ttxt'; // signature of the SimpleText application

/*
 * Default editor application is SimpleText.
 */

const OSType kDefaultEditorSignature = kSimpleTextSignature;

void
ToolServerExec( const char *command, Error *e )
{
	// If we're running under toolserver, abort with an error.
	
	if( getenv( "BackgroundShell" ) )
	{
	    e->Set( ErrClient::ToolServer2 );
	    return;
	}
	
	// First, we need to get tool server.
	
	ProcessSerialNumber process;
	if( !FindProcessBySignature( kMPWToolServerSignature, &process ) )
	{
	    if( LaunchApplicationBySignature( kMPWToolServerSignature, 0, &process ) != noErr )
	    {
		e->Set( ErrClient::ToolServer );
		return;
	    }
	}
	
	// Create an empty Apple Event targeted at the tool server.
	
	AppleEvent commandEvent;
	if( CreateSimpleAppleEvent( kAEMPWEventClass, kAEMPWDoScript,
				&process, &commandEvent ) != noErr )
	{
	    e->Set( ErrClient::ToolCmdCreate );
	    return;
	}
	
	// Add the command to the event.
	
	if( AddSimpleParameterToAppleEvent( &commandEvent, keyDirectObject,
					typeChar, command, strlen(command) ) != noErr )
	{
	    AEDisposeDesc( &commandEvent );
	    
	    e->Set( ErrClient::ToolCmdCreate );
	    return;
	}

	// Send the event.
	
	OSErr err = SendAppleEventWaitForReply( &commandEvent );	
	AEDisposeDesc( &commandEvent );
	
	if( err != noErr )
	{
	    e->Set( ErrClient::ToolCmdSend );
	    return;
	}
}

void
EditWithExternalEditor( char * document, Enviro * enviro, Error * e, bool waitUntilDone )
{
	char *editor;

	// Locate the document.
	
	FSSpec documentFile;
	FSSpecFromPath( document, &documentFile );
	

	// Start up the editor.
	
	ProcessSerialNumber process;
	
	
	// Check if we can start it by name.
	
	if( !( editor = enviro->Get( "P4EDITOR" ) ) )
	    editor = getenv( "EDITOR" );

	if( editor )
	{
	    // Start the editor and open the document.
	    
	    FSSpec editorFile;
	    FSSpecFromPath( editor, &editorFile );
	    
	    if( FindProcessByFile( &editorFile, &process ) )
	    {
	    	SendOpenDocumentEvent( &process, &documentFile );
		SetFrontProcess( &process );
	    }
	    else
	    {
	        LaunchApplicationByFile( &editorFile, &documentFile, &process );
	    }
	}
	else
	{
	    OSType editorSignature;
	    
	    // Extract the signature.
	    
	    char *signatureAsString = getenv( "EDITOR_SIGNATURE" );
	    if( signatureAsString )
	    {
	    	if( strlen( signatureAsString ) != 4 )
		{
		    e->Set( ErrClient::BadSignature );
		    return;
		}
		editorSignature = (signatureAsString[0] & 0xFF) << 24
				| (signatureAsString[1] & 0xFF) << 16
				| (signatureAsString[2] & 0xFF) << 8
				| (signatureAsString[3] & 0xFF);
	    }
	    else
	    {
	    	// Use SimpleText.
	    
	    	editorSignature = kDefaultEditorSignature;
	    }
	    
	    // Start the editor and open the document.
	    
	    if( FindProcessBySignature( editorSignature, &process ) )
	    {
	    	SendOpenDocumentEvent( &process, &documentFile );
			SetFrontProcess( &process );
	    }
	    else
	    {
	        LaunchApplicationBySignature( editorSignature, &documentFile, &process );
	    }
	}
	
	// Get out if there was an error.
	
	if( e->Test() )
	{
	    return;
	}
	
	// We're done when the editor quits.
	if ( waitUntilDone )
	{
		WaitForApplicationToQuit( &process );
	}
}


void RunCommandInExternalP4CLIClient(
		StrArray * commands,
		const StrPtr & client,
		const StrPtr & host,
		const StrPtr & passwd,
		const StrPtr & port,
		const StrPtr & user,
		bool closeAfterCommand,
		Error * e )
{
	Boolean wasRunning = MacUtil_IsProcessRunning( kP4SIOUXToolSignature );
	
	
	
	//
	// Start it up if it's not already going
	//
	if ( !wasRunning )
	{
		FSSpec spec;
		if ( FindApplicationBySignature( kP4SIOUXToolSignature, &spec ) )
		{
			MacUtil_launch( &spec, true );
		}
		else
		{
			e->Set( ErrClient::CantFindApp );
			goto bail;
		}
	}
	
	{
		ProcessInfoRec tempRec;
		
		if ( MacUtil_FindProcess( kP4SIOUXToolSignature, &tempRec ) == noErr )
		{
			//
			// Make it the foreground process
			//
			MacUtil_SetFrontProcess( kP4SIOUXToolSignature );
			
			//
			// For every environment setting, pass it to the new process
			//
			MacUtil_SendSetEnvAppleEvent( kP4SIOUXToolSignature, "P4CLIENT", client.Text(), false, false, NULL);
			MacUtil_SendSetEnvAppleEvent( kP4SIOUXToolSignature, "P4HOST", host.Text(), false, false, NULL);
			MacUtil_SendSetEnvAppleEvent( kP4SIOUXToolSignature, "P4PASSWD", passwd.Text(), false, false, NULL);
			MacUtil_SendSetEnvAppleEvent( kP4SIOUXToolSignature, "P4PORT", port.Text(), false, false, NULL);
			MacUtil_SendSetEnvAppleEvent( kP4SIOUXToolSignature, "P4USER", user.Text(), false, false, NULL);
			
			
			//
			// Run the command
			//
			MacUtil_SendMainMethodAppleEvent( kP4SIOUXToolSignature, commands, true, false, NULL);
		}
		else
		{
			goto bail;
		}
	}
	
	if ( !wasRunning && closeAfterCommand )
	{
		MacUtil_SendQuitAppleEvent( kP4SIOUXToolSignature, false );
	}
		
bail:
	;
}