/*
 * Copyright 1997 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * ToolServerExec - Start up a copy of the MPW tool server, and send it a command.
 */
#ifndef __CLIENTUSERMAC_H__
#define __CLIENTUSERMAC_H__

void ToolServerExec( const char *command, Error *error );

/*
 * EditWithExternalEditor - Start up an external editor, and wait for it to quit.
 *
 * The editor is specified by the environment variable EDITOR.
 * If EDITOR is not defined, then the editor whose creator is EDITOR_SIGNATURE is used.
 * If that is not defined, then SimpleText is used.
 */

void EditWithExternalEditor( char * document, Enviro * enviro, Error * error, bool waitUntilDone = true );

void RunCommandInExternalP4CLIClient(
		StrArray * commands,
		const StrPtr & client,
		const StrPtr & host,
		const StrPtr & passwd,
		const StrPtr & port,
		const StrPtr & user,
		bool closeAfterCommand,
		Error * e );

#endif /* __CLIENTUSERMAC_H__ */