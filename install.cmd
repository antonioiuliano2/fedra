@echo off
:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server
:: GS - 09.01.04 based on install.sh

 set installdir=%CD%
 set PROJECT_INC=%installdir%\include
 set PROJECT_SRC=%installdir%\src

:: making directories, if not already existing and clean
:: -----------------------------------------------------
 if not exist %installdir%\bin               mkdir %installdir%\bin  
 if not exist %installdir%\lib               mkdir %installdir%\lib
 if not exist %installdir%\include           mkdir %installdir%\include
 if not exist %installdir%\include\vt++      mkdir %installdir%\include\vt++    
 if not exist %installdir%\include\smatrix   mkdir %installdir%\include\smatrix 
 del /s/q %installdir%\bin
 del /s/q %installdir%\lib
 del /s/q %installdir%\include

:: Define links from include-dir to src-files; former "setlinks.sh"
:: ----------------------------------------------------------------
 set fsutil=win32\tools\fsutil.exe
 for %%F in (%PROJECT_SRC%\libEdb\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEmath\*.h)  do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEphys\*.h)  do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEGA\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEdr\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEIO\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEdd\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libEMC\*.h)    do %fsutil% hardlink create %PROJECT_INC%\%%~nF.h %%F
 for %%F in (%PROJECT_SRC%\libVt++\vt++\include\*.*)     do %fsutil% hardlink create %PROJECT_INC%\vt++\%%~nF%%~xF    %%F
 for %%F in (%PROJECT_SRC%\libVt++\smatrix\include\*.*)  do %fsutil% hardlink create %PROJECT_INC%\smatrix\%%~nF%%~xF %%F

:: create ProjectDef_w32.mk in ./config-directory
:: ----------------------------------------------
 set ProjectDef=%installdir%\src\config\ProjectDef.w32.mk
 echo PROJECT_ROOT=%installdir%> %ProjectDef%
 echo. >> %ProjectDef%
 echo BIN_DIR = $(PROJECT_ROOT)/bin>> %ProjectDef%
 echo LIB_DIR = $(PROJECT_ROOT)/lib>> %ProjectDef%
 echo INC_DIR = $(PROJECT_ROOT)/include>> %ProjectDef%
 echo. >> %ProjectDef%
 echo PROJECT_LIBS = -LIBPATH:$(LIB_DIR)>> %ProjectDef%

:: create setup_new.cmd (and delete the old setup vars)
:: ----------------------------------------------------
set SetupNew=%installdir%\setup_new.cmd
 echo set FEDRA_ROOT=%installdir%>  %SetupNew%
 echo path %%FEDRA_ROOT%%\bin;%%FEDRA_ROOT%%\lib;%%PATH%%>>  %SetupNew%
 echo win32\tools\setenv.exe  -u FEDRA_ROOT %installdir%>>  %SetupNew%
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\bin>>  %SetupNew%
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\lib>>  %SetupNew%
 echo.                                              >>  %SetupNew%
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\bin>>  %SetupNew%
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\lib>>  %SetupNew%
 echo if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete>>  %SetupNew%

:: copy vsvars.bat into src directory
:: ----------------------------------
 IF DEFINED VS71COMNTOOLS                                copy "%VS71COMNTOOLS%"\vsvars32.bat src
 IF NOT DEFINED VS71COMNTOOLS IF DEFINED VSCOMNTOOLS     copy "%VSCOMNTOOLS%"\vsvars32.bat src
 IF NOT DEFINED VS71COMNTOOLS IF NOT DEFINED VSCOMNTOOLS echo "Not found MS Visual Studio .NET or MS Visual Studio .NET 2003" 
 if exist src\vsvars.bat src\vsvars.bat 

:: compilation of libraries
:: ------------------------
 set /P YesNo=Do you want to compile the libraries: [y/n]?
	IF /I '%YesNo%'=='Y' cd %installdir%\src
	IF /I '%YesNo%'=='Y' call cleanall.cmd
	IF /I '%YesNo%'=='Y' call makeall.cmd

:: load environment variables
:: --------------------------
 cd %installdir%
 setup_new.cmd