mkdir -p htmldoc/srctmp
cd htmldoc/srctmp
pwd
for a in  ../../src/lib*; do echo $a; cp -v $a/*.cxx . ; cp -v $a/*.h . ; cp -v $a/*.C .  ; cp ../../makehtml.C .;
root -b -q "makehtml.C"
done
