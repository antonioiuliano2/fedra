@echo off
:: Uninstallation script for the fedra-package
:: for Windows XP and Windows 2003 Server

 set installdir=%CD%
 set PROJECT_INC=%installdir%\include
 set PROJECT_SRC=%installdir%\src
 
 cd %installdir%\src
 call makeall.cmd clean
 cd %installdir%

 if exist %installdir%\bin     rmdir /s/q %installdir%\bin
 if exist %installdir%\lib     rmdir /s/q %installdir%\lib
 if exist %installdir%\include rmdir /s/q %installdir%\include

 set configdir=%installdir%\src\config
 if exist %configdir%\RootDef.mk    del /q %configdir%\RootDef.mk
 if exist %configdir%\TargetsDef.mk del /q %configdir%\TargetsDef.mk
 if exist %configdir%\ProjectDef.mk del /q %configdir%\ProjectDef.mk
 
 if exist %installdir%\setup_new.cmd del /q %installdir%\setup_new.cmd
 if DEFINED FEDRA_ROOT win32\tools\setenv.exe  -u FEDRA_ROOT -delete
 if DEFINED FEDRA_ROOT win32\tools\pathman /ru %%FEDRA_ROOT%%\bin
 if DEFINED FEDRA_ROOT win32\tools\pathman /ru %%FEDRA_ROOT%%\lib

 if exist %installdir%\src\vsvars32.bat del /q %installdir%\src\vsvars32.bat 

:: remove old FEDRA installation
 if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\bin
 if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\lib
 if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete
