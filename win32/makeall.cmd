@:: makeall.cmd 	Build FEDRA for WindowsNT/2000/XP 
@:: Gabriele Sirri (31-10-2003)

@ECHO OFF
setlocal

if /I '%1'=='clean' goto CLEAN

::----------------------------------------------------------------------
:BUILD

	SET CURRENTDIR=%CD%
:: Check if ROOT is well installed
	if not defined ROOTSYS goto ROOTSYS_NOT_DEFINED
	if not exist %ROOTSYS%\bin\root.exe goto ROOT_DONT_EXIST

:: Archive old FEDRA binaries
	if not exist bin\recset.exe goto NOTARCHIVE
	
   :LOOP
	SET Choice=
	SET /P Choice=Previous FEDRA binaries exist, do you want to archive them (Y/N)? 
	IF NOT '%Choice%'=='' SET Choice=%Choice:~0,1%
	ECHO.
	IF /I '%Choice%'=='Y' GOTO SAVEARCHIVE
	IF /I '%Choice%'=='N' GOTO NOTARCHIVE
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

:NOTARCHIVE

ECHO Remove old FEDRA environment variables
	cd tools
	call setfedra -u -d
	cd %CURRENTDIR%

ECHO Build FEDRA
	cd workspace
	call libEdb.mak.cmd
	call libEmath.mak.cmd
	call libEdr.mak.cmd
	call rwc2edb.mak.cmd
	call recset.mak.cmd
	ECHO Copying convertall.cmd ...
	copy convertall.cmd ..\bin
	cd %CURRENTDIR%

	ECHO.
ECHO Set the environment variables
	ECHO -----------------------------
	ECHO.
	cd tools
	call setfedra -u
	cd %CURRENTDIR%
	ECHO Environment variables have been changed only for the current user.
	ECHO Other users should set their own variables using setfedra utility.
	ECHO.

	ECHO NOTES:
	ECHO Example: load libEdb library in ROOT/CINT:
	ECHO          Root[0] gSystem.Load("%%FEDRA%%\\lib\\libEdb.dll");
	ECHO.
	
	pause
	goto STOP
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
        ECHO ERROR: root.exe not exist in %%ROOTSYS%%\bin\root.exe
	ECHO. 
	goto STOP
::----------------------------------------------------------------------
:CLEAN
	ECHO Deleting FEDRA binaries...
	IF EXIST bin RD /S/Q bin
	IF EXIST lib RD /S/Q lib
	IF EXIST workspace\obj RD /S/Q workspace\obj
	ECHO ok!
	goto STOP
::----------------------------------------------------------------------
:STOP
endlocal
