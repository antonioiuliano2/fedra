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
 if not exist %installdir%\include\dataio    mkdir %installdir%\include\dataio 
 if not exist %installdir%\macros            mkdir %installdir%\macros
 del /s/q %installdir%\bin
 del /s/q %installdir%\lib
 del /s/q %installdir%\include

:: Define links from include-dir to src-files; former "setlinks.sh"
:: ----------------------------------------------------------------
 set ln=win32\tools\fsutil.exe hardlink create 

 for /D %%D in (src\lib*) do for %%F in (%%D\*.h) do %ln% %PROJECT_INC%\%%~nxF %%F
 
 for %%F in (%PROJECT_SRC%\libDataConversion\dataio\*.*) do %ln% %PROJECT_INC%\dataio\%%~nxF  %%F
 for %%F in (%PROJECT_SRC%\libVt++\vt++\include\*.*)     do %ln% %PROJECT_INC%\vt++\%%~nxF    %%F
 for %%F in (%PROJECT_SRC%\libVt++\smatrix\include\*.*)  do %ln% %PROJECT_INC%\smatrix\%%~nxF %%F

:: create ProjectDef.mk in ./config-directory
:: ----------------------------------------------
(
 echo PROJECT_ROOT=%installdir%
 echo.
 echo BIN_DIR = $^(PROJECT_ROOT^)/bin
 echo LIB_DIR = $^(PROJECT_ROOT^)/lib
 echo INC_DIR = $^(PROJECT_ROOT^)/include
 echo.
) > %installdir%\src\config\ProjectDef.mk

:: create RootDef.mk and TargetsDef.mk links
:: ----------------------------------------------
 set configdir=%installdir%\src\config
 if exist %configdir%\RootDef.mk         del /q %configdir%\RootDef.mk
 if exist %configdir%\TargetsDef.mk      del /q %configdir%\TargetsDef.mk
 %ln% %configdir%\RootDef.mk                    %configdir%\RootDef.windows.mk
 %ln% %configdir%\TargetsDef.mk                 %configdir%\TargetsDef.windows.mk

:: create setup_new.cmd (and delete the old setup vars)
:: ----------------------------------------------------
(
 echo set FEDRA_ROOT=%installdir%
 echo path %%FEDRA_ROOT%%\bin;%%FEDRA_ROOT%%\lib;%%PATH%%
 echo win32\tools\setenv.exe  -u FEDRA_ROOT %installdir%
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\bin
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\lib
 echo.
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\bin
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\lib
 echo if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete
) > %installdir%\setup_new.cmd

:: copy vsvars.bat into src directory
:: ----------------------------------
 IF DEFINED VS80COMNTOOLS ( 
	copy "%VS80COMNTOOLS%"\vsvars32.bat src 
 ) ELSE (
   echo "MS Visual Studio 2005 not found" 
   IF DEFINED VS71COMNTOOLS ( 
	copy "%VS71COMNTOOLS%"\vsvars32.bat src 
   ) ELSE echo "MS Visual Studio .NET 2003 not found" 
)

:: check SySalDataIO.dll registration
:: ----------------------------------
 set regutil=reg.exe
 ECHO Check SySalDataIO.dll registration ......
 @%regutil% query HKEY_LOCAL_MACHINE\SOFTWARE\SySal2\Classes\SySalDataIO
 IF '%ERRORLEVEL%'=='0' ( 
	ECHO ok!  
 ) ELSE (
	ECHO WARNING: SySalDataIO is not registered on this machine!!!! 
 )
	
 echo.

:: copy macros
:: ------------
 copy /Y "%PROJECT_SRC%"\appl\macros\* "%installdir%"\macros

:: load environment variables
:: --------------------------
 cd %installdir%
 call setup_new.cmd

:: compilation of libraries
:: ------------------------
 set /P YesNo=Do you want to compile the libraries: [y/n]?
	IF /I '%YesNo%'=='Y' (
		cd %installdir%\src
		call makeall.cmd clean
		call makeall.cmd
	)

cd %installdir%
pause
