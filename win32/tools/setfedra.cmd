@ECHO OFF
:: SetFedra.cmd           Set variables for FEDRA
:: Gabriele Sirri (31-10-2003)
:: 
:: OBSOLETE !!!!!!! used by install.cmd to remove the old variables

	IF '%1'=='-m'       goto MACHINESETTINGS
	IF '%1'=='-u'       goto USERSETTINGS
goto HELP

setlocal
:: -------------------------------------
::Variables in the MACHINE environment
:MACHINESETTINGS
	ECHO Change settings for the MACHINE environment
	SET SETOPT=m
	SET PATHOPT=s
	GOTO END1

::Variables in the USER environment
:USERSETTINGS
	ECHO Change settings for the USER environment
	SET SETOPT=u
	SET PATHOPT=u
	GOTO END1
:END1
:: -------------------------------------
	IF '%2'=='-delete'       goto REMOVEOPTIONS
	IF '%2'=='-d'       goto REMOVEOPTIONS

::Add options
:ADDOPTIONS
	  cd ..
	SET SETVALUE=%CD%
	SET SETADDREMOVE=
	  cd tools
	SET PATHADDREMOVE=a
		set FEDRA %SETVALUE%
		path %PATH%;%SETVALUE%\lib;%SETVALUE%\bin
	GOTO END2

::Remove options
:REMOVEOPTIONS
	SET SETVALUE=
        SET SETADDREMOVE=-delete
	SET PATHADDREMOVE=r
	GOTO END2
:END2
:: -------------------------------------
::Set the variables
	setenv    -%SETOPT% FEDRA %SETVALUE% %SETADDREMOVE%
	pathman /%PATHADDREMOVE%%PATHOPT% %%FEDRA%%\lib
	pathman /%PATHADDREMOVE%%PATHOPT% %%FEDRA%%\bin
endlocal
goto STOP

:: -------------------------------------
:HELP
ECHO setfedra.exe set environment variables for FEDRA
ECHO.
ECHO The variables can be set in the USER or in the MACHINE environment 
ECHO and can be removed with the -delete option
ECHO. 
ECHO usage: setfedra [-u or -m] [-delete]
ECHO.	
ECHO       options: -u  USER environment
ECHO                -m  MACHINE environment
ECHO                -d  remove the variables (also -delete)
ECHO.

:: -------------------------------------
:STOP