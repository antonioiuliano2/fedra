@:: makeall.cmd 	Build FEDRA for WindowsNT/2000/XP/2003 
@:: Gabriele Sirri (20-11-2003)

@ECHO OFF
setlocal

if /I '%1'=='clean' goto CLEAN
if /I '%1'=='chktgt' goto CHECKTARGET
if /I '%1'=='archive' goto ARCHIVE

IF '%1'=='-7' SET compilerver=-7
::----------------------------------------------------------------------
:START
	SET CURRENTDIR=%CD%

:: Archive old fedra binaries
	if exist lib\*.* call %0 ARCHIVE
	
ECHO Check if ROOT is installed 
	if not defined ROOTSYS goto ROOTSYS_NOT_DEFINED
	if not exist %ROOTSYS%\bin\root.exe goto ROOT_DONT_EXIST
	ECHO ok!

ECHO Check the version of ROOT
       IF NOT EXIST workspace\obj\rootver.txt GOTO ROOTISCHANGED
       dir %ROOTSYS%\bin |find "." > rootver.temp
       fc workspace\obj\rootver.txt rootver.temp > temp.temp
       if %ERRORLEVEL%==0 GOTO ROOTISNOTCHANGED

	:ROOTISCHANGED
		ECHO The version of ROOT has been changed, I will remove FEDRA binaries...
		call %0 clean
		goto ROOTCHECKEND
	:ROOTISNOTCHANGED
		ECHO ok!
	:ROOTCHECKEND
		if exist temp.temp del temp.temp
		if exist rootver.temp del rootver.temp
::-----------------------------------------------------------------------------
:BUILDFEDRA
	if not exist workspace\obj mkdir workspace\obj 
	dir %ROOTSYS%\bin |find "." > workspace\obj\rootver.txt

ECHO Remove old FEDRA environment variables
	cd tools
	call setfedra -u -d
	cd %CURRENTDIR%

ECHO Build FEDRA
	cd workspace
	call make.cmd	libEdb	%compilerver%
	call make.cmd	libEmath	%compilerver%
	call make.cmd	libEdr	%compilerver%
	call make.cmd	libEdd	%compilerver%
	call make.cmd	rwc2edb	%compilerver%
	call make.cmd	recset	%compilerver%
	echo FOR %%1 %%%%F IN (*.rwc) DO rwc2edb %%%%f %%%%f.root > ..\bin\convertall.cmd
	cd %CURRENTDIR%

	ECHO.
ECHO Set the environment variables
	ECHO -----------------------------
	ECHO.
	cd tools
	call setfedra -u
	cd %CURRENTDIR%
	set FEDRA=%CURRENTDIR%
	set path=%CURRENTDIR%\bin;%CURRENTDIR%\lib;%PATH% 
	ECHO Environment variables have been changed only for the current user.
	ECHO Other users should set their own variables using setfedra utility.
	ECHO.

ECHO Check Targets:
ECHO --------------
call %0 chktgt lib\libEdb.lib
call %0 chktgt lib\libEdb.dll
call %0 chktgt lib\libEmath.lib
call %0 chktgt lib\libEmath.dll
call %0 chktgt lib\libEdr.lib
call %0 chktgt lib\libEdr.dll
call %0 chktgt lib\libEdd.lib
call %0 chktgt lib\libEdd.dll
call %0 chktgt bin\rwc2edb.exe
call %0 chktgt bin\recset.exe

	ECHO.
	ECHO NOTES:
	ECHO Example: load libEdb library in ROOT/CINT:
	ECHO          Root[0] gSystem.Load("%%FEDRA%%\\lib\\libEdb.dll");
	ECHO.
	
	pause
	goto STOP

::------ SUBROUTINES

::----------------------------------------------------------------------
:ROOTSYS_NOT_DEFINED
	ECHO.
        ECHO ERROR: the ROOTSYS environment variable has not been set. Please read 
	ECHO        the installation note of ROOT to run on the Windows command prompt 
	ECHO. 
	goto STOP
::----------------------------------------------------------------------
:ROOT_DONT_EXIST 
	ECHO.
        ECHO ERROR: root.exe doesn't exist in %%ROOTSYS%%\bin\root.exe
	ECHO. 
	goto STOP
::----------------------------------------------------------------------
:CLEAN
	ECHO Deleting FEDRA binaries...
	IF EXIST bin RD /S/Q bin
	IF EXIST lib RD /S/Q lib
	IF EXIST workspace\obj RD /S/Q workspace\obj
	IF EXIST workspace\rootcompare.txt DEL workspace\rootcompare.txt
	ECHO ok!
	goto STOP
::----------------------------------------------------------------------
:CHECKTARGET
        IF EXIST     %2 ECHO 	%2...ok!
        IF NOT EXIST %2 ECHO 	%2...ERROR
	  goto STOP
::----------------------------------------------------------------------
:ARCHIVE
   :LOOP
	SET Choice=
	SET /P Choice=Previous FEDRA binaries exist, do you want to archive these files (Y/N)? 
	IF NOT '%Choice%'=='' SET Choice=%Choice:~0,1%
	ECHO.
	IF /I '%Choice%'=='Y' GOTO SAVEARCHIVE
	IF /I '%Choice%'=='N' GOTO STOP
   GOTO LOOP
	
   :SAVEARCHIVE
   ECHO.
   ECHO The files will be saved in %cd%\old\"folder"
	SET Choice=
	SET /P Choice=Insert the folder name:
	ECHO.
	mkdir old
	mkdir old\%Choice%
	mkdir old\%Choice%\bin
	mkdir old\%Choice%\lib
	mkdir old\%Choice%\tools
	move  bin\*.*   old\%Choice%\bin
	move  lib\*.*   old\%Choice%\lib
	copy  tools\*.* old\%Choice%\tools 
	ECHO Files saved. If you want to use these files, change the environment variables
	ECHO using %CD%\old\%Choice%\tools\setfedra -u
	echo.
	pause
	echo.
	goto STOP
::----------------------------------------------------------------------
:STOP
endlocal
