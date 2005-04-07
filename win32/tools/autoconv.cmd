@echo off

:: note: if errorlevel x means if errorlevel .GE. x 

set _dir1=d:\temp\RAW
set _dir2=d:\temp\ROOT
set _dir3=d:\temp\TEMP

set _d1=k:
set _d2=l:
set _d3=m:

set _ext1=rwc
set _ext2=root

set _logfile=autoconv.log

if not exist %_dir2%\nul mkdir %_dir2%
if not exist %_dir3%\nul mkdir %_dir3%

subst %_d1% %_dir1%
subst %_d2% %_dir2%
subst %_d3% %_dir3%

for /R %_d1% %%f in (*.%_ext1%) do (
   if not exist %_d2%%%~pnf.%_ext2% (
      getacqstatus %%f
      if errorlevel 1 ( 
	 echo %%f terminated: convert to root %date% %time% >> %_logfile%
         if not exist %_d2%%%~pf\nul mkdir %_d2%%%~pf
            rwc2edb %%f %_d2%%%~pnf.%_ext2%
      ) else (
         echo WARNING: %%f not terminated %date% %time%   >> %_logfile% 
      )
   )

   if exist %_d2%%%~pnf.%_ext2% (
rem   checkraw %_d2%%%~pnf.%_ext2% %date% %time%         >> %_logfile%
rem   if errorlevel 1 (
            if not exist %_d3%%%~pf\nul mkdir %_d3%%%~pf
            move %%~dpnf.* %_d3%%%~pf
rem   )
   )

)

subst %_d1% /D
subst %_d2% /D 
subst %_d3% /D 

:: clear local variables
for /f "usebackq tokens=1-2 delims==" %%a in (`set _`) do set %%a=

)
