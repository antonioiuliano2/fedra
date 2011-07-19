# usage:
#       ./makeall.sh         - build  all libraries
#       ./makeall.sh  clean  - clean  all directories
#       ./makeall.sh  depend - create all dependencies
#

LIBS="libEdb libEbase libDataConversion libEGA libEmath libEphys libEdr libEIO 
libEMC libEdd libVt++ libAlignment libScan libShower libEmr libEDA libShowRec"

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

APPLS="appl/recset appl/rwc2edb appl/macros appl/o2root appl/comptonmap appl/m2track appl/eda appl/emrec"

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
