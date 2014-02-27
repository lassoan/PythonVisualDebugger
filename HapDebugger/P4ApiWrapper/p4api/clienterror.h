/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * clienterror.h - definitions of errors for client subsystem.
 */

class ErrClient {

    public:

	static ErrorId Connect;
	static ErrorId BadFlag;
	static ErrorId Fatal;
	static ErrorId ClobberFile;
	static ErrorId MkDir;
	static ErrorId Eof;
	static ErrorId CantEdit;
	static ErrorId NoMerger;

	static ErrorId MacError;
	static ErrorId ToolServer2;
	static ErrorId ToolServer;
	static ErrorId ToolCmdCreate;
	static ErrorId ToolCmdSend;
	static ErrorId Memory;
	static ErrorId CantFindApp;
	static ErrorId BadSignature;

} ;
