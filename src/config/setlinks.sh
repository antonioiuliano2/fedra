#!/bin/bash

export PROJECT_INC=/home/valeri/public/fedra/include
export PROJECT_SRC=/home/valeri/public/fedra/src

listEdr=`ls $PROJECT_SRC/libEdr/*.h`
listEdd=`ls $PROJECT_SRC/libEdd/*.h`
listEdb=`ls $PROJECT_SRC/libEdb/*.h`
listEmath=`ls $PROJECT_SRC/libEmath/*.h`
listTest=`ls $PROJECT_SRC/libTest/*.h`
listVt=`ls $PROJECT_SRC/libVt++/vt++/include/*.hh`

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
for hname in ${listTest} ; do
 ln -fs $PROJECT_SRC/libTest/${hname##*/} $PROJECT_INC/${hname##*/}
done
for hname in ${listVt} ; do
 ln -fs $PROJECT_SRC/libVt++/vt++/include/${hname##*/} $PROJECT_INC/${hname##*/}
done

