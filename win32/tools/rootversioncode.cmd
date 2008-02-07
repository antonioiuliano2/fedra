@echo off
for /F "tokens=3" %%i IN ('findstr /c:"#define ROOT_VERSION_CODE" %ROOTSYS%\include\RVersion.h') DO EXIT /B %%i

