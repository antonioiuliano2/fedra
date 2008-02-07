:: @echo off
:: Uninstallation script for the fedra-package
:: for Windows XP and Windows 2003 Server

set installdir=%CD%

::-------------------------------------
IF NOT DEFINED FEDRA_ROOT SET FEDRA_ROOT=%installdir%

IF NOT EXIST %installdir%\src\config\ProjectDef.mk (
 echo PROJECT_ROOT=%installdir%
 echo.
 echo BIN_DIR = $^(PROJECT_ROOT^)/bin
 echo LIB_DIR = $^(PROJECT_ROOT^)/lib
 echo INC_DIR = $^(PROJECT_ROOT^)/include
 echo.
) > %installdir%\src\config\ProjectDef.mk
 set ln=win32\tools\fsutil.exe hardlink create 
 set configdir=%installdir%\src\config
 if not exist %configdir%\RootDef.mk         %ln% %configdir%\RootDef.mk        %configdir%\RootDef.windows.mk
 if not exist %configdir%\TargetsDef.mk      %ln% %configdir%\TargetsDef.mk     %configdir%\TargetsDef.windows.mk
::-------------------------------------

 set PROJECT_INC=%installdir%\include
 set PROJECT_SRC=%installdir%\src
 
 pushd %installdir%\src
 call makeall.cmd clean 
 popd

 if exist %installdir%\bin     rmdir /s/q %installdir%\bin
 if exist %installdir%\lib     rmdir /s/q %installdir%\lib
 if exist %installdir%\include rmdir /s/q %installdir%\include
 if exist %installdir%\macros  rmdir /s/q %installdir%\macros

 set configdir=%installdir%\src\config
 if exist %configdir%\RootDef.mk        del /q %configdir%\RootDef.mk
 if exist %configdir%\TargetsDef.mk     del /q %configdir%\TargetsDef.mk
 if exist %configdir%\ProjectDef.mk     del /q %configdir%\ProjectDef.mk
 
 if exist %installdir%\setup_new.cmd del /q %installdir%\setup_new.cmd
 if DEFINED FEDRA_ROOT win32\tools\setenv.exe  -u FEDRA_ROOT -delete
 if DEFINED FEDRA_ROOT win32\tools\pathman /ru %%FEDRA_ROOT%%\bin
 if DEFINED FEDRA_ROOT win32\tools\pathman /ru %%FEDRA_ROOT%%\lib

 if exist %installdir%\src\vsvars32.bat del /q %installdir%\src\vsvars32.bat 

:: remove old FEDRA installation if defined
 if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\bin
 if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\lib
 if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete

:: remove MSVS temporary files
 if exist %installdir%\win32\solution\Fedra.suo del /A:H %installdir%\win32\solution\Fedra.suo
 if exist %installdir%\win32\solution\Fedra.ncb del      %installdir%\win32\solution\Fedra.ncb
 if exist %installdir%\win32\solution\Debug     rd /S/Q  %installdir%\win32\solution\Debug
 if exist %installdir%\win32\solution\Release   rd /S/Q  %installdir%\win32\solution\Release

:: remove MSVS 2005 temporary files
 if exist %installdir%\win32\solution8\Fedra.suo del /A:H %installdir%\win32\solution8\Fedra.suo
 if exist %installdir%\win32\solution8\Fedra.ncb del      %installdir%\win32\solution8\Fedra.ncb
 if exist %installdir%\win32\solution8\Debug     rd /S/Q  %installdir%\win32\solution8\Debug
 if exist %installdir%\win32\solution8\Release   rd /S/Q  %installdir%\win32\solution8\Release
 if exist %installdir%\win32\solution8\*.user    del      %installdir%\win32\solution8\*.user


