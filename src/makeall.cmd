:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server

@ECHO OFF
 IF /I '%1'=='chktgt'	GOTO CHKTGT
::-----------------------------------------------------------------------

 call vsvars32.bat 

 cd libEmath
 nmake /F Makefile.w32

 cd ../libEdb
 nmake /F Makefile.w32

 cd ../libVt++
 nmake /F Makefile.w32

 cd ../libEphys
 nmake /F Makefile.w32

 cd ../libEGA
 nmake /F Makefile.w32

 cd ../libEdr
 nmake /F Makefile.w32

 cd ../libEIO
 nmake /F Makefile.w32

 cd ../libEdd
 nmake /F Makefile.w32

 cd ../libEMC
 nmake /F Makefile.w32

 cd ../appl/recset
 nmake /F Makefile.w32

 cd ../../appl/rwc2edb
 nmake /F Makefile.w32

 cd ../../appl/cp2edb
 nmake /F Makefile.w32
 
 cd ../../appl/bmatrix
 nmake /F Makefile.w32
 
 cd ../../

 ECHO Check Targets:
 ECHO --------------
 CALL %0 chktgt ..\lib\libEdb.lib
 CALL %0 chktgt ..\lib\libEdb.dll
 CALL %0 chktgt ..\lib\libEmath.lib
 CALL %0 chktgt ..\lib\libEmath.dll
 CALL %0 chktgt ..\lib\libVt++.lib
 CALL %0 chktgt ..\lib\libVt++.dll
 CALL %0 chktgt ..\lib\libvt.lib
 CALL %0 chktgt ..\lib\libvt.dll
 CALL %0 chktgt ..\lib\libEphys.lib
 CALL %0 chktgt ..\lib\libEphys.dll
 CALL %0 chktgt ..\lib\libEGA.lib
 CALL %0 chktgt ..\lib\libEGA.dll
 CALL %0 chktgt ..\lib\libEdr.lib
 CALL %0 chktgt ..\lib\libEdr.dll
 CALL %0 chktgt ..\lib\libEIO.lib
 CALL %0 chktgt ..\lib\libEIO.dll
 CALL %0 chktgt ..\lib\libEdd.lib
 CALL %0 chktgt ..\lib\libEdd.dll
 CALL %0 chktgt ..\lib\libEMC.lib
 CALL %0 chktgt ..\lib\libEMC.dll
 CALL %0 chktgt ..\bin\rwc2edb.exe
 CALL %0 chktgt ..\bin\edb2rwc.exe
 CALL %0 chktgt ..\bin\rwcread.exe
 CALL %0 chktgt ..\bin\rwdread.exe
 CALL %0 chktgt ..\bin\recset.exe
 CALL %0 chktgt ..\bin\cp2edb.exe
 CALL %0 chktgt ..\lib\bmatrix.lib
 CALL %0 chktgt ..\lib\bmatrix.dll

GOTO END

::-------------------------------------------
:CHKTGT
        IF EXIST     %2 ECHO 	%2...ok!
        IF NOT EXIST %2 ECHO 	%2...ERROR
 GOTO END
::-------------------------------------------
:END