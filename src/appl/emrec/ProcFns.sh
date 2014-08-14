#!/bin/bash

# used vars:
__brick=0
__brick0=000000
__major=0
__minor=0
__id=0.0.0.0
__id0=000000.0.0.0
__pl_id=0.1.0.0
__pl0=001
__dz=300
__verbosity=2

###########################################################################

#used config files
#ToDo: check with Valera correct paths
__cfgpath=../parset
__initpath="/opera/ana/valeri/swdev/mysetsw.sh"
 
 __cfgpre_link=$__cfgpath/prelink.rootrc
__cfgfull_link=$__cfgpath/fulllink.rootrc

 __cfgpre_align=$__cfgpath/prealign.rootrc
__cfgfull_align=$__cfgpath/fullalign.rootrc

__cfgtrack=$__cfgpath/track.rootrc
__cfgtrackan=$__cfgpath/trackan.rootrc

############################################################################

#############    interface part ###################

function Setup {
  if [[ "$#" -ne 3 ]]; then
    echo "Setup must be called with 3 params: brick major minor"
    echo "Please correct your script and try again."
    echo "See you soon =)"
  else
  
    __brick=$1
    printf -v __brick0 "%06d" $__brick
    __major=$2
    __minor=$3
    __id=$__brick.0.$__major.$__minor
    __id0=$__brick0.0.$__major.$__minor

    source $__initpath

    
    rm -f align.$__id.txt
    rm -f link.$__id.txt

    echo "Welcome to the FEDRA processing script!"
    echo "We are going to process the " $__brick.0.$__major.$__minor "dataset."
  fi
}

function SetDZ {
  __dz=$1
}
function SetVerbosity {
  __verbosity=$1
}

function ResetScanSet {
  makescanset -set=$__id -reset -v=$__verbosity -dz=$__dz
}

function UpdateScanSet {
  makescanset -set=$__id -v=$__verbosity
}


function Link {
  SetFulllinkCfg
  DoLink
  RenamePlotFulllink
}
function PreLink {
  SetPrelinkCfg
  DoLink
  RenamePlotPrelink
}
function Link_Pl {
  SetFulllinkCfg
  DoLink_Pl "$@"
  PlotLink
  RenamePlotFulllink
}
function PreLink_Pl {
  SetPrelinkCfg
  DoLink_Pl "$@"
  PlotLink
  RenamePlotPrelink
}

function Align {
  SetFullalignCfg
  DoAlign
  UpdateScanSet
  RenamePlotFullalign
}
function PreAlign {
  SetPrealigCfg
  DoAlign
  UpdateScanSet
  RenamePlotPrealign
}
function Align_Pl {
  SetFullalignCfg
  DoAlign_Pl "$@"
  UpdateScanSet
  PlotAlign
  RenamePlotFullalign
}
function PreAlign_Pl {
  SetPrealigCfg
  DoAlign_Pl "$@"
  UpdateScanSet
  PlotAlign
  RenamePlotPrealign
}

function Track {
  SetTrackCfg
  DoTrack
}

function LocalCorr { # numX NumY numIterations
  SetTrackanCfg
  if [[ "$#" -eq 2 ]]; then
    DoLocalCorr $1 $2 1
  else
    if [[ "$#" -eq 3 ]]; then
      DoLocalCorr $1 $2 $3
    fi
  fi
}

function StorePreLink {
  ChangeSfxForPlateFiles 1 100 "cp.root" "pre.cp.root"
}
function RestorePreLink {
  ChangeSfxForPlateFiles 1 100 "pre.cp.root" "cp.root"
}

function StoreFullLink {
  ChangeSfxForPlateFiles 1 100 "cp.root" "full.cp.root"
}
function RestoreFullLink {
  ChangeSfxForPlateFiles 1 100 "full.cp.root" "cp.root"
}

function StorePreLink_Pl {
  ChangeSfxForPlateFiles $1 $1 "cp.root" "pre.cp.root"
}
function RestorePreLink_Pl {
  ChangeSfxForPlateFiles $1 $1 "pre.cp.root" "cp.root"
}

function StoreFullLink_Pl {
  ChangeSfxForPlateFiles $1 $1 "cp.root" "full.cp.root"
}
function RestoreFullLink_Pl {
  ChangeSfxForPlateFiles $1 $1 "full.cp.root" "cp.root"
}

