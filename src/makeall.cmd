:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server
::
:: usage:
::       makeall.cmd           - build all targets
::       makeall.cmd  checkall - check all targets
::       makeall.cmd  clean    - clean all targets and intermediate files
::       makeall.cmd  depend   - create all dependencies //not implemented yet

::-------------------------------------------
:MAIN
   @ECHO OFF
   SET eLIBS= libEmath libEdb libDataConversion libVt++ libEphys libEGA libEdr libEIO libEdd libEMC libEOracle libScan libACQ libShower appl\bmatrix libEGraphTool
   SET eBINS=appl\recset appl\rwc2edb appl\tracks2edb appl\checkrun appl\edbtools appl\display

   IF /I '%1'=='checkall' (
      GOTO CHECKALL
   ) ELSE (
      GOTO MAKEALL
   )

GOTO END

::-------------------------------------------
:MAKEALL
   IF not defined VSINSTALLDIR call vsvars32.bat 
   FOR %%f IN (%eLIBS% %eBINS%) DO ( 
      IF '%1'=='' ECHO.
      IF '%1'=='' ECHO - - - - - - - - - - - - %1 %%f - - - - - - - - - - - -
      pushd %%f
      IF EXIST Makefile.w32 (
         nmake %1 /NOLOGO /F Makefile.w32
      ) ELSE (
         nmake %1 /NOLOGO
      )
      popd
   )
   echo.
   IF '%1'=='' CALL %0 CHECKALL

GOTO END

::-------------------------------------------
:CHECKALL
   ECHO Check Targets:
   ECHO --------------
   set eLIBS= %eLIBS% libvt libbmatrix
   FOR %%f IN (%eLIBS%) DO (
      IF NOT EXIST ..\lib\%%~nf.lib  IF NOT EXIST ..\lib\%%~nf.dll (
         ECHO lib\%%~nf.lib...ERROR!		lib\%%~nf.dll...ERROR! ) | find /V "lib\bmatrix"
      IF EXIST ..\lib\%%~nf.lib  IF NOT EXIST ..\lib\%%~nf.dll (
         ECHO lib\%%~nf.lib...ok		lib\%%~nf.dll...ERROR! ) | find /V "lib\bmatrix"
      IF NOT EXIST ..\lib\%%~nf.lib  IF EXIST ..\lib\%%~nf.dll (
         ECHO lib\%%~nf.lib...ERROR!		lib\%%~nf.dll...ok )     | find /V "lib\bmatrix"
      IF EXIST ..\lib\%%~nf.lib  IF EXIST ..\lib\%%~nf.dll (
         ECHO lib\%%~nf.lib...ok		lib\%%~nf.dll...ok )     | find /V "lib\bmatrix"
   )
   ECHO.
   set eBINS= %eBINS% rwcread rwdread rwdcomp edb2rwc getacqstatus
   FOR %%f IN (%eBINS%) DO (
      IF EXIST ..\bin\%%~nf.exe  (
         ECHO bin\%%~nf.exe ...ok
      ) ELSE (
         ECHO bin\%%~nf.exe ...ERROR!
      )
   )

GOTO END

::-------------------------------------------
:END

