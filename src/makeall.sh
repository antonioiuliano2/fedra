# usage:
#       ./makeall.sh         - build all libraries
#       ./makeall.sh  clean  - clean all directories
#       ./makeall.sh  depend - create all dipendencies
#

cd libEmath
make $1

cd ../libEdb
make $1

cd ../libEGA
make $1

cd ../libEdr
make $1

cd ../libEIO
make $1

cd ../libEdd
make $1

cd ../libEMC
make $1

cd ../libEphys
make $1

cd ../libEmr
make $1

cd ../libVt++
make $1

cd ../appl/recset
make $1

cd ..
cd ..
