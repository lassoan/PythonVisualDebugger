@rem Keep environment changes local to this batch file
@setlocal

set destdir=c:\temp\haprelease

@rem Make VS 2010 tools available
call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"

@echo This batch file makes a release of the HapDebugger. It makes two
@echo .zip files, one for source and one for the binaries. It also
@echo builds the .zip directory to make sure all necessary files
@echo were included.

@rem Clean out the release directory.
rmdir /s/q %destdir%

@rem Copy the necessary components over, filtering out unneeded files.
xcopy /s/v/k/i /exclude:filter.txt . %destdir%\source
@rem Copy over three .lib files which get filtered out but are needed
xcopy P4ApiWrapper\p4api\*.lib %destdir%\source\P4ApiWrapper\p4api

@rem Make all files writeable
cd %destdir%
attrib -r *.* /s

@rem Zip everything up
zip20 -r HapDebuggerSrc.zip .

@rem Build everything to test that it works.
devenv source\hap.sln /build release

@rem Build again so that all of the error counts are easily visible.
@echo Rebuilding to generate error summary
devenv source\hap.sln /build release

mkdir bin
xcopy source\bin\HapClient.exe bin
xcopy source\bin\HapDebugger.exe bin
xcopy source\bin\hapdbg.dll bin
xcopy source\bin\scintilla.dll bin
xcopy source\bin\expat.dll bin
xcopy source\bin\P4ApiWrapper.dll bin
cd bin
zip20 ..\HapDebuggerBin.zip *.*
cd ..

@echo If no errors or warnings were detected, HapDebuggerSrc.zip contains a
@echo releasable version of the HapDebugger source, HapDebuggerBin.zip
@echo contains the binaries.

@echo To release a new version to SourceForge:
@echo Rename the binaries appropriately and ftp them to upload.sourceforge.net
@echo (login as "anonymous") and put the zip files in the /incoming directory.
@echo Then log in to the sourceforge website, go to the hap site, go to the admin
@echo page, then click on edit/release files - then click on add release for the HapDebugger
@echo sources (or binaries) - the rest is pretty self explanatory.

@echo The result .zip files can be found in %destdir%
