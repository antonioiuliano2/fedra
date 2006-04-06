# usage:
#       ./makeall.sh         - build  all libraries
#       ./makeall.sh  clean  - clean  all directories
#       ./makeall.sh  depend - create all dependencies
#

LIBS="libEdb libDataConversion libEGA libEmath libEphys libEdr libEIO libEMC libEdd libVt++ libEOracle"
for lib in ${LIBS} ; do
    echo 
    echo "make $1 in ${lib} ............."
    cd ${lib}
    make -j5 $1
    cd ..
done

APPLS="appl/recset appl/rwc2edb appl/tracks2edb appl/bmatrix appl/checkrun"
for appl in ${APPLS} ; do
    echo 
    echo "make $1 in ${appl} ............"
    cd ${appl}
    make -j5 $1
    cd ../..
done
