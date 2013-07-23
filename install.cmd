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
 set ln=call win32\tools\ln.exe
 for /D %%D in (src\lib*) do for %%F in (%%D\*.h)        do %ln% %%F %PROJECT_INC%\%%~nxF

 for %%F in (%PROJECT_SRC%\libDataConversion\dataio\*.*) do %ln% %%F %PROJECT_INC%\dataio\%%~nxF
 for %%F in (%PROJECT_SRC%\libVt++\vt++\include\*.*)     do %ln% %%F %PROJECT_INC%\vt++\%%~nxF
 for %%F in (%PROJECT_SRC%\libVt++\smatrix\include\*.*)  do %ln% %%F %PROJECT_INC%\smatrix\%%~nxF

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
 %ln% %configdir%\RootDef.windows.mk            %configdir%\RootDef.mk
 %ln% %configdir%\TargetsDef.windows.mk         %configdir%\TargetsDef.mk

 if exist %installdir%\src\libScan\Makefile      del /q %installdir%\src\libScan\Makefile
 %ln% %installdir%\src\libScan\Makefile.w32 %installdir%\src\libScan\Makefile 

:: create setup_new.cmd (and delete the old setup vars)
:: ----------------------------------------------------
(
 echo set FEDRA_ROOT=%%~dp0%%
 echo path %%FEDRA_ROOT%%\bin;%%FEDRA_ROOT%%\lib;%%PATH%%
 echo win32\tools\setenv.exe  -u FEDRA_ROOT %%~dp0%%
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\bin
 echo win32\tools\pathman.exe /au %%%%FEDRA_ROOT%%%%\lib
 echo.
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\bin
 echo if DEFINED FEDRA win32\tools\pathman /ru %%%%FEDRA%%%%\lib
 echo if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete
 echo.
 echo if NOT DEFINED HOME copy src\appl\macros\fedra.rootrc %%%%HOMEDRIVE%%%%%%%%HOMEPATH%%%%\.rootrc
 echo if NOT DEFINED HOME win32\tools\setenv.exe  -u HOME %%%%HOMEDRIVE%%%%%%%%HOMEPATH%%%%
 echo.
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

:: copy macros
:: ------------
 ECHO copy macros ....
 copy /Y "%PROJECT_SRC%"\appl\macros\*  "%installdir%"\macros
 del /Q  "%installdir%"\macros\*.obj    "%installdir%"\Makefile

:: load environment variables
:: --------------------------
 ECHO load environment variables ....
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

:: check SySalDataIO.dll registration
:: ----------------------------------
 set regutil=reg.exe
 ECHO Check SySalDataIO.dll registration ......
 @%regutil% query HKEY_LOCAL_MACHINE\SOFTWARE\SySal2\Classes\SySalDataIO
 IF '%ERRORLEVEL%'=='0' ( 
	FOR /f "usebackq tokens=3 delims= " %%a IN (`REG QUERY HKCR\CLSID\{4B47E8CE-894C-11D3-A47C-9F3735226036}\InProcServer32 /ve`) DO dir %%a
        IF ERRORLEVEL 1  (
           ECHO WARNING: SySalDataIO class is registered but the .dll cannot be found. 
           ECHO          You need to register SySalDataIO again, please!
        ) ELSE (
           ECHO SySalDataIO registration ok!
        )
 ) ELSE (
	ECHO WARNING: SySalDataIO is not registered on this machine!!!! 
 )
 echo.

pause
