#!/bin/bash

export PROJECT_INC=/home/valeri/public/fedra/include
export PROJECT_SRC=/home/valeri/public/fedra/src

listEdr=`ls $PROJECT_SRC/libEdr/*.h`
listEdd=`ls $PROJECT_SRC/libEdd/*.h`
listEdb=`ls $PROJECT_SRC/libEdb/*.h`
listEmath=`ls $PROJECT_SRC/libEmath/*.h`
listEphys=`ls $PROJECT_SRC/libEphys/*.h`

for hname in ${listEdr} ; do
 ln -fs $PROJECT_SRC/libEdr/${hname##*/} $PROJECT_INC/${hname##*/}
done
for hname in ${listEdd} ; do
 ln -fs $PROJECT_SRC/libEdd/${hname##*/} $PROJECT_INC/${hname##*/}
done
for hname in ${listEdb} ; do
 ln -fs $PROJECT_SRC/libEdb/${hname##*/} $PROJECT_INC/${hname##*/}
done
for hname in ${listEmath} ; do
 ln -fs $PROJECT_SRC/libEmath/${hname##*/} $PROJECT_INC/${hname##*/}
done
for hname in ${listEphys} ; do
 ln -fs $PROJECT_SRC/libEphys/${hname##*/} $PROJECT_INC/${hname##*/}
done
ln -fs $PROJECT_SRC/libVt++/vt++/include $PROJECT_INC/vt++
ln -fs $PROJECT_SRC/libVt++/smatrix/include $PROJECT_INC/smatrix


