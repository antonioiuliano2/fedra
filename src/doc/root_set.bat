set VC_PATH=c:\Program Files\Microsoft Visual Studio\VC98
set INCLUDE=%VC_PATH%\include
set LIB=%VC_PATH%\lib
path %VC_PATH%\bin;%path%

set rootsys=D:\root_2_24
set CINTSYSDIR = %rootsys%\cint
path %rootsys%\bin;%path%

set EDB_PATH=D:\R\Edb_jul15
set LIB=%LIB%;%EDB_PATH%\src
path %EDB_PATH%\src;%path%

set SCAN_ONLINE=D:\R\ONLINE
set SCAN_DATA=D:\R\DATA
