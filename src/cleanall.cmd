:: Installation script for the fedra-package
:: for Windows XP and Windows 2003 Server

cd libEmath
nmake /F Makefile.w32 clean

cd ../libEdb
nmake /F Makefile.w32 clean

cd ../libVt++
nmake /F Makefile.w32 clean

cd ../libEphys
nmake /F Makefile.w32 clean

cd ../libEGA
nmake /F Makefile.w32 clean

cd ../libEdr
nmake /F Makefile.w32 clean

cd ../libEIO
nmake /F Makefile.w32 clean

cd ../libEdd
nmake /F Makefile.w32 clean

cd ../libEMC
nmake /F Makefile.w32 clean

cd ../appl/recset
nmake /F Makefile.w32 clean

cd ../../appl/rwc2edb
nmake /F Makefile.w32 clean

cd ../../appl/cp2edb
nmake /F Makefile.w32 clean

cd ../../appl/bmatrix
nmake /F Makefile.w32 clean

cd ../..

