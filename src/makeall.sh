# usage:
#       ./makeall.sh         - build  all libraries
#       ./makeall.sh  clean  - clean  all directories
#       ./makeall.sh  depend - create all dependencies
#

LIBS="libEdb libDataConversion libEGA libEmath libEphys libEdr libEIO libEMC libEdd libVt++ libScan libShower"

if [ "${ORACLE_HOME}" != "" ]; then
LIBS="${LIBS} libEOracle"
fi

for lib in ${LIBS} ; do
    if [ "$1" != "check" ]; then 
       echo 
       echo "make $1 in ${lib} ............."
    fi
    cd ${lib}
    make -j5 $1
    cd ..
done

APPLS="appl/recset appl/rwc2edb appl/tracks2edb appl/checkrun appl/o2root"
for appl in ${APPLS} ; do
    if [ "$1" != "check" ]; then 
       echo
       echo "make $1 in ${appl} ............"
    fi
    cd ${appl}
    make -j5 $1
    cd ../..
done


if [ "$1" == "" ] ; then
  echo ------------------------------------
  ./makeall.sh check
fi
