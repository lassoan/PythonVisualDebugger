HAP Python Remote Debugger Readme.txt
Copyright 2001 Humongous Entertainment

History:
	04 December 2001 - readme created for initial release
	February 2003 - updated
	
Overview:
The HAP (Humongous Addition to Python) Python Remote Debugger was created 
as a few people at Humongous Entertainment began using Python and it became 
clear that none of the available debuggers would fit our needs.  We were going to be 
using Python in a few ways that made the existing debuggers less than ideal:
	Our applications would run in full screen mode
	They would make use of multi threading
	They would be developed by people without extensive software development experience
	They would be developed for Windows, Macintosh and other platforms.
	
After initial experiments revealed that it would be possible to make our own debugger, we
started really liking how it was turning out.  Ultimately, we decided that it would be a good
chance to give something back to the Python community so we released the project as
an open source project.

The debugger is made up of two applications: DbgRemote is the editor and IDE, 
ConsoleEmbed is the remote debugging host - it runs the python script and 
communicates to the IDE via a network socket. The idea was that the 
ConsoleEmbed application would be ported to any platforms that we needed
to support while the DbgRemote app would be maintained only on the windows
platform.  This way we could use a single debugging interface to debug our python
applications regardless of what platform we were running them on.  This two piece
approach also allowed us to break into running python code in a more elegant way
than possible in the standard python debuggers.  

We hope that you find this project useful.  If you have comments or suggestions, please
write in - the developers mailing list on SourceForge is the best place to start.

The current release was built with Python 2.6 - it should be possible to build it for earlier
versions however.

Revision history:

February 2010
- fixed a requirement that the HapClient be built with the same version of VC++ as Python.
This restriction occurred because the HapClient was passing a FILE* to Python and the
layout of the FILE object changes between VC++ versions. This restriction was inconvenient
because it prevented upgrading HapClient to VS 2010. Using PyRun_SimpleString instead of
PyRun_SimpleFile to start the debugging session removed this dependency.
- Got everything to build mostly warning free with VS 2010
- Added VS 2008 and VS 2010 solution files (VS 2005 no longer supported). The VS 2010 solution
is recommended for future development as the VS 2008 solution may be out of date.
- Fixed erroneous use of namespace aliases
- Changed from stricmp/strnicmp/strlwr to the ISO C++ _stricmp/_strnicmp/_strlwr to suppress deprecation warnings
- Fixed some const-correctness errors
- Added pre-build checks for the existence of $PYTHON_ROOT\include\python.h
- Added a force-clearing of the PYTHONHOME environment variable due to problems it causes on
my machine. This line (in hapdebugger, AfxWinMain) may need removing on some people's machines.
- Fixed the function declarations for OnNcHitTest to match the new correctness
- Added and used a HAP_MIN template function to avoid namespace problems with min and MIN
- Added _CRT_SECURE_NO_WARNINGS to the preprocessor definitions to suppress lots of warnings,
which should probably be individually addressed at some point.
- Got debug builds to work by changing _DEBUG to NDEBUG and specifying the release CRT. The only
difference now between debug and release should be optimization. This change was needed because
a true debug build of the debugger would require a debug build of Python, which is unwieldy. The
P4ApiWrapper project doesn't build in debug for some reason, but the others all do.
- Fixed filter.txt to filter out new intermediate files like .ipch and .sdf files when building a release
- Fixed makerelease.bat to clean it up, support VS 2010, and fix numerous obsolete options.
- Fixed bug with files not in folders not getting saved, by forcing them into folders.
- Fixed infinite loop when dragging folders to the project root


Bugs to fix:

See KnownDefects.txt and todo.txt for lists of bugs.
