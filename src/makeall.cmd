:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server
::
:: usage:
::       makeall.cmd           - build all targets
::       makeall.cmd  debug    - build all targets with debug informations
::       makeall.cmd  check    - check if targets exist
::       makeall.cmd  checkall - check if targets exist [old]
::       makeall.cmd  clean    - clean all targets and intermediate files
::       makeall.cmd  depend   - create all dependencies *******not implemented yet

::-------------------------------------------
:MAIN
   @ECHO OFF
   SET eLIBS= libEmath libEdb libDataConversion libVt++ libEphys libEGA libEdr libEIO libEdd libEMC libAlignment libScan libEOracle libACQ libShower appl\bmatrix libEGraphTool libEmr 
   SET eBINS=appl\recset appl\rwc2edb appl\macros appl\display appl\o2root appl\comptonmap 

   IF /I '%1'=='check' (
      GOTO CHECKALL
   ) ELSE IF /I '%1'=='debug' (
      GOTO MAKEALLDEBUG
   ) ELSE (
      GOTO MAKEALL
   )

GOTO END

::-------------------------------------------
:MAKEALL
   IF not defined VSINSTALLDIR call vsvars32.bat 
   FOR %%f IN (%eLIBS% %eBINS%) DO ( 
      IF '%1'=='' (
		ECHO.
      	ECHO - - - - - - - - - - - - %%f - - - - - - - - - - - - - -
	)
      pushd %%f
      IF EXIST Makefile.w32 (
         nmake %1 /NOLOGO /F Makefile.w32
      ) ELSE (
         nmake %1 /NOLOGO
      )
      popd
   )
   echo.
   IF '%1'=='' CALL %0 CHECK

   (
	echo compiled by %USERDOMAIN%\%USERNAME% on %COMPUTERNAME% ^(%DATE% %TIME%^)
        echo    NMAKE options/macrodefs/targets: %1
        nmake /NOLOGO /P | find "_NMAKE_VER"
        for /F "tokens=3" %%i IN ('findstr /c:"#define ROOT_RELEASE " %%ROOTSYS%%\include\RVersion.h') DO echo    ROOT VERSION %%i
   ) >  "%~dp0"\makeall-report.txt


GOTO END

::-------------------------------------------
:MAKEALLDEBUG
   IF not defined VSINSTALLDIR call vsvars32.bat 
   FOR %%f IN (%eLIBS% %eBINS%) DO ( 
	ECHO.
      ECHO - - - - - - - - - - %%f [Debug configuration]- - - - - - - -
      pushd %%f
      IF EXIST Makefile.w32 (
         nmake CFG="Debug" /NOLOGO /F Makefile.w32
      ) ELSE (
         nmake CFG="Debug" /NOLOGO
      )
      popd
   )
   echo.
   CALL %0 CHECK

   (
	echo compiled by %USERDOMAIN%\%USERNAME% on %COMPUTERNAME% ^(%DATE% %TIME%^)
        echo    NMAKE options/macrodefs/targets: CFG="Debug"
        nmake /NOLOGO /P | find "_NMAKE_VER"
        for /F "tokens=3" %%i IN ('findstr /c:"#define ROOT_RELEASE " %%ROOTSYS%%\include\RVersion.h') DO echo    ROOT VERSION %%i
   ) >  "%~dp0"\makeall-report.txt


GOTO END

::-------------------------------------------
:CHECKALL
   ECHO Check Targets:
   ECHO --------------
   FOR %%f IN (%eLIBS% %eBINS%) DO ( 
      pushd %%f
      IF EXIST Makefile.w32 (
         nmake check /NOLOGO /F Makefile.w32
      ) ELSE (
         nmake check /NOLOGO
      )
      popd
   )
   echo.

GOTO END

::-------------------------------------------
:END

