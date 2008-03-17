#!/bin/bash

installdir=`pwd`
if [[ ${installdir##*/} == "src" ]] ; then
 installdir=`cd ..; pwd`
fi
export installdir=$installdir

cd $installdir/src
. ./makeall.sh clean
cd $installdir

# rm directories and files, if already existing
if [[ -a $installdir/bin ]] ; then
  rm -r $installdir/bin
fi
if [[ -a $installdir/include ]] ; then
  rm -r $installdir/include
fi
if [[ -a $installdir/lib ]] ; then
  rm -r $installdir/lib
fi
if [[ -a $installdir/macros ]] ; then
  rm -rf $installdir/macros
fi
#if [[ -a $installdir/config ]] ; then
#  rm -rf $installdir/config
#fi
if [[ -a $installdir/setup_new.sh ]] ; then
  rm $installdir/setup_new.sh
fi
if [[ -a $installdir/src/config/RootDef.mk ]] ; then
  rm $installdir/src/config/RootDef.mk
fi
if [[ -a $installdir/src/config/TargetsDef.mk ]] ; then
  rm $installdir/src/config/TargetsDef.mk
fi
if [[ -a $installdir/src/config/ProjectDef.mk ]] ; then
  rm $installdir/src/config/ProjectDef.mk
fi
