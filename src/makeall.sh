# usage:
#       ./makeall.sh         - build  all libraries
#       ./makeall.sh  clean  - clean  all directories
#       ./makeall.sh  depend - create all dipendencies
#

LIBS="libEdb libEGA libEmath libEphys libEdr libEIO libEMC libEdd libVt++"
for lib in ${LIBS} ; do
    echo 
    echo "make $1 in ${lib} ..."
    cd ${lib}
    make $1
    cd ..
done

APPLS="appl/recset"
for appl in ${APPLS} ; do
    echo 
    echo "make $1 in ${appl} ..."
    cd ${appl}
    make $1
    cd ../..
done
