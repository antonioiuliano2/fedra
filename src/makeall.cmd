:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server
::
:: usage:
::       makeall.cmd         - build  all libraries
::       makeall.cmd  clean  - clean  all directories
::       makeall.cmd  depend - create all dipendencies //not implemented yet
::


@ECHO OFF
 IF /I '%1'=='checkall'	GOTO CHECKALL
 IF /I '%1'=='chktgt'	GOTO CHKTGT
::-----------------------------------------------------------------------

 call vsvars32.bat 

 cd libEmath
 nmake %1

 cd ../libEdb
 nmake %1

 cd ../libVt++
 nmake %1 /F Makefile.w32

 cd ../libEphys
 nmake %1

 cd ../libEGA
 nmake %1

 cd ../libEdr
 nmake %1

 cd ../libEIO
 nmake %1

 cd ../libEdd
 nmake %1

 cd ../libEMC
 nmake %1

 cd ../libEdg
 nmake %1

 cd ../appl/recset
 nmake %1 /F Makefile.w32

 cd ../../appl/rwc2edb
 nmake %1 /F Makefile.w32

 cd ../../appl/cp2edb
 nmake %1 /F Makefile.w32
 
 cd ../../appl/bmatrix
 nmake %1
 
 cd ../../

 IF '%1'=='' CALL %0 CHECKALL

 GOTO END

::-------------------------------------------
:CHECKALL

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
 CALL %0 chktgt ..\lib\libbmatrix.lib
 CALL %0 chktgt ..\lib\libbmatrix.dll

GOTO END

::-------------------------------------------
:CHKTGT
        IF EXIST     %2 ECHO 	%2...ok!
        IF NOT EXIST %2 ECHO 	%2...ERROR
 GOTO END
::-------------------------------------------
:END