function SetCustomPrelinkCfg {
  __cfgpre_link=$1
}
function SetCustomLinkCfg {
  __cfgfull_link=$1
}
function SetCustomPrealignCfg {
  __cfgpre_align=$1
}
function SetCustomAlignCfg {
  __cfgfull_align=$1
}

function SetCustomTrackCfg {
  __cfgtrack=$1
}

#############    internal part ###################
function SetPrelinkCfg {
  cp $__cfgpre_link link.rootrc
}
function SetFulllinkCfg {
  cp $__cfgfull_link link.rootrc
}
function SetPrealigCfg {
  cp $__cfgpre_align align.rootrc
}
function SetFullalignCfg {
  cp $__cfgfull_align align.rootrc
}
function SetTrackCfg {
  cp $__cfgtrack track.rootrc
}
function SetTrackanCfg {
  cp $__cfgtrackan trackan.rootrc
}


function GetPlateId {
  __pl_id=$__brick.$1.$__major.$__minor
  printf -v __pl0 "%03d" $1
}

function DoLink {
  emlink -set=$__id -new -v=$__verbosity >> link.$__id.txt
}

function DoLink_Pl {
  if [[ "$#" -eq 1 ]]; then
    DoLink_1Pl $1
  else
    if [[ "$#" -eq 2 ]]; then
      for i in `eval "echo {$1..$2}"`
      do
        DoLink_1Pl $i
      done
    else
      echo "ERROR: 1 OR 2 PARAMETERS NEEDED FOR Link_Pl/PreLink_Pl !"
    fi  
  fi
}

function DoLink_1Pl {
  GetPlateId $1
  emlink -id=$__pl_id -new -v=$__verbosity >> link.$__id.txt
}

function DoAlign {
  emalign -set=$__id -new -v=$__verbosity >> align.$__id.txt
}

function DoAlign_Pl {
  if [[ "$#" -eq 1 ]]; then
    DoAlign_1Pl $1 $1
  else
    if [[ "$#" -eq 2 ]]; then
      for i in `eval "echo {$1..$2}"`
      do
        DoAlign_1Pl $i $2
      done
    else
      echo "ERROR: 1 OR 2 PARAMETERS NEEDED FOR Align_Pl/PreAlign_Pl !"
    fi  
  fi
}

function DoAlign_1Pl {
  GetPlateId $1
  curr_plate_id=$__pl_id
  
  GetPlateId $(($1-1))
  prev_plate_id=$__pl_id
  
  emalign -A=$curr_plate_id -B=$prev_plate_id -new -v=$__verbosity >> align.$__id.txt
  
  if [[ "$1" -eq "$2" ]]; then
    GetPlateId $(($1+1))
    next_plate_id=$__pl_id
  
    emalign -A=$next_plate_id -B=$curr_plate_id -new -v=$__verbosity >> align.$__id.txt
  fi
}

function DoTrack {
  emtra -set=$__id -new -v=$__verbosity
}

function DoLocalCorr {
  for i in `eval "echo {1..$3}"`
  do
    emtrackan -set=$__id -corraff -divide="$1"x"$2" -v=$__verbosity
    DoTrack
  done
}

function ChangeSfxForPlateFiles {
  for i in `eval "echo {$1..$2}"`
  do
    GetPlateId $i
    if  [ -f  p$__pl0/$__pl_id.$3 ];
    then
      mv -f p$__pl0/$__pl_id.$3 p$__pl0/$__pl_id.$4
    fi
  done
}

function RenamePlotPrelink {
  mv -f b$__id0.link.pdf b$__id0.prelink.pdf 
}
function RenamePlotFulllink {
  mv -f b$__id0.link.pdf b$__id0.fulllink.pdf 
}

function RenamePlotPrealign {
  mv -f b$__id0.align.pdf b$__id0.prealign.pdf 
}
function RenamePlotFullalign {
  mv -f b$__id0.align.pdf b$__id0.fullalign.pdf 
}

function PlotLink {
  emlink -set=$__id -check -v=$__verbosity >> link.$__id.txt
}

function PlotAlign {
  emalign -set=$__id -check -v=$__verbosity >> align.$__id.txt
}