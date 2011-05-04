
if "%1"=="clean" goto clean

mkdir %fedra_root%\htmldoc
cd %fedra_root%\htmldoc

mkdir srctmp

for /D %%i in (..\src\*) do copy %%i\*.C srctmp
for /D %%i in (..\src\*) do copy %%i\*.h srctmp
for /D %%i in (..\src\*) do copy %%i\*.cxx srctmp

cd srctmp

echo makehtml(){                >makehtml.C
echo THtml h;                   >>makehtml.C
echo h.SetProductName("FEDRA"); >>makehtml.C
echo h.SetOutputDir("../");     >>makehtml.C
echo h.SetInputDir(".");        >>makehtml.C
echo h.MakeAll();               >>makehtml.C
echo }                          >>makehtml.C


root -b -q makehtml.C

cd ..

rmdir srctmp /S /Q

cd ..

goto end


:clean
echo clean

rmdir /S /Q %fedra_root%\htmldoc

:end

