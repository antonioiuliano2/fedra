#!/bin/bash
# Installation script for the fedra-package   #
# 14.11.03                                    #

installdir=`pwd`
if [[ ${installdir##*/} == "src" ]] ; then
 installdir=`cd ..; pwd`
fi 
export installdir=$installdir

# making directories, if not already existing
if [[ ! -a $installdir/bin ]] ; then
  mkdir $installdir/bin
fi
if [[ ! -a $installdir/include ]] ; then
  mkdir $installdir/include
fi
if [[ ! -a $installdir/lib ]] ; then
  mkdir $installdir/lib
fi

# Define links from include-dir to src-files; former "setlinks.sh"
PROJECT_INC=$installdir/include
PROJECT_SRC=$installdir/src

listEdr=`ls $installdir/src/libEdr/*.h`
listEdd=`ls $installdir/src/libEdd/*.h`
listEdb=`ls $installdir/src/libEdb/*.h`
listEmath=`ls $installdir/src/libEmath/*.h`
listTest=`ls $installdir/src/libTest/*.h`

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

# create ProjectDef.mk in ./config-directory
ProjectDef="$installdir/src/config/ProjectDef.mk"

echo "PROJECT_ROOT=$installdir"  > $ProjectDef
echo >> $ProjectDef
echo 'BIN_DIR = $(PROJECT_ROOT)/bin' >> $ProjectDef
echo 'LIB_DIR = $(PROJECT_ROOT)/lib' >> $ProjectDef
echo 'INC_DIR = $(PROJECT_ROOT)/include' >> $ProjectDef
echo >> $ProjectDef
echo 'PROJECT_LIBS = -L$(LIB_DIR)' >> $ProjectDef


# create setup_new.sh, depending on default-shell used
usedshell=`echo $SHELL`
if [[ (${usedshell##*/} == "tcsh") ||  (${usedshell##*/} == "csh") ]] ; then 
 echo 'setenv  FEDRA_ROOT '"$installdir" > $installdir/setup_new.sh
 echo 'setenv  LD_LIBRARY_PATH $FEDRA_ROOT/lib:${LD_LIBRARY_PATH}' >> $installdir/setup_new.sh
 echo 'setenv  PATH ${PATH}:$FEDRA_ROOT/bin' >> $installdir/setup_new.sh
fi

if [[ (${usedshell##*/} == "bash") ||  ${usedshell##*/} == "sh" || ${usedshell##*/} == "ksh" ]] ; then 
 echo 'export FEDRA_ROOT='"$installdir" > $installdir/setup_new.sh
 echo 'export LD_LIBRARY_PATH=$FEDRA_ROOT/lib:${LD_LIBRARY_PATH}' >> $installdir/setup_new.sh
 echo 'export PATH=${PATH}:$FEDRA_ROOT/bin' >> $installdir/setup_new.sh
fi
export FEDRA_ROOT="$installdir"
export LD_LIBRARY_PATH=$FEDRA_ROOT/lib:${LD_LIBRARY_PATH}
export PATH=${PATH}:$FEDRA_ROOT/bin

# compilation of libraries
echo "Do you want to compile the libraries: [y/n]"
read yesno
if [[ $yesno == 'y' ]] ; then
  chmod u+x $installdir/src/makeall.sh
  cd $installdir/src
  $installdir/src/makeall.sh
fi

# compilation of recset
if [[ ! -a $installdir/bin/recset ]] ; then
  echo
  echo
  echo "Do you want to compile recset [y/n]"
  read yesno
  if [[ $yesno == 'y' ]] ; then
    cd $installdir/src/appl/recset
    gmake
    cd $installdir
  fi 
fi
  
   
 
  



 
 
